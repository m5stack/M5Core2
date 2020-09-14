#include "M5TouchButton.h"

 
std::vector<TouchButton*> TouchButtons::_buttons;

void TouchButtons::read(touch &touchDevice) {
	if (_buttons.empty()) return;
	for ( auto button : _buttons ) {
		if (touchDevice.inBox(button->x0, button->y0, button->x1, button->y1)) {
			button->setState(1);
		} else {
			button->setState(0);
		}
	}
}


TouchButton::TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fnPress_)(TouchButton &) /* = nullptr */, void (*fnRelease_)(TouchButton &) /* = nullptr */, uint8_t id_ /* = 0 */) {
	x0 = x0_;
	y0 = y0_;
	x1 = x1_;
	y1 = y1_;
	fnPress = fnPress_;
	fnRelease = fnRelease_;
	id = id_;
	_state = 0;
	_time = millis();
	_lastState = _state;
	_changed = 0;
	_hold_time = -1;
	_lastChange = _time;
	_pressTime = _time;
	TouchButtons::_buttons.push_back(this);
}

TouchButton::~TouchButton() {
	for(int i = 0; i < TouchButtons::_buttons.size(); ++i) {
		if (TouchButtons::_buttons[i] == this) {
			TouchButtons::_buttons.erase(TouchButtons::_buttons.begin() + i);
			return;
		}
	}
}

uint8_t TouchButton::setState(uint8_t newState) {
	static uint32_t ms;
	ms = millis();
	_time = ms;
	_lastState = _state;
	_state = newState;
	if (_state != _lastState) {
		_lastChange = ms;
		_changed = 1;
		if (_state) {
			_pressTime = _time;
			if (fnPress) fnPress(*this);
		} else {
			if (fnRelease) fnRelease(*this);
		}
	}
	else {
		_changed = 0;
	}
	return _state;
}

/*----------------------------------------------------------------------*
 * isPressed() and isReleased() check the TouchButton state when it was
 * last read, and return false (0) or true (!=0) accordingly.
 * These functions do not cause the TouchButton to be read.
 *----------------------------------------------------------------------*/
uint8_t TouchButton::isPressed() {
	return _state == 0 ? 0 : 1;
}

uint8_t TouchButton::isReleased() {
	return _state == 0 ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * wasPressed() and wasReleased() check the TouchButton state to see if it
 * changed between the last two reads and return false (0) or
 * true (!=0) accordingly.
 * These functions do not cause the TouchButton to be read.
 *----------------------------------------------------------------------*/
uint8_t TouchButton::wasPressed() {
	return _state && _changed;
}

uint8_t TouchButton::wasReleased() {
	return !_state && _changed && millis() - _pressTime < _hold_time;
}

uint8_t TouchButton::wasReleasefor(uint32_t ms) {
	_hold_time = ms;
	return !_state && _changed && millis() - _pressTime >= ms;
}
/*----------------------------------------------------------------------*
 * pressedFor(ms) and releasedFor(ms) check to see if the TouchButton is
 * pressed (or released), and has been in that state for the specified
 * time in milliseconds. Returns false (0) or true (1) accordingly.
 * These functions do not cause the TouchButton to be read.
 *----------------------------------------------------------------------*/
uint8_t TouchButton::pressedFor(uint32_t ms) {
	return (_state == 1 && _time - _lastChange >= ms) ? 1 : 0;
}

uint8_t TouchButton::pressedFor(uint32_t ms, uint32_t continuous_time) {
	if (_state == 1 && _time - _lastChange >= ms && _time - _lastLongPress >= continuous_time) {
		_lastLongPress = _time;
		return 1;
	} 
	return 0;
}

uint8_t TouchButton::releasedFor(uint32_t ms) {
	return (_state == 0 && _time - _lastChange >= ms) ? 1 : 0;
}
/*----------------------------------------------------------------------*
 * lastChange() returns the time the TouchButton last changed state,
 * in milliseconds.																											*
 *----------------------------------------------------------------------*/
uint32_t TouchButton::lastChange() {
	return _lastChange;
}
