/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Sleep-Axp example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Core2.h>

// After M5Core2 is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core2

  M5.Lcd.setTextFont(2);  // Set font size to 2

  Serial.println("Light / Deep Sleep Test."); // The serial port prints the formatted string with a newline
  M5.Lcd.println("Light / Deep Sleep Test."); // The screen prints the formatted string and wraps it

  Serial.println("Going to light sleep for 5 seconds.");
  M5.Lcd.println("Going to light sleep for 5 seconds.");
  delay(2500);  // delay 2500ms

  M5.Axp.LightSleep(SLEEP_SEC(5));  // Restart after 10 seconds of light sleep and continue from the next line

  Serial.println("Wakeup from light sleep.");
  M5.Lcd.println("Wakeup from light sleep.");
  delay(1000);

  Serial.println("Going to deep sleep for 5 seconds.");
  M5.Lcd.println("Going to deep sleep for 5 seconds.");
  delay(2500);

  M5.Axp.DeepSleep(SLEEP_SEC(5)); // Wake up after 5 seconds of light sleep, the CPU will reboot and the program will start from the beginning
}

void loop()
{

}
