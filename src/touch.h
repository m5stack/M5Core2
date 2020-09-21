/*

M5Stack Core2 touch library

	This is the library behind the M5.Touch object that you can use to read
	from the touch sensor on the M5Stack Core2.

	This library also provides ways to describe points and rectangles on
	the sensor as well as a TouchButton class (Arduino-style buttons for
	any rectangle) and a Gesture object for recognising touch gestures and
	a number of supporting data types. You can use the built-in events to
	trigger your own functions when things happen on the sensor.


About the touch sensor in the M5Stack Core2

	Touch is provided by a FocalTech FT6336 chip, which supports two
	simultaneous touches. However, the M5Stack Core2 touch display is only
	multi-touch in one dimension. What that means is that it can detect two
	separate touches only if they occur on different vertical positions.
	This has to do with the way the touch screen is wired, it's not
	something that can be changed in software. So you will only ever see
	two points if they do not occur side-by-side. Touches that do happen
	side-by-side blend into one touch that is detected somewhere between
	the actual touches.

	While this limits multi-touch somewhat, you can still create multiple
	buttons and see two that are not on the same row simultaneously. You
	could also use one of the buttons below the screen as a modifier for
	something touched on the screen.
	
	The touch sensor extends to below the screen of the Core2: the sensor
	is 320x280 pixels, the screen is 320x240. The missing 40 pixels are
	placed below the screen, where the printed circles are. This is meant
	to simulate the three hardware buttons on the original M5Stack units.
	Note that on at least some units the touch sensor in this area only
	operates properly if the USB cable is plugged in or if the unit is
	placed on a metal surface.


Describing points and areas

	TouchPoint and TouchArea allow you to create variables of the types
	TouchPoint and TouchZone to hold a point or an area on the screen.

	TouchPoint
		Holds a point on the screen. Has members x and y that hold the
		coordinates of a touch. Values -1 for x and y indicate an invalid
		touch value.
	
	TouchZone
		Holds a rectangular area on the screen. Members x, y, w and h are
		for the x and y coordinate of the top-left corner and the width and
		height of the rectangle.
		
	The 'valid' method tests if a point is valid. Using the 'in' or
	'contains' method you can test if a point is in a zone. Here are some
	examples to make things clear.
		
		TouchPoint a;
		TouchPoint b(20, 120);
		Serial.println(a);				// (-1, -1)
		a.set(10, 30);
		Serial.println(a.valid());		// 1    
		Serial.println(b.y);			// 120
		TouchZone z(0,0,100, 100);
		Serial.println(z.w);			// 100
		Serial.println(z.contains(a));	// 1
		Serial.println(b.in(z));		// 0
		

Basic API

	The basic API provides a way to rad the data from the touch sensor.
	While you may want to use this directly. But you may want to skip using
	this API as even for simple applications the more advanced ways of
	using the touch sensor are easier.

	M5.update()
		In the loop() part of your sketch, call "M5.update()". This is the
		only part that talks to the touch interface. It updates the data
		used by the rest of the two-touch API.

	M5.Touch.changed
		Is true if M5.update() detected any changes since the last time it
		was called.

	M5.Touch.points
		Contains the number of touches detected: 0, 1 or 2.

	M5.Touch.point[0], M5.Touch.point[1]
		M5.Touch.point[0] and M5.Touch.point[1] are TouchPoints that hold
		the detected touches.
		
	So the simplest sketch to print the location where the screen is
	touched would be:
		
		#include <M5Core2.h>

		void setup() {
		  M5.begin();
		}

		void loop() {
		  M5.update();
		  if (M5.Touch.changed && M5.Touch.points == 1) {
		    Serial.println( M5.Touch.point[0] );
		  }
		}


Buttons

	You can create virtual buttons for any given rectangle on the screen by
	creating a global variable to hold the TouchButton object and providing
	the coordinates (x, y, width and height). These buttons can be used in
	two ways. You can either use them the way you would a normal Arduino
	button, or you can provide handler functions to process various events
	for the button. We'll talk about the events later, but here's a simple
	sketch that defines a button and then does something when it's pressed.

		#include <M5Core2.h>

		TouchButton b(0,0,100,100);

		void setup() {
		  M5.begin();
		}

		void loop() {
		  M5.update();
		  if (b.wasPressed()) Serial.print("* ");
		}
		
	wasPressed() will only be true once after you release the button. You
	can also use the other Arduino button functions such as isPressed()
	that is true as soon as and as long as the button is touched. Note that
	the TouchButtons only become pressed if the touch starts within the
	button, not if you swipe over it, and that it will stay pressed as long
	as the finger touches, even if it leaves the button area. You may want
	read about the events further down to distinguish between different
	kinds of touches.

	The three buttons BtnA, BtnB and BtnC from the older M5Stack units come
	already implemented as buttons that lie just below the screen where the
	three circles are. If you want them to be a little bigger and also
	cover the area of the screen where you may be showing labels for the
	buttons, simply raise the top of the buttons like this:
	
		 M5.BtnA.y0 = M5.BtnB.y0 = M5.BtnC.y0 = 220;

	Buttons are only active when their variables exist, meaning that if you
	define button variables in a function that has its own loop that calls
	M5.update(), they will not be detected anywhere else.


Gestures

	When you create a gesture you can specify two TouchZones. Whenever a
	swipe on the sensor starts in the first zone and ends in the second,
	that gesture is counted detected. Like in the following simple example
	sketch:

		#include <M5Core2.h>

		TouchZone topHalf(0,0,320,120);
		TouchZone bottomHalf(0,120,320,160);
		Gesture swipeDown(topHalf, bottomHalf);

		void setup() {
		  M5.begin();
		}

		void loop() {
		  M5.update();
		  if (swipeDown.wasDetected()) Serial.println("Swiped down!");
		}

	After the start and end zones, you can also optionally specify a name,
	a maximum time for the gesture (default 500 ms) and minimum distance
	the swipe must cover on the screen (default 75 pixels). Only the first
	gesture for which a swipe meets the criteria will be detected. 


Events

	The most powerful way to react to things happening with buttons,
	gestures or just touches and swipes on random parts of the screen is by
	using events. For this you need to define one or more event handler
	functions. This is done like this:

		void myHandler(TouchEvent& e) { ... }

	It's important to do it exactly this way, only changing the name of the
	function. You can then set things up so that this function receives
	events. Note that the function name is provided without the brackets. 

		M5.Touch.addHandler(myHandler);

		- or -

		testbutton.addHandler(myHandler);

	With the first line the function gets all events, with the second line
	only those that pertain to a specific button or gesture. Events have
	the following members:
	
		e.type
			The type of event, such as TE_TOUCH or TE_TAP, see below.
			
		e.finger
			0 or 1, whether this is the first or second finger detected
			
		e.from and e.to
			TouchPoints that say from where to where this event happened
			
		e.duration
			Duration of the event in ms
			
		e.button and e.gesture
			Pointers to the event or gesture attached to the event
				
	Here's a list of possible event types and when they're fired:
	
		TE_TOUCH
			A finger comes on the screen. If that is within the area of a
			button, a pointer to that button will be in e.button.
			
		TE_MOVE
			A finger has moved. e.from and e.to contain the coordinates. If
			this swipe started within a button, that button will be in
			e.button for all subsequent TE_MOVE and the eventual TE_RELEASE
			event, even if the finger is not on the button anymore. This
			allows buttons to be swiped.
			
		TE_RELEASE
			The e.from and e.to will hold the beginning and ending
			positions for this entire swipe, e.duration holds the time
			since the finger was placed on the screen.
			
		TE_GESTURE
			After TE_RELEASE, the first gesture that matches a swipe fires
			a TE_GESTURE, with a pointer to the gesture in e.gesture. None
			of the further events below are then generated, even if the
			gesture started on a button.
			
		TE_TAP
			Fired when a short tap on the screen is detected. Will come
			with the associated button in e.button if applicable.
			
		TE_DBLTAP
			Like TE_TAP but for a quick double tap. Note that no TE_TAP will
			be fired in this case.
			
		TE_DRAGGED
			Fired when the finger has left the button before it is lifted.
			
		TE_PRESSED
			Fired after the button is released and it wasn't a TE_TAP,
			TE_DBLTAP or TE_SWIPE. 

	When you add a handler function you can also specify what events it
	should receive by supplying it as the second argument after the handler
	function. If you want to register multiple events for the same
	function, don't register the handler twice, but simply add (or bitwise
	or) the event values. The default value there is the pseudo-event
	TE_ALL, which is simply a value with all the event bits turned on. You
	can also subtract event type values from TE_ALL to exclude them.

	If you add the pseudo-event value TE_BTNONLY to the value supplied to
	addHandler, it indicates that you only want the function to receive
	events that have a button attached. (This only makes sense if you
	register your handler with M5.touch, where it could also see event that
	were not tied to a button.)
	
	Here are some examples of ways to add a handler function:
	
		button1.addHandler(b1Function, TE_TOUCH _ TE_RELEASE);
			b1Function only get these two events for button1
			
		M5.Touch.addHandler(btnHandle, TE_ALL + TE_BTNONLY - TE_MOVE);
			btnHandle gets all events (except TE_MOVE) tied to buttons 
			
		swipeUp.addHandler(nextPage);
			Handler nextPage is called when swipeUp gesture detected.
			Note that nextpage must still be in the handler format,
			accepting the event as an argument, even when it can
			completely ignore it.
			
	If your event calls functions in e.button or e.gesture, remember that
	these are pointers. Without going into too much detail, it means it
	must do so with the -> notation, so to read the button x position, you
	would say "e.button->x".

	If you registered a function with M5.Touch and do this on an event that
	has no button attached, your program will crash. So make sure you only
	get details about buttons if you know your event has a button pointer.
	You can either test that with "if (e.button) ..." or make sure with
	TE_BTNONLY.

	Please have a look at the example sketch (see below) to understand how
	this all works and run the sketch to see all the events printed to the
	serial port. 


Legacy API

	There was a previous version of this library, and it provided a number
	of functions that were single touch only. The older version did not
	have M5.update(). Instead it used ispressed() and getPressedPoint()
	functions as well as HotZones, which provided something that worked a
	little bit like TouchButtons. This older API is still supported (the
	M5Core2 Factory Test sketch still works), but you should not use it
	for new programs. The ispressed() function specifically does not mix
	well with code that uses M5.update().


Example

	It may sound complicated when you read it all in this document, but
	it's all made to be easy to use.
	
	Under File/Examples/M5Core2/Basics/touch in your Arduino environment is
	an example sketch called "touch" that shows this library in action.
	Please have a look at it to understand how this all works and run the
	sketch to see all the events printed to the serial port. It shows
	buttons, gestures and events and should be pretty self-explanatory.

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
#define MAX_BETWEEN_TAP		200
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