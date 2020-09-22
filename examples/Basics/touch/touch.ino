#include <M5Core2.h>

TouchZone topHalf(0,0,320,120);
TouchZone bottomHalf(0,120,320,160);
Gesture swipeDown(topHalf, bottomHalf, "Swipe Down");

TouchButton lt = TouchButton(0, 0, 160, 120, "left-top");
TouchButton lb = TouchButton(0, 120, 160, 120, "left-bottom");
TouchButton rt = TouchButton(160, 0, 160, 120, "right-top");
TouchButton rb = TouchButton(160, 120, 160, 120, "right-bottom");

void setup() {
  M5.begin();
  M5.Touch.addHandler(eventDisplay);
  M5.Touch.addHandler(colorButtons, TE_BTNONLY + TE_TOUCH + TE_RELEASE);
  swipeDown.addHandler(yayWeSwiped);
  rt.addHandler(dblTapped, TE_DBLTAP);
}

void loop() {
  M5.update();
}

void eventDisplay(TouchEvent& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d)", M5.Touch.eventTypeName(e), e.finger, M5.Touch.eventObjName(e),  e.from.x, e.from.y);
  if (e.type != TE_TOUCH && e.type != TE_TAP && e.type != TE_DBLTAP) {
    Serial.printf("--> (%3d, %3d)  %5d ms", e.to.x, e.to.y, e.duration);
  }
  Serial.println();
}

void colorButtons(TouchEvent& e) {
  TouchButton& b = *e.button;
  M5.Lcd.fillRect(b.x, b.y, b.w, b.h, b.isPressed() ? WHITE : BLACK);
}

void yayWeSwiped(TouchEvent& e) {
  Serial.println("--- SWIPE DOWN DETECTED ---");
}

void dblTapped(TouchEvent& e) {
  Serial.println("--- TOP RIGHT BUTTON WAS DOUBLETAPPED ---");
}