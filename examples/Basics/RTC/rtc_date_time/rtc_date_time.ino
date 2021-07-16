/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：RTC--Real time clock example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Core2.h>

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

char timeStrbuff[64];

void flushTime(){
    M5.Rtc.GetTime(&RTCtime); // Gets the time in the real-time clock
    M5.Rtc.GetDate(&RTCDate);
    
    sprintf(timeStrbuff,"%d/%02d/%02d %02d:%02d:%02d",  // Stores real-time time and date data to timeStrbuff
                        RTCDate.Year,RTCDate.Month,RTCDate.Date,
                        RTCtime.Hours,RTCtime.Minutes,RTCtime.Seconds);
                                         
    M5.lcd.setCursor(10,100); // Move the cursor position to (x,y)
    M5.Lcd.println(timeStrbuff);  // Output the contents of timeStrbuff
}

void setupTime(){
  
  RTCtime.Hours = 23; // Set the time
  RTCtime.Minutes = 33;
  RTCtime.Seconds = 33;
  M5.Rtc.SetTime(&RTCtime); // and writes the set time to the real time clock
  
  RTCDate.Year = 2020;  // Set the date
  RTCDate.Month = 11;
  RTCDate.Date = 6;
  M5.Rtc.SetDate(&RTCDate);
}
// After M5Core2 is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup() {

  M5.begin(); //Init M5Core2
  setupTime();
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  flushTime();
  delay(1000);  //delay 1000ms
}