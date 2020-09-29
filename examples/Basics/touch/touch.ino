#include <M5Core2.h>

// Set up the zones for the gestures. The true at the end sets the rot1
// flag, which causes these zones to stay in same place on physical screen
// regardless of rotation.
Zone topHalf(0, 0, 320, 100, true);
Zone bottomHalf(0, 140, 320, 140, true);
Zone leftHalf(0, 0, 120, 280, true);
Zone rightHalf(240, 0, 120, 280, true);

// Defines the gestures themselves
Gesture swipeRight(leftHalf, rightHalf, "swipe right");
Gesture swipeDown(topHalf, bottomHalf, "swipe down");
Gesture swipeLeft(rightHalf, leftHalf, "swipe left");
Gesture swipeUp(bottomHalf, topHalf, "swipe up");

// Defines the buttons. Colors in format {bg, text, outline}
ButtonColors on = {RED, WHITE, WHITE};
ButtonColors off = {BLACK, WHITE, WHITE};
Button tl(0, 0, 0, 0, false ,"top-left", off, on, TL_DATUM);
Button bl(0, 0, 0, 0, false, "bottom-left", off, on, BL_DATUM);
Button tr(0, 0, 0, 0, false, "top-right", off, on, TR_DATUM);
Button br(0, 0, 0, 0, false, "bottom-right", off, on, BR_DATUM);

void setup() {
  M5.begin();
  M5.Events.addHandler(rotate, E_GESTURE);  
  M5.Events.addHandler(toggleColor, E_DBLTAP + E_BTNONLY);
  M5.Events.addHandler(eventDisplay);
  br.addHandler(tappedBottomRight, E_TAP);
  doButtons();
}

void loop() {
  M5.update();
}

// Positions the buttons and draws them. (Only because height and width
// change. If we were only switching between normal and upside-down we
// would only need the Buttons.draw() here.)
void doButtons() {
  int16_t hw = M5.Lcd.width() / 2;
  int16_t hh = M5.Lcd.height() / 2;
  tl.set(0, 0, hw - 5, hh - 5);
  bl.set(0, hh + 5, hw - 5, hh - 5);
  tr.set(hw + 5, 0, hw - 5, hh - 5);
  br.set(hw + 5, hh + 5, hw - 5, hh - 5);
  M5.Buttons.draw();
}

void rotate(Event& e) {
  // Gestures and Buttons have an instanceIndex() that starts at zero
  // so by defining the gestures in the right order I can use that as
  // the input for M5.Lcd.setRotation.
  uint8_t new_rotation = e.gesture->instanceIndex();
  if (new_rotation != M5.Lcd.rotation) {
    M5.Lcd.setRotation(new_rotation);
  	M5.Lcd.clearDisplay();
    doButtons();
  }
}

void toggleColor(Event& e) {
  // This just creates shorthand "b" for the button in the event, so we
  // can type "b." instead of "e.button->"
  Button& b = *e.button;
  
  // Toggles the background between black and blue
  if (b.off.bg == BLACK) b.off.bg = BLUE; else b.off.bg = BLACK;
  
  b.draw();
}

void tappedBottomRight(Event& e) {
  Serial.println("You tapped the bottom right!");
}
  
void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d)", e.typeName(), e.finger, e.objName(), e.from.x, e.from.y);
  if (e.type != E_TOUCH && e.type != E_TAP && e.type != E_DBLTAP) {
    Serial.printf(" --> (%3d, %3d)  %5d ms", e.to.x, e.to.y, e.duration);
  }
  Serial.println();
}
