/*

== The PointAndZone Library ==

This library was written to supply Point and Zone, common primitives for
M5Display and M5Button, libraries originally written for the M5Stack series
of devices. They could be useful for many other applications, especially
anything based on a TFT_eSPI display driver.


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
  in a zone. There is also a 'distanceTo' method to see how far two points
  are apart and 'directionTo' will tell you compass direction from one point
  to another. See the PointAndZone.h file for more detailed information.

  The 'directionTo' method allows you to see what the compass course from one
  point to another would be. So if point A were (0, 0) and B were (10, 10),
  "A.directionTo(B)" would return 45. With two extra arguments you can test
  whether a course matches within a certain range. For instance
  "A.directionTo(B, 0, 30)" would return 'false' because 45 is not within 30
  degrees of 0 (due north). This is useful for directions of gestures, for
  example.

  Here are some examples to make things clear:

    Point a;
    Point b(50, 120);
    Serial.println(a.valid());          // 0
    Serial.println(a);                  // (invalid)
    a.set(10, 30);
    Serial.println(a);					// (10,30)
    Serial.println(a.valid());          // 1
    Serial.println(b.y);                // 120
    Serial.println(a.distanceTo(b));    // 98
    Zone z(0,0,100, 100);
    Serial.println(z.w);                // 100
    Serial.println(z.contains(a));      // 1
    Serial.println(b.in(z));            // 0


== Rotation and the 'rot1' Property ==

*/

#ifndef _POINTANDZONE_H_
#define _POINTANDZONE_H_

#include <Arduino.h>
#include <M5Display.h>  // so that we can get the rotation

#include "utility/Config.h"  // Defines 'TFT', a pointer to the display

#define INVALID_VALUE -32768
#define PLUSMINUS 45  // default value for isDirectionTo

class Zone;

class Point {
 public:
  Point(int16_t x_ = INVALID_VALUE, int16_t y_ = INVALID_VALUE);
  bool operator==(const Point& p);
  bool operator!=(const Point& p);
  explicit operator bool();
  operator char*();
  void set(int16_t x_ = -1, int16_t y_ = -1);
  bool valid();
  bool in(Zone& z);
  bool Equals(const Point& p);
  uint16_t distanceTo(const Point& p);
  uint16_t directionTo(const Point& p, bool rot1 = false);
  bool isDirectionTo(const Point& p, int16_t wanted,
                     uint8_t plusminus = PLUSMINUS, bool rot1 = false);
  void rotate(uint8_t m);
  int16_t x, y;

 private:
  char _text[12];
};

class Zone {
 public:
  Zone(int16_t x_ = INVALID_VALUE, int16_t y_ = INVALID_VALUE, int16_t w_ = 0,
       int16_t h_ = 0, bool rot1_ = false);
  explicit operator bool();
  bool valid();
  void set(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ = false);
  bool contains(const Point& p);
  bool contains(int16_t x, int16_t y);
  void rotate(uint8_t m);
  int16_t x, y, w, h;
  bool rot1;
};

#endif /* _POINTANDZONE_H_ */
