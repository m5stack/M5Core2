/*

  M5Stack Core2 M5TouchButton library version 1.0, by Rop Gonggrijp

  Implements Arduino button library compatible buttons for any chosen
  rectangle on the M5Stack Core2 touch screen.

  For this to work, M5.update() has to be ran to scan for button presses,
  so make sure to put that in the loop() part of your sketch.

  To create a button, just create a button variable that defines the area
  on the touch screen for the button. E.g.:

    TouchButton testButton(0, 0, 50, 50);

  The format is x0, y0, x1, y1 where x0, y0 is the left top of the button
  and x1, y1 is the right bottom. From here on you can use all the standard
  Arduino button functions such that textButtton.isPressed() will now tell
  you if the top left of the screen is touched.

  Buttons will be deleted from the list if their variables go out of focus,
  so if you define buttons in a subroutine, they will not be in your way
  anywhere else.

  If button areas overlap, both buttons will become pressed if the overlap
  is touched. Note that you cannot ever press two non-overlapping buttons
  simultaneously because the M5Core2 touch screen is not multi-touch.

  The three buttons BtnA, BtnB and BtnC from the older M5Stack units come
  already implemented as buttons that lie just below the screen where the
  three circles are. If you want them to be a little bigger and also cover
  the area of the screen where you may be showing labels for the buttons,
  simply raise the top of the buttons like this:

     M5.BtnA.y1 = M5.BtnB.y1 = M5.BtnC.y1 = 220;

  The screen is 320 x 240 pixels, the touch sensor is 320 x 280, 40 pixels
  are below the screen.


  Calling functions automatically

  In addition to the standard button functions, you can sepcify a function
  to be called when a button is pressed. Let's look at the following
  example:

	#include <M5Core2.h>

	void setup() {
	  M5.begin();
	}

	void loop() {
	  M5.update();
	}
	
	void myFunction(uint8_t id) {
	  Serial.println(id);
	}

	TouchButton testButton1 = TouchButton(0, 0, 100, 100, myFunction, 1);
	TouchButton testButton2 = TouchButton(100, 0, 200, 100, myFunction, 2);
    
  As you can see we supply a function name (without the brackets) and an
  8-bit id to the button definition, in addition to the coordinates of the
  button. If the button is pressed, the specified function is called with
  the id of the key as an argument. No more tedious testing for all your
  keys separately!
  
  Naturally you can also have a separate function for each key and just
  ignore the id number (it defaults to 0). Function pointer and id can also
  be retrieved or set later, with testButton1.function and testButton1.id.
  So to attach a function to the leftmost button under the screen, simply
  add:
  
    BtnA.function = myFunction;



  The button handling is a stripped version from the Arduino TouchButton
  Library v1.0 by Jack Christensen May 2011, published Mar 2012

  This work is licensed under the Creative Commons Attribution- ShareAlike
  3.0 Unported License. To view a copy of this license, visit
  http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to
  Creative Commons, 171 Second Street, Suite 300, San Francisco,
  California, 94105, USA.                               

*/

#include "M5TouchButton.h"

 
std::vector<TouchButton*> TouchButtons::_buttons;

TouchButtons::TouchButtons(touch & device) {
  _device = device;
}

void TouchButtons::read(void) {
  if (_buttons.empty()) return;
  auto tp = _device.getPressPoint();
  for ( auto button : _buttons ) {
    if (tp.x >= button->x0 && tp.x <= button->x1 && tp.y >= button->y0 && tp.y <= button-> y1) {
  	  button->setState(1);
      if (button->function != nullptr && button->wasPressed()) {
        button->function(button->id);
      }
    } else {
      button->setState(0);
    }
  }
}


TouchButton::TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*function_)(uint8_t id_) /* = nullptr */, uint8_t id_ /* = 0 */) {
  x0 = x0_;
  y0 = y0_;
  x1 = x1_;
  y1 = y1_;
  function = function_;
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

TouchButton::~TouchButton(void) {
  for(int i = 0; i < TouchButtons::_buttons.size(); ++i) {
    if (TouchButtons::_buttons[i] == this) {
      TouchButtons::_buttons.erase(TouchButtons::_buttons.begin() + i);
      return;
    }
  }
}

uint8_t TouchButton::setState(uint8_t pinVal) {
  static uint32_t ms;
  ms = millis();
  _time = ms;
  _lastState = _state;
  _state = pinVal;
  if (_state != _lastState) {
    _lastChange = ms;
    _changed = 1;
    if (_state) { _pressTime = _time; }
  }
  else {
    _changed = 0;
  }
  return _state;
}

/*----------------------------------------------------------------------*
 * isPressed() and isReleased() check the TouchButton state when it was last *
 * read, and return false (0) or true (!=0) accordingly.                *
 * These functions do not cause the TouchButton to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t TouchButton::isPressed(void) {
  return _state == 0 ? 0 : 1;
}

uint8_t TouchButton::isReleased(void) {
  return _state == 0 ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * wasPressed() and wasReleased() check the TouchButton state to see if it   *
 * changed between the last two reads and return false (0) or           *
 * true (!=0) accordingly.                                              *
 * These functions do not cause the TouchButton to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t TouchButton::wasPressed(void) {
  return _state && _changed;
}

uint8_t TouchButton::wasReleased(void) {
  return !_state && _changed && millis() - _pressTime < _hold_time;
}

uint8_t TouchButton::wasReleasefor(uint32_t ms) {
  _hold_time = ms;
  return !_state && _changed && millis() - _pressTime >= ms;
}
/*----------------------------------------------------------------------*
 * pressedFor(ms) and releasedFor(ms) check to see if the TouchButton is     *
 * pressed (or released), and has been in that state for the specified  *
 * time in milliseconds. Returns false (0) or true (1) accordingly.     *
 * These functions do not cause the TouchButton to be read.                  *
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
 * lastChange() returns the time the TouchButton last changed state,         *
 * in milliseconds.                                                     *
 *----------------------------------------------------------------------*/
uint32_t TouchButton::lastChange(void) {
  return _lastChange;
}
