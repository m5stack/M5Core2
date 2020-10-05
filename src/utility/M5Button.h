#ifndef _M5BUTTON_H_
#define _M5BUTTON_H_

class Gesture;

#include <Arduino.h>
#include "PointAndZone.h"
#include <M5Display.h>
#include <Free_Fonts.h>
#include "utility/Config.h"
#ifdef M5Stack_M5Core2
	#include <M5Touch.h>
#endif

#define TAP_TIME			150
#define DBLTAP_TIME			200
#define LONGPRESS_TIME		0
#define GESTURE_MAXTIME		500
#define GESTURE_MINDIST		75

#define NUM_EVENTS			11
#define E_TOUCH				0x0001
#define E_RELEASE			0x0002
#define E_MOVE  			0x0004
#define E_GESTURE			0x0008
#define E_TAP				0x0010
#define E_DBLTAP			0x0020
#define E_DRAGGED			0x0040
#define E_PRESSED			0x0080
#define E_PRESSING			0x0100
#define E_LONGPRESSED		0x0200
#define E_LONGPRESSING		0x0400

#define E_ALL				0x0FFF

#define NONE				0x0120	// Special color value: transparent

#define BUTTON_FREEFONT		FSS9
#define BUTTON_TEXTFONT		1
#define BUTTON_TEXTSIZE		1
#define BUTTON_DATUM		MC_DATUM

struct ButtonColors {
	uint16_t bg;
	uint16_t text;
	uint16_t outline;
};

class Button;
class Event;

#ifdef _M5TOUCH_H_
	struct Finger {
		Point current, previous, startPoint, tapPoint;
		uint32_t startTime, tapTime;
		Button* button;
	};
#endif
	
class Event {
  public:
  	Event();
  	operator uint16_t();
	const char* typeName();
	const char* objName();
	uint8_t finger;
	uint16_t type;
	Point from, to;
	uint16_t duration;
	Button* button;
	Gesture* gesture;
};

class Button : public Zone {
  public:
    static std::vector<Button*> instances;
	Button(
	  int16_t x_, int16_t y_, int16_t w_, int16_t h_,
	  bool rot1_ = false,
	  const char* name_ = "",
	  ButtonColors off_ = {NONE, NONE, NONE},
	  ButtonColors on_ = {NONE, NONE, NONE},
	  uint8_t datum_ = BUTTON_DATUM,
	  int16_t dx_ = 0,
	  int16_t dy_ = 0,
	  uint8_t r_ = 0xFF
	);
	Button(
	  uint8_t pin_, uint8_t invert_, uint32_t dbTime_, String hw = "hw",
	  int16_t x_ = 0, int16_t y_ = 0, int16_t w_ = 0, int16_t h_ = 0,
	  bool rot1_ = false,
	  const char* name_ = "",
	  ButtonColors off_ = {NONE, NONE, NONE},
	  ButtonColors on_ = {NONE, NONE, NONE},
	  uint8_t datum_ = BUTTON_DATUM,
	  int16_t dx_ = 0,
	  int16_t dy_ = 0,
	  uint8_t r_ = 0xFF
	);
	~Button();
	operator bool();
	bool operator==(const Button& b);
	bool operator!=(const Button& b);
	bool operator==(Button* b);
	bool operator!=(Button* b);
	int16_t instanceIndex();
	bool read(bool manualRead = true);
	void fingerDown(Point pos = Point(1,1), uint8_t finger = 0);
	void fingerUp(uint8_t finger = 0);
	void fingerMove(Point pos, uint8_t finger);
	void cancel();
	bool isPressed();
	bool isReleased();
	bool wasPressed();
	bool wasReleased();
	bool pressedFor(uint32_t ms);
	bool pressedFor(uint32_t ms, uint32_t continuous_time);
	bool releasedFor(uint32_t ms);
	bool wasReleasefor(uint32_t ms);
	void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
	void delHandlers(void (*fn)(Event&) = nullptr);
	char* name();
	uint32_t lastChange();
	Event event;
	uint16_t tapTime, dbltapTime, longPressTime;
  private:
  	void init();
	bool postReleaseEvents();
	bool timeoutEvents();
  protected:
  	friend class M5Buttons;
	char _name[16];
	uint8_t _pin;
	uint16_t _dbTime;
	bool _invert;
	bool _changed, _state, _tapWait, _pressing;
	bool _longPressing, _cancelled, _manuallyRead;
	uint8_t _setState;
	uint32_t _time;
	uint32_t _lastChange, _lastLongPress, _pressTime, _hold_time;
	uint8_t _finger;
	Point _fromPt[2], _toPt[2], _currentPt[2];
	
  // visual stuff
  public:
	void draw(ButtonColors bc);
	void draw();
	void setLabel(const char* label_);
	void setFont(const GFXfont* freeFont_);
	void setFont(uint8_t textFont_ = 0);
	void setTextSize(uint8_t textSize_ = 0);
	char* label();
	ButtonColors off, on;
	Zone* drawZone;
	uint8_t datum, r;
	int16_t dx, dy;
	void (*drawFn)(Button* b, ButtonColors bc);
  protected:
	bool _compat; // For TFT_eSPI_Button emulation		
	char _label[51];
	uint8_t _textFont;
	const GFXfont* _freeFont;
	uint8_t _textSize;
};

class M5Buttons {
  public:
    static M5Buttons* instance;
    static void drawFunction(Button* button, ButtonColors bc);
	M5Buttons();
	Button* which(Point& p);
	void draw();
	void update();
	void setFont(const GFXfont* freeFont_);
	void setFont(uint8_t textFont_);
	void setTextSize(uint8_t textSize_);
	void (*drawFn)(Button* button, ButtonColors bc);
  private:
	uint8_t _textFont;
	const GFXfont* _freeFont;
	uint8_t _textSize;
	bool _leftovers;
	
	#ifdef _M5TOUCH_H_
	  public:
        // This "button" receives events when press starts outside any buttons
        // static Button background;
	  private:
		Finger _finger[2];
	#endif
};

class Gesture {
  public:
	static std::vector<Gesture*> instances;
  	Gesture(
  	  Zone& fromZone_,
  	  Zone& toZone_,
  	  const char* name_ = "",
  	  uint16_t maxTime_ = GESTURE_MAXTIME,
  	  uint16_t minDistance_ = GESTURE_MINDIST
  	);
  	~Gesture();
  	operator bool();
  	int16_t instanceIndex();
  	bool test(Point& from, Point& to, uint16_t duration);
  	bool wasDetected();
  	void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
  	void delHandlers(void (*fn)(Event&) = nullptr);
  	char* name();
	Zone* fromZone;
	Zone* toZone;
	Event event;
  	uint16_t maxTime, minDistance;
  private:
	friend class M5Buttons;
	bool _detected;
  	char _name[16];
};

struct EventHandler {
	uint16_t eventMask;
	Button* button;
	Gesture* gesture;
	void (*fn)(Event&);
};

class M5Events {
  public:
    M5Events();
    static M5Events* instance;
    Event fireEvent(
      uint8_t finger,
      uint16_t type,
      Point& from,
      Point& to,
      uint16_t duration,
      Button* button,
      Gesture* gesture
    );
    void addHandler(
      void (*fn)(Event&),
      uint16_t eventMask = E_ALL,
      Button* button = nullptr,
      Gesture* gesture = nullptr
    );
    void delHandlers(
      void (*fn)(Event&),
      Button* button,
      Gesture* gesture
    );
	std::vector<EventHandler> _eventHandlers;
};

// TFT_eSPI_Button compatibility
class TFT_eSPI_Button : public Button {
  public:
	TFT_eSPI_Button();
	void initButton(
	  TFT_eSPI *gfx, 
	  int16_t x, int16_t y, uint16_t w, uint16_t h, 
	  uint16_t outline, uint16_t fill, uint16_t textcolor,
	  char *label_,
	  uint8_t textsize_
	);
	void initButtonUL(
	  TFT_eSPI *gfx,
	  int16_t x_, int16_t y_, uint16_t w_, uint16_t h_, 
	  uint16_t outline, uint16_t fill, uint16_t textcolor, 
	  char *label_,
	  uint8_t textsize_
	);
	void     setLabelDatum(int16_t x_delta, int16_t y_delta, uint8_t datum = MC_DATUM);
	void     drawButton(bool inverted = false, String long_name = "");
	bool     contains(int16_t x, int16_t y);
	void     press(bool p);
	bool     isPressed();
	bool     justPressed();
	bool     justReleased();
};

#endif /* _M5BUTTON_H_ */