/*
    Description: Read the characters entered by CardKB Unit and display them on the screen.
*/
#include <Wire.h>
#include <M5Core2.h>

#define CARDKB_ADDR 0x5f

void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(1, 10);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("IIC Address: 0x5F\n");
  M5.Lcd.printf(">>");
}
void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1);
  while(Wire.available())
  {
    char c = Wire.read(); // receive a byte as characterif
    if (c != 0)
    {
      M5.Lcd.printf("%c", c);
      Serial.println(c, HEX);
     // M5.Speaker.beep();
    }
  }
   delay(10);
}
