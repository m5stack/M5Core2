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
		TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fnPress_)(TouchButton &) = nullptr, void (*fnRelease_)(TouchButton &) = nullptr, uint8_t id_ = 0);
		~TouchButton();
		uint16_t x0;
		uint16_t y0;
		uint16_t x1;
		uint16_t y1;
		void (*fnPress)(TouchButton &);
		void (*fnRelease)(TouchButton &);
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
		uint8_t _state;					//current TouchButton state
		uint8_t _lastState;			//previous TouchButton state
		uint8_t _changed;				//state changed since last read
		uint32_t _time;					//time of current state (all times are in ms)
		uint32_t _lastChange;		//time of last state change
		uint32_t _lastLongPress;	 //time of last state change
		uint32_t _pressTime;		//press time
		uint32_t _hold_time;		//hold time call wasreleasefor
};

class TouchButtons {
	friend class TouchButton;
	public:
		void read(touch &touchDevice);
	private:
		static std::vector<TouchButton*> _buttons;
};

#endif
