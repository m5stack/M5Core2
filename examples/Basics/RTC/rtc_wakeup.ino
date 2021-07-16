/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：RTC Time management example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Core2.h>

RTC_TimeTypeDef RTCtime;
RTC_TimeTypeDef RTCtime_Now;

char timeStrbuff[64];

// After M5Core2 is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core2

  RTCtime.Hours = 10; // Set the time
  RTCtime.Minutes = 30;
  RTCtime.Seconds = 45;
  M5.Lcd.setCursor(0,80); // Move the cursor position to (x,y)
  M5.Lcd.println("BtnA: shutdown, use power button to turn back on"); // The screen prints the formatted string and wraps it
  M5.Lcd.println("BtnB: shutdown, wake up after 5 seconds");
  M5.Lcd.printf("BtnC: shutdown, wake up at RTC Time %d:%d:%d",RTCtime.Hours,RTCtime.Minutes, RTCtime.Seconds);
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop(){
  M5.update();  // Read the status of keys A, B, and C

  if(M5.BtnA.wasPressed()){ // Constantly check the status of keys A, B, and C, if A press....
    M5.shutdown();  // Turn off the power
  }else if(M5.BtnB.wasPressed()){
    M5.shutdown(5); // Turn off the power and wake up again after 5 seconds
  }else if(M5.BtnC.wasPressed()){
    M5.shutdown(RTCtime); // Turn off the power and wake up at the specified time
  }

  M5.Lcd.setCursor(0,140);
  M5.Rtc.GetTime(&RTCtime_Now); // Gets the current time
  sprintf(timeStrbuff,"RTC Time Now is %02d:%02d:%02d",// Stores real-time time data to timeStrbuff
         RTCtime_Now.Hours,RTCtime_Now.Minutes,RTCtime_Now.Seconds);  
  M5.Lcd.println(timeStrbuff);  // Screen printing output timeStrbuff

}
