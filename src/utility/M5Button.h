/*

== M5Button: Buttons, Gestures and Events ==

  * Hardware button support that is 100% Arduino Button Library compatible.

  * Buttons on the screen, either as labels above the original M5Stack's
    hardware buttons or anywhere on the touch screen of the Core2.

  * Zone and Point objects to work with screen locations are areas. Functions
    for distance, direction and more.

  * Touch gestures that are processed before the buttons, so you can still
    use gestures when the screen is full of buttons.

  * Buttons and gestures send events that you can attach handler routines to,
    or poll in a loop. Events include tap, doubletap, pressed, dragged and
    more. Support for key repeat.

  * Extensive rotation support, including support for buttons and gestures
    that stay referenced to the physical screen regardless of rotation.

  * Intuitive, consistent and well-documented API.

  * Emulation of the (much less feature-rich) TFT_eSPI_Button class. This
    goes together well with M5Touch's emulation of the TFT_eSPI resistive
    touch screen interface to run a lot of existing programs without
    modification.

  This library was written for the M5Stack series of devices, but was made to
  be general enough to be produce pretty visual buttons with any TFT_eSPI
  display. Its more advanced features need the M5Touch interface, although
  other input methods could be implemented.


== Point and Zone: Describing Points and Areas on the Screen ==

  The Point and Zone classes allow you to create variables that hold a point
  or an area on the screen. You can

  Point(x, y)
    Holds a point on the screen. Has members x and y that hold the
    coordinates of a touch. Values -1 for x and y indicate an invalid value,
    and that's what a point starts out with if you declare it without
    parameters. The 'valid()' method tests if a point is valid. If you
    explicitly evaluate a Point as a boolean ("if (p) ..."), you also get
    whether the point is valid, so this is equivalent to writing "if
    (p.valid()) ...".

  Zone(x, y, w, h)
    Holds a rectangular area on the screen. Members x, y, w and h are for the
    x and y coordinate of the top-left corner and the width and height of the
    rectangle.

  The 'set' method allows you to change the properties of an existing Point
  or Zone. Using the 'in' or 'contains' method you can test if a point lies
  in a zone.

  There is also a 'distanceTo' method to see how far two points are apart and
  'directionTo' will tell you compass direction from one point to another.
  Both are used for example in the gesture recognition of the M5Button
  library.

  The PointAndZone library also provides the low-level support for screen
  rotation translations. The 'Zone' object actually has a fifth argument
  called 'rot1'.

  The documentation in src/utility/PointAndZone.h provides more details about
  rotation and examples covering most of the above.


== Buttons ==

  You can create classic Arduino buttons that act on the voltage on a pin on
  the controller. On the M5Stack Core2, you can also create buttons that act
  on touch within a given rectangle on the screen. If you want, that same
  rectangle will also be used for a graphical representation of the button
  that will show a text label in a colored background with a colored outline.
  The colors of background, text, and outline can be configured, both for the
  'off' and the 'on' state of the button.

  Whether on the M5Stack with hardware buttons or on the Core2 with a touch
  screen, buttons are special forms of the 'Zone' object, meaning that all
  functions that apply to 'Zone' objects also work on buttons. On the M5Stack
  with buttons, while this zone cannot be used for touch input, it can still
  be used to display a button that responds to the button state.


== Hardware Buttons ==

  For hardware buttons, use the classic Arduino way of setting up the button,
  by providing the pin, whether the pin is inverted and the debounce time.

    Button myButton(39, true, 10);

    #include <M5Stack.h>

    Button myButton(39, true, 10);

    void setup() {
      M5.begin();
    }

    void loop() {
      M5.update();
      if (myButton.wasPressed()) Serial.print("* ");
    }

  This would set up 'myButton' with the inverse state of pin 39, and a
  debounce time of 10 milliseconds. Because pin 39 is the left button on an
  M5Stack with buttons, this sketch will output a star to the serial port
  every time you release the button.

    Note that the sketch uses 'M5.update()' instead of 'myButton.read()'. You
    don't need to read() your buttons explicitly anymore. All buttons created
    with M5Button are automatically read by 'M5.update()'. (Unless you read
    them with 'myButton.read()', in which case 'M5.update()' stops doing that
    to avoid you missing things.)

  The next sections will describe buttons on gestures on the touch screen, but if you have an M5Stack device without a touch screen, make sure you pick up at "Events", because many events work on hardware buttons too. Hardware buttons can have responsive labels on the screen, we'll get to that also.


== Buttons Using the Touch Screen ==


  All you need to do is creating a variable to hold the Button object and providing the
  coordinates (x, y, width and height). These buttons can be used in two
  ways. You can either use them the way you would a normal Arduino button, or
  you can provide handler functions to process various events for the button.
  We'll talk about the events later, but here's a simple sketch that defines
  a button and then does something when it's pressed.

    #include <M5Core2.h>

    Button b(0,0,100,100);

    void setup() {
      M5.begin();
    }

    void loop() {
      M5.update();
      if (b.wasPressed()) Serial.print("* ");
    }

  wasPressed() will only be true once after you release the button. You can
  also use the other Arduino button functions such as isPressed() that is
  true as soon as and as long as the button is touched. Note that the Buttons
  only become pressed if the touch starts within the button, not if you swipe
  over it, and that it will stay pressed as long as the finger touches, even
  if it leaves the button area. You may want read about the events further
  down to distinguish between different kinds of touches.

  The three buttons BtnA, BtnB and BtnC from the older M5Stack units come
  already implemented as buttons that lie just below the screen where the
  three circles are. If you want them to be a little bigger and also cover
  the area of the screen where you may be showing labels for the buttons,
  simply raise the top of the buttons like this:

     M5.BtnA.y0 = M5.BtnB.y0 = M5.BtnC.y0 = 220;

  Buttons are only active when their variables exist, meaning that if you
  define button variables in a function that has its own loop that calls
  M5.update(), they will not be detected anywhere else.


== Gestures on the Touch Screen ==

  When you create a gesture you can specify two Zones. Whenever a swipe on
  the sensor starts in the first zone and ends in the second, that gesture is
  counted detected. Like in the following simple example sketch:

    #include <M5Core2.h>

    Zone topHalf(0,0,320,120);
    Zone bottomHalf(0,120,320,160);
    Gesture swipeDown(topHalf, bottomHalf);

    void setup() {
      M5.begin();
    }

    void loop() {
      M5.update();
      if (swipeDown.wasDetected()) Serial.println("Swiped down!");
    }

  After the start and end zones, you can also optionally specify a name, a
  maximum time for the gesture (default 500 ms) and minimum distance the
  swipe must cover on the screen (default 75 pixels). Only the first gesture
  for which a swipe meets the criteria will be detected.


== Events ==

  The most powerful way to react to things happening with buttons, gestures
  or just touches and swipes on random parts of the screen is by using
  events. For this you need to define one or more event handler functions.
  This is done like this:

    void myHandler(Event& e) { ... }

  It's important to do it exactly this way, only changing the name of the
  function. You can then set things up so that this function receives events.
  Note that the function name is provided without the brackets.

    M5.Touch.addHandler(myHandler);

    - or -

    testbutton.addHandler(myHandler);

  With the first line the function gets all events, with the second line only
  those that pertain to a specific button or gesture. Events have the
  following members:

    e.type
      The type of event, such as E_TOUCH or E_TAP, see below.

    e.finger
      0 or 1, whether this is the first or second finger detected

    e.from and e.to
      Points that say from where to where this event happened

    e.duration
      Duration of the event in ms

    e.button and e.gesture
      Pointers to the event or gesture attached to the event

  Here's a list of possible event types and when they're fired:

    E_TOUCH
      A finger comes on the screen. If that is within the area of a
      button, a pointer to that button will be in e.button.

    E_MOVE
      A finger has moved. e.from and e.to contain the coordinates. If
      this swipe started within a button, that button will be in
      e.button for all subsequent E_MOVE and the eventual E_RELEASE
      event, even if the finger is not on the button anymore. This
      allows buttons to be swiped.

    E_RELEASE
      The e.from and e.to will hold the beginning and ending
      positions for this entire swipe, e.duration holds the time
      since the finger was placed on the screen.

    E_GESTURE
      After E_RELEASE, the first gesture that matches a swipe fires
      a E_GESTURE, with a pointer to the gesture in e.gesture. None
      of the further events below are then generated, even if the
      gesture started on a button.

    E_TAP
      Fired when a short tap on the screen is detected. Will come
      with the associated button in e.button if applicable.

    E_DBLTAP
      Like E_TAP but for a quick double tap. Note that no E_TAP will
      be fired in this case.

    E_DRAGGED
      Fired when the finger has left the button before it is lifted.

    E_PRESSED
      Fired after the button is released and it wasn't a E_TAP,
      E_DBLTAP or E_SWIPE.

  When you add a handler function you can also specify what events it should
  receive by supplying it as the second argument after the handler function.
  If you want to register multiple events for the same function, don't
  register the handler twice, but simply add (or bitwise or) the event
  values. The default value there is the pseudo-event E_ALL, which is simply
  a value with all the event bits turned on. You can also subtract event type
  values from E_ALL to exclude them.

  If you add the pseudo-event value E_BTNONLY to the value supplied to
  addHandler, it indicates that you only want the function to receive events
  that have a button attached. (This only makes sense if you register your
  handler with M5.touch, where it could also see event that were not tied to
  a button.)

  Here are some examples of ways to add a handler function:

    button1.addHandler(b1Function, E_TOUCH _ E_RELEASE);
      b1Function only get these two events for button1

    M5.Touch.addHandler(btnHandle, E_ALL + E_BTNONLY - E_MOVE);
      btnHandle gets all events (except E_MOVE) tied to buttons

    swipeUp.addHandler(nextPage);
      Handler nextPage is called when swipeUp gesture detected. Note that
      nextpage must still be in the handler format, accepting the event as an
      argument, even when it can completely ignore it.

  If your event reads date or calls functions in e.button or e.gesture,
  remember that these are pointers. Without going into too much detail, it
  means it must do so with the -> notation, so to read the button x position,
  you would say "e.button->x".

  IMPORTANT: If you registered a function with M5.Touch and do this on an
  event that has no button attached, your program will crash. So make sure
  you only get details about buttons if you know your event has a button
  pointer. You can either test that with "if (e.button) ..." or make sure
  with E_BTNONLY.

  Please have a look at the example sketch (see below) to understand how this
  all works and run the sketch to see all the events printed to the serial
  port.


== Visual Buttons/Labels with Hardware Buttons ==

  If you want to display a button on the screen for your hardware button,
  just set your hardware up as usual, but then follow the parameter list with
  "hw" (in quotes), followed by the parameters of the touch button below. the
  hardware buttons in the older M5Stack devices are ready to display labels
  for the buttons that respond to the state: all you need is supply colors.
  their initialisation (in M5Stack.h in this library) looks like this:

    Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS, "hw", 3, 218, 102, 21, true, "BtnA");
    Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS, "hw", 109, 218, 102, 21, true, "BtnB");
    Button BtnC = Button(BUTTON_C_PIN, true, DEBOUNCE_MS, "hw", 215, 218, 102, 21, true, "BtnC");





*/

#ifndef _M5BUTTON_H_
#define _M5BUTTON_H_

class Gesture;

#include <Arduino.h>
#include <Free_Fonts.h>
#include <M5Display.h>

#include "PointAndZone.h"
#include "utility/Config.h"

#ifdef M5Stack_M5Core2
#include <M5Touch.h>
#endif /* M5Stack_M5Core2 */

#define BUTTON_FREEFONT FSS9
#define BUTTON_TEXTFONT 1
#define BUTTON_TEXTSIZE 1
#define BUTTON_DATUM MC_DATUM

#define TAP_TIME 150
#define DBLTAP_TIME 200
#define LONGPRESS_TIME 0
#define REPEAT_DELAY 0
#define REPEAT_INTERVAL 200

#define GESTURE_MAXTIME 500
#define GESTURE_MINDIST 75
#define ANYWHERE Zone()

#define NUM_EVENTS 11
#define E_TOUCH 0x0001
#define E_RELEASE 0x0002
#define E_MOVE 0x0004
#define E_GESTURE 0x0008
#define E_TAP 0x0010
#define E_DBLTAP 0x0020
#define E_DRAGGED 0x0040
#define E_PRESSED 0x0080
#define E_PRESSING 0x0100
#define E_LONGPRESSED 0x0200
#define E_LONGPRESSING 0x0400

#define E_ALL 0x0FFF

#define NONE 0x0120  // Special color value: transparent

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
  uint16_t direction(bool rot1 = false);
  bool isDirection(int16_t wanted, uint8_t plusminus = PLUSMINUS,
                   bool rot1 = false);
  uint16_t distance();
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
  Button(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ = false,
         const char* name_ = "", ButtonColors off_ = {NONE, NONE, NONE},
         ButtonColors on_ = {NONE, NONE, NONE}, uint8_t datum_ = BUTTON_DATUM,
         int16_t dx_ = 0, int16_t dy_ = 0, uint8_t r_ = 0xFF);
  Button(uint8_t pin_, uint8_t invert_, uint32_t dbTime_, String hw = "hw",
         int16_t x_ = 0, int16_t y_ = 0, int16_t w_ = 0, int16_t h_ = 0,
         bool rot1_ = false, const char* name_ = "",
         ButtonColors off_ = {NONE, NONE, NONE},
         ButtonColors on_ = {NONE, NONE, NONE}, uint8_t datum_ = BUTTON_DATUM,
         int16_t dx_ = 0, int16_t dy_ = 0, uint8_t r_ = 0xFF);
  ~Button();
  operator bool();
  bool operator==(const Button& b);
  bool operator!=(const Button& b);
  bool operator==(Button* b);
  bool operator!=(Button* b);
  int16_t instanceIndex();
  bool read(bool manualRead = true);
  void fingerDown(Point pos = Point(1, 1), uint8_t finger = 0);
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
  uint16_t repeatDelay, repeatInterval;

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
  uint32_t _time, _lastRepeat;
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
  bool _compat;  // For TFT_eSPI_Button emulation
  char _label[51];
  uint8_t _textFont;
  const GFXfont* _freeFont;
  uint8_t _textSize;
};

class Gesture {
 public:
  static std::vector<Gesture*> instances;
  Gesture(Zone fromZone_, Zone toZone_, const char* name_ = "",
          uint16_t minDistance_ = GESTURE_MINDIST,
          int16_t direction_ = INVALID_VALUE, uint8_t plusminus_ = PLUSMINUS,
          bool rot1_ = false, uint16_t maxTime_ = GESTURE_MAXTIME);
  Gesture(const char* name_ = "", uint16_t minDistance_ = GESTURE_MINDIST,
          int16_t direction_ = INVALID_VALUE, uint8_t plusminus_ = PLUSMINUS,
          bool rot1_ = false, uint16_t maxTime_ = GESTURE_MAXTIME);
  ~Gesture();
  operator bool();
  int16_t instanceIndex();
  bool test(Point& from, Point& to, uint16_t duration);
  bool wasDetected();
  void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
  void delHandlers(void (*fn)(Event&) = nullptr);
  char* name();
  Zone fromZone;
  Zone toZone;
  Event event;
  int16_t direction;
  uint8_t plusminus;
  bool rot1;
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
  Event fireEvent(uint8_t finger, uint16_t type, Point& from, Point& to,
                  uint16_t duration, Button* button, Gesture* gesture);
  void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL,
                  Button* button = nullptr, Gesture* gesture = nullptr);
  void delHandlers(void (*fn)(Event&), Button* button, Gesture* gesture);
  Event event;
  std::vector<EventHandler> _eventHandlers;

 private:
  uint8_t _textFont;
  const GFXfont* _freeFont;
  uint8_t _textSize;
  bool _leftovers;

#ifdef _M5TOUCH_H_
 private:
  Finger _finger[2];
#endif
};

class M5Events {
 public:
  M5Events();
  static M5Events* instance;
};

// TFT_eSPI_Button compatibility
class TFT_eSPI_Button : public Button {
 public:
  TFT_eSPI_Button();
  void initButton(TFT_eSPI* gfx, int16_t x, int16_t y, uint16_t w, uint16_t h,
                  uint16_t outline, uint16_t fill, uint16_t textcolor,
                  char* label_, uint8_t textsize_);
  void initButtonUL(TFT_eSPI* gfx, int16_t x_, int16_t y_, uint16_t w_,
                    uint16_t h_, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, char* label_, uint8_t textsize_);
  void setLabelDatum(int16_t x_delta, int16_t y_delta,
                     uint8_t datum = MC_DATUM);
  void drawButton(bool inverted = false, String long_name = "");
  bool contains(int16_t x, int16_t y);
  void press(bool p);
  bool isPressed();
  bool justPressed();
  bool justReleased();
};

#endif /* _M5BUTTON_H_ */
