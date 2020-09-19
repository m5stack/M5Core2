#include "touch.h"

// TouchPoint class

TouchPoint::TouchPoint(int16_t x_ /* = -1 */, int16_t y_ /* = -1 */) {
	x = x_;
	y = y_;
}

bool TouchPoint::operator ==(const TouchPoint& p) {
	return (x == p.x && y == p.y);
}
	
bool TouchPoint::operator !=(const TouchPoint& p) {
	return (x != p.x || y != p.y);
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

bool TouchPoint::in(const TouchZone &z) {
    return (y >= z.y0 && y <= z.y1 && x >= z.x0 && x <= z.x1);
}

bool TouchPoint::Equals(const TouchPoint& p) {
	return (x == p.x && y == p.y);
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

TouchZone::TouchZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_) {
	set(x0_, y0_, x1_, y1_);
}

// empty constructor for descendant classes
TouchZone::TouchZone() {};

void TouchZone::set(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_) {
	x0 = x0_;
	y0 = y0_;
	x1 = x1_;
	y1 = y1_;
}

bool TouchZone::contains(const TouchPoint &p) {
	return (p.y >= y0 && p.y <= y1 && p.x >= x0 && p.x <= x1);
}


// HotZone class (for compatibility with older M5Core2 code)

HotZone::HotZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() /* = nullptr */) {
	set(x0_, y0_, x1_, y1_);
	fun = fun_;
}

void HotZone::setZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() /*= nullptr */){
	set(x0_, y0_, x1_, y1_);
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

TouchButton::TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, const char* name_ /* = "" */) {
	set(x0_, y0_, x1_, y1_);
	strncpy(name, name_, 15);
	_state = false;
	_time = millis();
	_lastState = _state;
	_changed = false;
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
	_lastState = _state;
	_state = newState;
	if (_state != _lastState) {
		_lastChange = ms;
		_changed = true;
		if (_state) {
			_pressTime = _time;
		}
	}
	else {
		_changed = false;
	}
	return _state;
}

bool TouchButton::isPressed() { return _state; }

bool TouchButton::isReleased() { return !_state; }

bool TouchButton::wasPressed() { return _state && _changed; }

bool TouchButton::wasReleased() {
	return (!_state && _changed && millis() - _pressTime < _hold_time);
}

bool TouchButton::wasReleasefor(uint32_t ms) {
	_hold_time = ms;
	return (!_state && _changed && millis() - _pressTime >= ms);
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
	_detected = false;
	touch::instance->registerGesture(this);
}

Gesture::~Gesture() {
	touch::instance->deregisterGesture(this);
}

bool Gesture::test(TouchEvent& e) {
	_detected = false;
	if (e.duration > maxTime) return false;
	if (e.from.distanceTo(e.to) < minDistance) return false;
	if (!e.from.in(fromZone) || !e.to.in(toZone)) return false;
	_detected = true;
	return true;
}

bool Gesture::detected() { return _detected; }

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
    Wire1.beginTransmission(CST_DEVICE_ADDR);
    Wire1.write(0xA4);
    Wire1.write(0x00);
    Wire1.endTransmission();
}

bool touch::ispressed() {
    return ( digitalRead(CST_INT) == LOW );
}

// This is normally called from M5.update()
void touch::read() {
	// Return immediately if read() is called more frequently than the
	// touch sensor updates. This prevents unnecessary I2C reads, and the
	// data can also get corrupted if reads are too close together.
	if (millis() - _lastRead < MIN_INTERVAL) return;
	_lastRead = millis();
	
	TouchPoint p[2];
	uint8_t pts = 0;
	uint8_t p0f = 0;
	if (ispressed()) {
		const uint8_t size = 11;
		uint8_t data[size];
		Wire1.beginTransmission((uint8_t)CST_DEVICE_ADDR);
		Wire1.write(0x02);
		Wire1.endTransmission();
		Wire1.requestFrom((uint8_t)CST_DEVICE_ADDR, size);
		for (uint8_t i = 0; i < size; i++) {
			data[i] = Wire1.read();
		}
		pts = data[0];
		if (pts > 2) return;
		if (pts) {
			// Read the data. Never mind trying to read the "weight" and
			// "size" properties or using the built-in gestures: they
			// are all set to zero. Probably needs more expensive 
			// touch-sensor.
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
	point[0] = p[0];
	point[1] = p[1];
	points = pts;
	point0finger = p0f;
	
	doEvents();
}

bool touch::in(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	for (uint8_t n = 0; n < points; n++) {
		if (point[n].y >= y0 && point[n].y <= y1 && point[n].x >= x0 && point[n].x <= x1) {
			return true;
		}
	}
	return false;
}

bool touch::in(TouchZone& z) { return in(z.x0, z.x1, z.y0, z.y1); }

// Note that you can only use this in one loop: once you've read the value
// it is reset.
bool touch::hasChanged() {
	if (point[0] != _previous[0] || point[1] != _previous[1]) {
		_previous[0] = point[0];
		_previous[1] = point[1];
		return true;
	} else {
		return false;
	}
}

TouchPoint touch::getPressPoint() {
	read();
	return point[0];
}


// Events processing

void touch::doEvents() {
	TouchPoint invalid;
	_finger[point0finger].current = point[0];
	_finger[1 - point0finger].current = point[1];
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
			fireEvent(i, TE_TOUCH, now, invalid, 0, fi.button, nullptr);
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

const char* touch::eventTypeName(TouchEvent& e) {
	const char *unknown = "TE_UNKNOWN";
	const char *eventNames[6] = {
		"TE_TOUCH", 
		"TE_RELEASE",
		"TE_MOVE",
		"TE_TAP",
		"TE_DBLTAP",
		"TE_GESTURE"
	};
	for (uint8_t i = 0; i < 6; i++) {
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

void touch::doButtons(TouchEvent& e) {
	if (e.type == TE_TOUCH) {
		_finger[e.finger].button = nullptr;
		for ( auto button : _buttons ) {
			if (button->contains(e.from)) {
				_finger[e.finger].button = button;
				button->setState(true);
				return;
			}
		}
	}
	if (e.type == TE_RELEASE) {
		TouchButton* button = _finger[e.finger].button;
		if (button) {
			button->setState(false);
		}
	}
}

void touch::registerButton(TouchButton* button) {
	_buttons.push_back(button);
}

void touch::deregisterButton(TouchButton* button) {
	for(int i = 0; i < _buttons.size(); ++i) {
		if (_buttons[i] == button) {
			_buttons.erase(_buttons.begin() + i);
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
			return;
		}
	}
}