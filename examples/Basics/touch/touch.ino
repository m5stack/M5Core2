#include <M5Core2.h>

void setup() {
  M5.begin();
}

void loop() {
  M5.update();
  if (M5.Touch.hasChanged()) {
    if (!M5.Touch.points) Serial.print("--");
    if (M5.Touch.points) Serial.printf("x: %d, y: %d        ",  M5.Touch.point[0].x, M5.Touch.point[0].y);
    if (M5.Touch.points == 2) Serial.printf("x: %d, y: %d",  M5.Touch.point[1].x, M5.Touch.point[1].y);
    Serial.println();
  }
}

void btnHandler(TouchButton &btn) {
  M5.Lcd.fillRect(btn.x0, btn.y0, btn.x1 - btn.x0, btn.y1 - btn.y0, btn.isPressed() ? WHITE : BLACK);
}

TouchButton lt = TouchButton(0, 0, 159, 119, btnHandler, btnHandler);
TouchButton lb = TouchButton(0, 120, 159, 240, btnHandler, btnHandler);
TouchButton rt = TouchButton(160, 0, 320, 119, btnHandler, btnHandler);
TouchButton rb = TouchButton(160, 120, 320, 240, btnHandler, btnHandler);