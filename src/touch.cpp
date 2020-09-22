#include "touch.h"

// TouchPoint class

TouchPoint::TouchPoint(int16_t x_ /* = -1 */, int16_t y_ /* = -1 */) {
	x = x_;
	y = y_;
}

bool TouchPoint::operator ==(const TouchPoint& p) {
	return (Equals(p));
}
	
bool TouchPoint::operator !=(const TouchPoint& p) {
	return (!Equals(p));
}

void TouchPoint::operator = (const TouchPoint& p) {
	x = p.x;
	y = p.y;
}

TouchPoint::operator char*() {
	sprintf(_text, "(%d, %d)", x, y);
	return _text;
}

void TouchPoint::set(int16_t x_ /* = -1 */, int16_t y_ /* = -1 */) {
	x = x_;
	y = y_;
}

bool TouchPoint::Equals(const TouchPoint& p) {
	return (x == p.x && y == p.y);
}

bool TouchPoint::in(const TouchZone& z) {
	return (y >= z.y && y <= z.y + z.h && x >= z.x && x <= z.x + z.w);
}

bool TouchPoint::valid() {
	return (x != -1);
}

uint16_t TouchPoint::distanceTo(const TouchPoint& p) {
	int16_t dx = x - p.x;
	int16_t dy = y - p.y;
	return sqrt(dx*dx + dy*dy);
}


// TouchZone class

TouchZone::TouchZone(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_) {
	set(x_, y_, w_, h_);
}

// constructor without arguments for descendant classes
TouchZone::TouchZone() {};

void TouchZone::set(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
}

bool TouchZone::contains(const TouchPoint &p) {
	return (p.y >= y && p.y <= y + h && p.x >= x && p.x <= x + w);
}


// HotZone class (for compatibility with older M5Core2 code)

HotZone::HotZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() /* = nullptr */) {
	set(x0_, y0_, x1_ - x0_, y1_ - y0_);
	fun = fun_;
}

void HotZone::setZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() /*= nullptr */){
	set(x0_, y0_, x1_ - x0_, y1_ - y0_);
	fun = fun_;
}

bool HotZone::inHotZone(TouchPoint &p) { return contains(p); }
	
bool HotZone::inHotZoneDoFun(TouchPoint &p) {
	if (contains(p)) {
		if (fun) fun();
		return true;
	} else {
		return false;
	}
}


// TouchButton class

TouchButton::TouchButton(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, const char* name_ /* = "" */) {
	set(x_, y_, w_, h_);
	strncpy(name, name_, 15);
	_state = false;
	_time = millis();
	changed = false;
	_hold_time = -1;
	_lastChange = _time;
	_pressTime = _time;
	touch::instance->registerButton(this);
}

TouchButton::~TouchButton() {
	touch::instance->deregisterButton(this);
}

bool TouchButton::setState(bool newState) {
	static uint32_t ms;
	ms = millis();
	_time = ms;
	if (_state != newState) {
		_state = newState;
		_lastChange = ms;
		changed = true;
		if (_state) _pressTime = _time;
	} else {
		changed = false;
	}
	return _state;
}

bool TouchButton::isPressed() { return _state; }

bool TouchButton::isReleased() { return !_state; }

bool TouchButton::wasPressed() { return _state && changed; }

bool TouchButton::wasReleased() {
	return (!_state && changed && millis() - _pressTime < _hold_time);
}

bool TouchButton::wasReleasefor(uint32_t ms) {
	_hold_time = ms;
	return (!_state && changed && millis() - _pressTime >= ms);
}

bool TouchButton::pressedFor(uint32_t ms) {
	return (_state && _time - _lastChange >= ms);
}

bool TouchButton::pressedFor(uint32_t ms, uint32_t continuous_time) {
	if (_state && _time - _lastChange >= ms && _time - _lastLongPress >= continuous_time) {
		_lastLongPress = _time;
		return true;
	} 
	return false;
}

bool TouchButton::releasedFor(uint32_t ms) {
	return (!_state && _time - _lastChange >= ms);
}

uint32_t TouchButton::lastChange() { return (_lastChange); }

void TouchButton::addHandler(void (*fn)(TouchEvent&), uint16_t eventMask /* = TE_ALL */) {
	touch::instance->addHandler(fn, eventMask, this, nullptr);
}

// Gesture class

Gesture::Gesture(TouchZone fromZone_, TouchZone toZone_, const char* name_ /* = "" */, uint16_t maxTime_ /* = 500 */, uint16_t minDistance_ /* = 50 */) {
	fromZone = fromZone_;
	toZone = toZone_;
	strncpy(name, name_, 15);
	maxTime = maxTime_;
	minDistance = minDistance_;
	detected = false;
	touch::instance->registerGesture(this);
}

Gesture::~Gesture() {
	touch::instance->deregisterGesture(this);
}

bool Gesture::test(TouchEvent& e) {
	if (e.duration > maxTime) return false;
	if (e.from.distanceTo(e.to) < minDistance) return false;
	if (!e.from.in(fromZone) || !e.to.in(toZone)) return false;
	detected = true;
	return true;
}

bool Gesture::wasDetected() { return detected; }

void Gesture::addHandler(void (*fn)(TouchEvent&), uint16_t eventMask /* = TE_ALL */) {
	touch::instance->addHandler(fn, eventMask, nullptr, this);
}


// touch class

touch* touch::instance = nullptr;

touch::touch() {
	instance = this;
}

touch::~touch() {}

void touch::begin() {
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

bool touch::ispressed() {
    return ( digitalRead(CST_INT) == LOW );
}

// Single register read and write
// The value is really 8 bits, but this way I can use leaving it off to read
uint8_t touch::ft6336(uint8_t reg, int16_t value /* = -1 */) {
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
uint8_t touch::ft6336(uint8_t reg, uint8_t size, uint8_t* data) {
	Wire1.beginTransmission((uint8_t)CST_DEVICE_ADDR);
	Wire1.write(reg);
	Wire1.endTransmission();
	Wire1.requestFrom((uint8_t)CST_DEVICE_ADDR, size);
	for (uint8_t i = 0; i < size; i++) data[i] = Wire1.read();
	return 0;
}

uint8_t touch::interval(int16_t ivl) {
	if (ivl == -1) {
		return _interval;
	} else {
		ft6336(0x88, (uint8_t)ivl);
		_interval = ivl;
		return 0;
	}
}

// This is normally called from M5.update()
void touch::read() {

	changed = false;
	for ( auto button : _buttons) button->changed = false;
	for ( auto gesture : _gestures) gesture->detected = false;
	
	// Return immediately if read() is called more frequently than the
	// touch sensor updates. This prevents unnecessary I2C reads, and the
	// data can also get corrupted if reads are too close together.
	if (millis() - _lastRead < _interval) return;
	_lastRead = millis();
	
	TouchPoint p[2];
	uint8_t pts = 0;
	uint8_t p0f = 0;
	if (ispressed()) {
		uint8_t data[11];
		ft6336(0x02, 11, data);
		pts = data[0];
		if (pts > 2) return;
		if (pts) {
			// Read the data. Never mind trying to read the "weight" and
			// "size" properties or using the built-in gestures: they
			// are always set to zero.
			p0f = (data[3] >> 4) ? 1 : 0;
			p[0].x = ((data[1] << 8) | data[2]) & 0x0fff;
			p[0].y = ((data[3] << 8) | data[4]) & 0x0fff;
			if (p[0].x > 320 || p[0].y > 280) return;
			if (pts == 2) {
				p[1].x = ((data[7] << 8) | data[8]) & 0x0fff;
				p[1].y = ((data[9] << 8) | data[10]) & 0x0fff;
				if (p[1].x > 320 || p[1].y > 280) return;
			}
		}
	}
	if (p[0] != point[0] || p[1] != point[1]) {
		changed = true;
		point[0] = p[0];
		point[1] = p[1];
		points = pts;
		_point0finger = p0f;
	}
	doEvents();
}

TouchPoint touch::getPressPoint() {
	read();
	return point[0];
}


// Events processing

void touch::doEvents() {
	TouchPoint invalid;
	_finger[_point0finger].current = point[0];
	_finger[1 - _point0finger].current = point[1];
	for (uint8_t i = 0; i < 2; i++) {
		Finger& fi = _finger[i];
		TouchPoint& now = fi.current;
		TouchPoint& prev = fi.previous;
		
		// fire any taps that didn't become doubletaps.
		if (fi.tapPoint.valid() && millis() - fi.tapTime > MAX_BETWEEN_TAP) {
			fireEvent(i, TE_TAP, fi.tapPoint, invalid, 0, fi.button, nullptr);
			fi.tapPoint.set();
		}
		
		if (now == prev) {
			continue;
		} else if (!prev.valid() && now.valid()) {
			fi.startTime = millis();
			fi.startPoint = now;
			fi.button = buttonFor(now);
			fireEvent(i, TE_TOUCH, now, now, 0, fi.button, nullptr);
		} else if (prev.valid() && !now.valid()) {
			TouchEvent e = fireEvent(i, TE_RELEASE, fi.startPoint, prev, millis() - fi.startTime, fi.button, nullptr);
			if (!doGestures(e)) {
				if (prev.distanceTo(fi.startPoint) < 10 && e.duration < MAX_TAP) {
					if (fi.tapPoint.valid() && prev.distanceTo(fi.tapPoint) < 20) {
						// there was a stored tap, so it's a doubletap now
						fireEvent(i, TE_DBLTAP, fi.startPoint, invalid, 0, fi.button, nullptr);
						fi.tapPoint.set();
					} else {
						// Store tap to be fired later if it times out
						fi.tapPoint = fi.startPoint;
						fi.tapTime = millis();
					}
				} else {
					if ( fi.button) {
						if ( !(fi.button->contains(prev)) ) {
							fireEvent(i, TE_DRAGGED, fi.startPoint, prev, millis() - fi.startTime, fi.button, nullptr);
						} else {
							fireEvent(i, TE_PRESSED, fi.startPoint, prev, millis() - fi.startTime, fi.button, nullptr);
						}
					}
				}
			}
		} else {
			fireEvent(i, TE_MOVE, prev, now, millis() - fi.startTime, fi.button, nullptr);
		}
		prev = now;
	}
}

TouchEvent touch::fireEvent(uint8_t finger, uint16_t type, TouchPoint& from,
  TouchPoint& to, uint16_t duration, TouchButton* button, Gesture* gesture) {
	TouchEvent e;
	e.finger = finger;
	e.type = type;
	e.from = from;
	e.to = to;
	e.duration = duration;
	e.button = button;
	e.gesture = gesture;
	if (e.button && e.type == TE_TOUCH) e.button->setState(true);
	if (e.button && e.type == TE_RELEASE) e.button->setState(false);
	for ( auto h : _eventHandlers ) {
		if (!(h.eventMask & e.type)) continue;
		if (h.button && h.button != e.button) continue;
		if (h.gesture && h.gesture != e.gesture) continue;
		if (h.eventMask & TE_BTNONLY && !e.button) continue;
		h.fn(e);
	}
	return e;
}

void touch::addHandler(void (*fn)(TouchEvent&), uint16_t eventMask /* = TE_ALL */, TouchButton* button /* = nullptr */, Gesture* gesture /* = nullptr */) {
	EventHandler handler;
	handler.fn = fn;
	handler.eventMask = eventMask;
	handler.button = button;
	handler.gesture = gesture;
	_eventHandlers.push_back(handler);
}

void touch::delHandlers(TouchButton* button, Gesture* gesture) {
	for(int i = _eventHandlers.size() - 1; i >= 0 ; --i) {
		if (button && _eventHandlers[i].button != button) continue;
		if (gesture && _eventHandlers[i].gesture != gesture) continue;
		_eventHandlers.erase(_eventHandlers.begin() + i);
	}
}

const char* touch::eventTypeName(TouchEvent& e) {
	const char *unknown = "TE_UNKNOWN";
	const char *eventNames[NUM_EVENTS] = {
		"TE_TOUCH", 
		"TE_RELEASE",
		"TE_MOVE",
		"TE_GESTURE",
		"TE_TAP",
		"TE_DBLTAP",
		"TE_DRAGGED",
		"TE_PRESSED"
	};
	for (uint8_t i = 0; i < NUM_EVENTS; i++) {
		if ((e.type >> i) & 1) return eventNames[i];
	}
	return unknown;
}

const char* touch::eventObjName(TouchEvent& e) {
	const char *empty = "";
	if (e.button) return e.button->name;
	if (e.gesture) return e.gesture->name;
	return empty;
}
	

// Button processing

TouchButton* touch::buttonFor(TouchPoint& p) {
	for ( auto button : _buttons ) {
		if (button->contains(p)) return button;
	}
	return nullptr;
}

void touch::registerButton(TouchButton* button) {
	_buttons.push_back(button);
}

void touch::deregisterButton(TouchButton* button) {
	for(int i = 0; i < _buttons.size(); ++i) {
		if (_buttons[i] == button) {
			_buttons.erase(_buttons.begin() + i);
			delHandlers(button, nullptr);
			return;
		}
	}
}

// Gestures processing

bool touch::doGestures(TouchEvent& e) {
	for ( auto gesture : _gestures) {
		if (gesture->test(e)) {
			fireEvent(e.finger, TE_GESTURE, e.from, e.to, e.duration, nullptr, gesture);
			return true;
		}
	}
	return false;
}

void touch::registerGesture(Gesture* gesture) {
	_gestures.push_back(gesture);
}

void touch::deregisterGesture(Gesture* gesture) {
	for(int i = 0; i < _gestures.size(); ++i) {
		if (_gestures[i] == gesture) {
			_gestures.erase(_gestures.begin() + i);
			delHandlers(nullptr, gesture);
			return;
		}
	}
}