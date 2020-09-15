/*

M5Stack Core2 touch library

	Touch is provided by a FocalTech FT6336 chip, which supports two
	simultaneous touches (see below for limitation).

	This library is initialised in M5Core2.h. An instance "M5.Touch" exists
	and can be used after you call "M5.begin()".


Two-touch API

	TouchPoint_t
		Variable type to hold a touchpoint. Has members x and y that hold
		the coordinates of a touch.

	M5.update()
		In the loop() part of your sketch, call "M5.update()". This is the
		only part that talks to the touch interface. It updates the data
		used by the rest of the two-touch API.

	uint8_t M5.Touch.points
		Contains the number of touches detected: 0, 1 or 2.

	TouchPoint_t M5.Touch.point[0], M5.Touch.point[1]
		M5.Touch.point[0] and M5.Touch.point[1] hold the detected touches.
		
	bool M5.Touch.inBox(x0, y0, x1, y1)
		true if any valid touch is found in the supplied rectangle. Can be
		used directly, but you'll be happier with the TouchButton library.

	bool M5.Touch.hasChanged()
		true if anything has moved on the touch screen since the last time
		this function was called.


Note about multi-touch

	The M5Stack Core2 touch display is only multi-touch in one dimension.
	What that means is that it can detect two separate touches only if they
	occur on different vertical positions. This has to do with the way the
	touch screen is wired, it's not something that can be changed in
	software. So you will only ever see two points if they do not occur
	side-by-side. Touches that do happen side-by-side blend into one touch
	that is detected somewhere between the actual touches.

	While this limits multi-touch somewhat, you can still create multiple
	buttons and see two that are not on the same row simultaneously. You
	could also use one of the buttons below the screen as a modifier for
	something touched on the screen.


Legacy single touch API

	TouchPoint_t
		Variable type to hold a touchpoint. Has members x and y that hold
		the coordinates of a touch.

	bool M5.Touch.ispressed()
		true when the touch screen is pressed.

	TouchPoint_t M5.Touch.getPressedPoint()
		Returns the point that is being pressed, or (-1,-1) is nothing is
		pressed. In case two points are pressed, this will return one of
		them.

	HotZone_t
		Defines a zone on the screen, comes with built-in functions to test
		if a given point is within that zone. Similar but more enhanced
		functionality is provided by the TouchButton library, see below.


TouchButton library

	The TouchButton library is initialised together with the touch library,
	and offers a higher level way to interact with the touch screen. Using
	this library you can create virtual buttons for any given rectangle of
	the screen. These buttons then have the same API as the standard
	Arduino buttons, except you can define optional handler functions that
	get called whenever the button is pressed or released.

	The library is called M5TouchButton and the documentation is in the
	header file that lives at src/utility/M5TouchButton.h in this library.


Example

	The examples/Basics directory has an example sketch called "touch" that
	shows both the touch library and the TouchButton library in action.
	Should be pretty self-explanatory.

*/


#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include <Wire.h>

#define CST_DEVICE_ADDR	0x38
#define CST_INT 		39
#define MIN_INTERVAL	10

typedef struct point
{
    int x;
    int y;
}TouchPoint_t;

typedef struct HotZone
{
    HotZone(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, void (*fun)() = nullptr){
        _x0 = x0; 
        _y0 = y0;
        _x1 = x1;
        _y1 = y1;
        _fun = fun;

    }
    inline void setZone(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, void (*fun)() = nullptr )
    {
        _x0 = x0; 
        _y0 = y0;
        _x1 = x1;
        _y1 = y1;
        _fun = fun;
    }
    inline bool inHotZone(TouchPoint_t point)
    {
        if(( point.x >= _x0 )&&( point.x <=  _x1 )&&
           ( point.y >= _y0 )&&( point.y <=  _y1 ))
        {
            return true;
        }
        return false;
    }
    inline bool inHotZoneDoFun(TouchPoint_t point)
    {
        if(( point.x >= _x0 )&&( point.x <=  _x1 )&&
           ( point.y >= _y0 )&&( point.y <=  _y1 ))
        {
            if( _fun != nullptr )
            {
                _fun();
            }
            return true;
        }
        return false;
    }
    uint16_t _x0;
    uint16_t _y0; 
    uint16_t _x1;
    uint16_t _y1;

    void (*_fun)();

}HotZone_t;

class touch
{
public:
    touch();
    ~touch();
    void begin();
    bool ispressed();
    void read();
    bool inBox(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool hasChanged();
    uint8_t points;
    TouchPoint_t point[2];
	    
    TouchPoint_t getPressPoint();
    
private:
	uint32_t _lastRead;
	TouchPoint_t _p0;
	TouchPoint_t _p1;
};

#endif