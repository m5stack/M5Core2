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

    BtnA.y1 = BtnB.y1 = BtnC.y1 = 220;

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

#ifndef M5TouchButton_h
#define M5TouchButton_h
// #if ARDUINO >= 100
#include <Arduino.h>
// #else
// #include <WProgram.h>
// #endif

#include "touch.h"

class TouchButton {
  public:
    TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*function_)(uint8_t) = nullptr, uint8_t id_ = 0);
    ~TouchButton();
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    void (*function)(uint8_t);
    uint8_t id;
    
    uint8_t setState(uint8_t);
    uint8_t isPressed();
    uint8_t isReleased();
    uint8_t wasPressed();
    uint8_t wasReleased();
    uint8_t pressedFor(uint32_t ms);
    uint8_t pressedFor(uint32_t ms, uint32_t continuous_time);
    uint8_t releasedFor(uint32_t ms);
    uint8_t wasReleasefor(uint32_t ms);
    uint32_t lastChange();

  private:
    uint8_t _state;         //current TouchButton state
    uint8_t _lastState;     //previous TouchButton state
    uint8_t _changed;       //state changed since last read
    uint32_t _time;         //time of current state (all times are in ms)
    uint32_t _lastChange;   //time of last state change
    uint32_t _lastLongPress;   //time of last state change
    uint32_t _pressTime;    //press time
    uint32_t _hold_time;    //hold time call wasreleasefor
};

class TouchButtons {
  friend class TouchButton;
  public:
    TouchButtons(touch & device);
    void read();
  private:
   	static std::vector<TouchButton*> _buttons;
    touch _device;
};

#endif
