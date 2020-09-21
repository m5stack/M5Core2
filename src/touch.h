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
		
	uint8_t id0
		(Advanced uses only.) The touch sensor will keep track of a touch
		between reads. For that purpose each touch has an id, either 1 or
		0. If there are two touches and one is released, the remaining
		touch is always in point[0], but id0 will show which of the two it
		was. If there are two touches, they are always returned in the
		order of id. You will likely not need id0 unless you want to track
		gestures or some other advanced use.


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

#define CST_DEVICE_ADDR		0x38
#define CST_INT 			39

// Strangely, the value 13 leads to slightly more frequent updates than 10
// (Still not every 13 ms, often more like 15 to 20)
#define DEFAULT_INTERVAL	13

#define MAX_TAP				100
#define MAX_BETWEEN_TAP		150
#define GESTURE_MAXTIME		500
#define GESTURE_MINDIST		75

class TouchZone;

class TouchPoint {
  public:
	TouchPoint(int16_t x_ = -1, int16_t y_ = -1);
	bool operator ==(const TouchPoint& p);
	bool operator !=(const TouchPoint& p);
	void operator = (const TouchPoint& p);
	operator char*();
	void set(int16_t x_ = -1, int16_t y_ = -1);
    bool in(const TouchZone& z);
    bool Equals(const TouchPoint& p);
    bool valid();
    uint16_t distanceTo(const TouchPoint& p);
    int16_t x, y;
  private:
    char _text[12];
};
#define TouchPoint_t TouchPoint

class TouchZone {
  public:
	TouchZone();
	TouchZone(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_);
	bool operator ==(const TouchPoint& p);
	void set(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_);
    bool contains(const TouchPoint &p);
    uint16_t x, y, w, h;
};

// For compatibility with older M5Core2 code
class HotZone : public TouchZone {
  public:
	HotZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() = nullptr);
	void setZone(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fun_)() = nullptr);
	bool inHotZone(TouchPoint &p);
	bool inHotZoneDoFun(TouchPoint &p);
    void (*fun)();
};
#define HotZone_t HotZone
 
#define NUM_EVENTS	8
#define TE_TOUCH	0x0001
#define TE_RELEASE	0x0002
#define TE_MOVE     0x0004
#define TE_GESTURE	0x0008
#define TE_TAP		0x0010
#define TE_DBLTAP	0x0020
#define TE_DRAGGED	0x0040
#define TE_PRESSED	0x0080

#define TE_ALL		0x0FFF
#define TE_BTNONLY	0x1000

struct TouchEvent;

class TouchButton : public TouchZone {
  public:
	TouchButton(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, const char* name_ = "");
	~TouchButton();
	bool setState(bool);
	bool isPressed();
	bool isReleased();
	bool wasPressed();
	bool wasReleased();
	bool pressedFor(uint32_t ms);
	bool pressedFor(uint32_t ms, uint32_t continuous_time);
	bool releasedFor(uint32_t ms);
	bool wasReleasefor(uint32_t ms);
	void addHandler(void (*fn)(TouchEvent&), uint16_t eventMask = TE_ALL);
	uint32_t lastChange();
	uint8_t finger;
	bool changed;				//state changed since last read
	char name[16];
  private:
	bool _state;				//current TouchButton state
	uint32_t _time;				//time of current state (all times are in ms)
	uint32_t _lastChange;		//time of last state change
	uint32_t _lastLongPress;	//time of last state change
	uint32_t _pressTime;		//press time
	uint32_t _hold_time;		//hold time call wasreleasefor
};

class Gesture {
  public:
  	Gesture(TouchZone fromZone_, TouchZone toZone_, const char* name_ = "",
  	  uint16_t maxTime_ = GESTURE_MAXTIME,
  	  uint16_t minDistance_ = GESTURE_MINDIST);
  	~Gesture();
  	bool test(TouchEvent& e);
  	bool wasDetected();
  	void addHandler(void (*fn)(TouchEvent&), uint16_t eventMask = TE_ALL);
  	TouchZone fromZone, toZone;
  	uint16_t maxTime, minDistance;
  	char name[16];
	bool detected;
};

struct TouchEvent {
	uint8_t finger;
	uint16_t type;
	TouchPoint from;
	TouchPoint to;
	uint16_t duration;
	TouchButton* button;
	Gesture* gesture;
};

struct EventHandler {
	uint16_t eventMask;
	TouchButton* button;
	Gesture* gesture;
	void (*fn)(TouchEvent&);
};

struct Finger {
	TouchPoint current;
	TouchPoint previous;
	uint32_t startTime;
	TouchPoint startPoint;
	TouchPoint tapPoint;
	uint32_t tapTime;
	TouchButton* button;
};

class touch {
  public:
    touch();
    ~touch();
    static touch* instance;
    void begin();
    bool ispressed();
	uint8_t ft6336(uint8_t reg, int16_t value = -1);
	uint8_t ft6336(uint8_t reg, uint8_t size, uint8_t* data);
	uint8_t interval(int16_t ivl);
    void read();
    void addHandler(void (*fn)(TouchEvent&), uint16_t eventMask = TE_ALL,
      TouchButton* button = nullptr, Gesture* gesture = nullptr);
    const char* eventTypeName(TouchEvent& e);
    const char* eventObjName(TouchEvent& e);
    uint8_t points;
    bool changed;
    TouchPoint point[2];
    TouchPoint getPressPoint();
    TouchButton* buttonFor(TouchPoint& p);
  private:
	friend class TouchButton;
	friend class Gesture;
    void delHandlers(TouchButton* button, Gesture* gesture);
    void doEvents();
    bool doGestures(TouchEvent& e);
    TouchEvent fireEvent(uint8_t finger, uint16_t type, TouchPoint& from,
      TouchPoint& to, uint16_t duration, TouchButton* button,
      Gesture* gesture);
    void registerButton(TouchButton* button);
    void deregisterButton(TouchButton* button);
    void registerGesture(Gesture* gesture);
    void deregisterGesture(Gesture* gesture);
    uint8_t _interval;
	uint32_t _lastRead;
	Finger _finger[2];
	uint8_t _point0finger;
	uint8_t _tapId;
	TouchPoint _tapPoint;
	uint32_t _tapTime;
	std::vector<EventHandler> _eventHandlers;
	std::vector<Gesture*> _gestures;
	std::vector<TouchButton*> _buttons;
};

#endif