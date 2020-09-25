#include <M5Core2.h>

TouchZone topHalf(0,0,320,120);
TouchZone bottomHalf(0,120,320,160);
Gesture swipeDown(topHalf, bottomHalf, "Swipe Down");

ButtonColors on = {RED, WHITE, WHITE};
ButtonColors off = {BLACK, WHITE, WHITE};
TouchButton tl(0, 0, 155, 115, "top-left", off, on, TL_DATUM);
TouchButton bl(0, 125, 155, 115, "bottom-left", off, on, BL_DATUM);
TouchButton tr(165, 0, 155, 115, "top-right", off, on, TR_DATUM);
TouchButton br(165, 125, 155, 115, "bottom-right", off, on, BR_DATUM);

void setup() {
  M5.begin();
  M5.Touch.addHandler(eventDisplay);
  M5.Touch.addHandler(dblTapped, TE_DBLTAP + TE_BTNONLY);
  swipeDown.addHandler(yayWeSwiped);
  M5.Touch.setFont(FSS12);
  M5.Touch.drawButtons();
}

void loop() {
  M5.update();
}

void eventDisplay(TouchEvent& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d)", e.typeName(), e.finger, e.objName(), e.from.x, e.from.y);
  if (e.type != TE_TOUCH && e.type != TE_TAP && e.type != TE_DBLTAP) {
    Serial.printf(" --> (%3d, %3d)  %5d ms", e.to.x, e.to.y, e.duration);
  }
  Serial.println();
}

void yayWeSwiped(TouchEvent& e) {
  Serial.println("--- SWIPE DOWN DETECTED ---");
}

void dblTapped(TouchEvent& e) {
  // This creates shorthand "b" for the button in the event, so "e.button->" becomes "b."
  // (Warning: only if you're SURE there's a button with the event, otherwise will crash)
  TouchButton& b = *e.button;
  
  // Toggles the background between black and blue
  if (b.off.bg == BLACK) b.off.bg = BLUE; else b.off.bg = BLACK;
  b.draw();
}
