/*

M5Stack Core2 M5TouchButton library version 1.0

	Implements Arduino button library compatible buttons for any chosen
	rectangle on the M5Stack Core2 touch screen. Up to two buttons can be
	pressed simultaneously.


Basic usage

	For this to work, M5.update() has to be ran to scan for button presses,
	so make sure to put that in the loop() part of your sketch.

	To create a button, just create a button variable that defines the area
	on the touch screen for the button. E.g.:

		TouchButton testButton(0, 0, 50, 50);

	The format is x0, y0, x1, y1 where x0, y0 is the left top of the button
	and x1, y1 is the right bottom. From here on you can use all the
	standard Arduino button functions such that testButtton.isPressed()
	will now tell you if the top left of the screen is touched.

	Buttons will be deleted from the list if their variables go out of
	focus, so if you define buttons in a subroutine, they will not be in
	your way anywhere else.

	If button areas overlap, both buttons will become pressed if the
	overlap is touched. Note that you cannot ever press two non-overlapping
	buttons simultaneously because the M5Core2 touch screen is not
	multi-touch.

	The three buttons BtnA, BtnB and BtnC from the older M5Stack units come
	already implemented as buttons that lie just below the screen where the
	three circles are. If you want them to be a little bigger and also
	cover the area of the screen where you may be showing labels for the
	buttons, simply raise the top of the buttons like this:

		 M5.BtnA.y0 = M5.BtnB.y0 = M5.BtnC.y0 = 220;

	The screen is 320 x 240 pixels, the touch sensor is 320 x 280, 40
	pixels are below the screen.
	

Note about multi-touch

	The M5Stack Core2 touch display is only multi-touch in one dimension.
	What that means is that it can detect two separate touches only if they
	occur on different vertical positions. This has to do with the way the
	touch screen is wired, it's not something that can be changed in
	software. So you will only ever see two points if they do not occur
	side-by-side. Touches that do happen side-by-side blend into one touch
	that is detected somewhere between the actual touches.

	While this limits multi-touch somewhat, you can still create multiple
	buttons and see two that are not on the same row simultaneously.
	

Calling functions automatically

	In addition to the coordinates, you can optionally specify a
	function to be called when a button is pressed, a function for when
	it's released, as well as an id number to figure out what button was
	pressed if multiple buttons are handled by the same function.
	
	Naturally you can also have a separate function for each key in which
	case you doon't need to supply an id number (it defaults to 0).
	Function pointers and id can also be retrieved or set later, with
	btn.fnPress, btn.fnRelease and fn.id. So to attach a function to the
	leftmost button under the screen, simply say:
	
		M5.BtnA.fnPress = myFunction;
		
		
Example

	The examples/Basics directory has an example sketch called "touch" that
	shows both the touch library and the TouchButton library in action.
	Should be pretty self-explanatory.														

*/

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
