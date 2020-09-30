#include <M5Core2.h>

// M5Touch class

/* static */ M5Touch* M5Touch::instance;

M5Touch::M5Touch() {
	if (!instance) instance = this;
}

void M5Touch::begin() {
    Wire1.begin(21,22);
    pinMode(CST_INT, INPUT);

	// By default, the FT6336 will pulse the INT line for every touch event.
	// But because it shares the Wire1 TwoWire/I2C with other devices, we
	// cannot create an interrupt service routine to handle these events.
	// So instead, we set the INT wire to polled mode, so it simply goes low
	// as long as there is at least one valid touch.
    ft6336(0xA4, 0x00);
    
    interval(DEFAULT_INTERVAL);
}

bool M5Touch::ispressed() {
    return ( digitalRead(CST_INT) == LOW );
}

// Single register read and write
// The value is really 8 bits, but this way I can use leaving it off to read
uint8_t M5Touch::ft6336(uint8_t reg, int16_t value /* = -1 */) {
	if (value == -1) {
		Wire1.beginTransmission((uint8_t)CST_DEVICE_ADDR);
		Wire1.write(reg);
		Wire1.endTransmission();
		Wire1.requestFrom((uint8_t)CST_DEVICE_ADDR, uint8_t(1));
		return Wire1.read();
	} else {
		Wire1.beginTransmission(CST_DEVICE_ADDR);
		Wire1.write(reg);
		Wire1.write((uint8_t)value);
		Wire1.endTransmission();
		return 0;
	}
}

// Reading size bytes into data
uint8_t M5Touch::ft6336(uint8_t reg, uint8_t size, uint8_t* data) {
	Wire1.beginTransmission((uint8_t)CST_DEVICE_ADDR);
	Wire1.write(reg);
	Wire1.endTransmission();
	Wire1.requestFrom((uint8_t)CST_DEVICE_ADDR, size);
	for (uint8_t i = 0; i < size; i++) data[i] = Wire1.read();
	return 0;
}

uint8_t M5Touch::interval(int16_t ivl) {
	if (ivl == -1) {
		return _interval;
	} else {
		ft6336(0x88, (uint8_t)ivl);
		_interval = ivl;
		return 0;
	}
}

// This is normally called from update()
bool M5Touch::read() {
	changed = false;
	
	// Return immediately if read() is called more frequently than the
	// touch sensor updates. This prevents unnecessary I2C reads, and the
	// data can also get corrupted if reads are too close together.
	if (millis() - _lastRead < _interval) return false;
	_lastRead = millis();
	
	Point p[2];
	uint8_t pts = 0;
	uint8_t p0f = 0;
	if (ispressed()) {
		uint8_t data[11];
		ft6336(0x02, 11, data);
		pts = data[0];
		if (pts > 2) return false;
		if (pts) {
			// Read the data. Never mind trying to read the "weight" and
			// "size" properties or using the built-in gestures: they
			// are always set to zero.
			p0f = (data[3] >> 4) ? 1 : 0;
			p[0].x = ((data[1] << 8) | data[2]) & 0x0fff;
			p[0].y = ((data[3] << 8) | data[4]) & 0x0fff;
			if (p[0].x > 320 || p[0].y > 280) return false;
			if (pts == 2) {
				p[1].x = ((data[7] << 8) | data[8]) & 0x0fff;
				p[1].y = ((data[9] << 8) | data[10]) & 0x0fff;
				if (p[1].x > 320 || p[1].y > 280) return false;
			}
		}
	}
	
	if (rotation != 1) {
		for (uint8_t i = 0; i < 2; i++) {
			if (p[i].valid()) p[i].rotate(rotation);
		}
	}
	
	if (p[0] != point[0] || p[1] != point[1]) {
		changed = true;
		point[0] = p[0];
		point[1] = p[1];
		points = pts;
		point0finger = p0f;
	}
	return true;
}

Point M5Touch::getPressPoint() {
	read();
	return point[0];
}

#ifndef _M5BUTTON_H_

	void M5Touch::update() {
		read();
	}

#else

	void M5Touch::update() {
		doEvents();
	}

	// Events processing

	void M5Touch::doEvents() {
		Point invalid;
		BUTTONS->setUnchanged();
		for ( auto gesture : Gesture::instances) gesture->detected = false;

		for (uint8_t i = 0; i < 2; i++) {
			Finger& fi = _finger[i];
			Event e = fi.process;
			fi.process = Event();
			if (e.type) {
				if (doGestures(e)) return;
				if (e.duration < MAX_TAP) {
					if (fi.tapPoint.valid()) {
						// there was a stored tap, so it's a doubletap now
						EVENTS->fireEvent(i, E_DBLTAP, fi.startPoint, invalid, 0, fi.button, nullptr);
						fi.tapPoint.set();
						return;
					} else {
						// Store tap to be fired later if it times out
						fi.tapPoint = fi.startPoint;
						fi.tapTime = millis();
					}
				} else {
					if (fi.button) {
						if ( !(fi.button->contains(e.to)) ) {
							EVENTS->fireEvent(i, E_DRAGGED, fi.startPoint, e.to, millis() - fi.startTime, fi.button, nullptr);
						} else {
							EVENTS->fireEvent(i, E_PRESSED, fi.startPoint, e.to, millis() - fi.startTime, fi.button, nullptr);
						}
						return;
					}
				}
			} else {
				// Timeouts
				if (fi.tapPoint.valid() && millis() - fi.tapTime > MAX_BETWEEN_TAP) {
					EVENTS->fireEvent(i, E_TAP, fi.tapPoint, invalid, 0, fi.button, nullptr);
					fi.tapPoint.set();
					return;
				}
			}
		}

		if (!read()) return;
		_finger[point0finger].current = point[0];
		_finger[1 - point0finger].current = point[1];
		
		for (uint8_t i = 0; i < 2; i++) {
			Finger& fi = _finger[i];
			Point& curr = fi.current;
			Point& prev = fi.previous;
		
			if (curr == prev) continue;
			if (!prev.valid() && curr.valid()) {
				fi.startTime = millis();
				fi.startPoint = curr;
				fi.button = BUTTONS->which(curr);
				if (fi.button) fi.button->setState(true);
				EVENTS->fireEvent(i, E_TOUCH, curr, curr, 0, fi.button, nullptr);
			} else if (prev.valid() && !curr.valid()) {
				if (fi.button) fi.button->setState(false);
				fi.process = EVENTS->fireEvent(i, E_RELEASE, fi.startPoint, prev, millis() - fi.startTime, fi.button, nullptr);
			} else {
				EVENTS->fireEvent(i, E_MOVE, prev, curr, millis() - fi.startTime, fi.button, nullptr);
			}
			prev = curr;
			return;
		}
	}

	// Gestures processing

	bool M5Touch::doGestures(Event& e) {
		for ( auto gesture : Gesture::instances) {
			if (gesture->test(e)) {
				EVENTS->fireEvent(e.finger, E_GESTURE, e.from, e.to, e.duration, nullptr, gesture);
				return true;
			}
		}
		return false;
	}

#endif /* _M5BUTTON_H_ */




// HotZone class (for compatibility with older M5Core2 code)

HotZone::HotZone(
  int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_,
  void (*fun_)() /* = nullptr */
) : Zone(x0_, y0_, x1_ - x0_, y1_ - y0_) {
	fun = fun_;
}

void HotZone::setZone(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_, void (*fun_)() /*= nullptr */){
	set(x0_, y0_, x1_ - x0_, y1_ - y0_);
	fun = fun_;
}

bool HotZone::inHotZone(Point &p) { return contains(p); }
	
bool HotZone::inHotZoneDoFun(Point &p) {
	if (contains(p)) {
		if (fun) fun();
		return true;
	} else {
		return false;
	}
}
