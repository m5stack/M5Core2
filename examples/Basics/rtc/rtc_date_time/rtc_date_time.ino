/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/core2
*
* describe：RTC--实时时钟示例
* date：2021/7/21
*******************************************************************************
*/
#include <M5Core2.h>

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

char timeStrbuff[64];

void flushTime(){
    M5.Rtc.GetTime(&RTCtime); //Gets the time in the real-time clock.  获取实时时钟内的时间
    M5.Rtc.GetDate(&RTCDate);
    sprintf(timeStrbuff,"%d/%02d/%02d %02d:%02d:%02d",  //Stores real-time time and date data to timeStrbuff.   将实时时间、日期数据存储至timeStrbuff
                        RTCDate.Year,RTCDate.Month,RTCDate.Date,
                        RTCtime.Hours,RTCtime.Minutes,RTCtime.Seconds);
    M5.lcd.setCursor(10,100); //Move the cursor position to (x,y).  移动光标位置到(x,y)处
    M5.Lcd.println(timeStrbuff);  //Output the contents of.  输出timeStrbuff中的内容
}

void setupTime(){

  RTCtime.Hours = 23; //Set the time.  设置时间
  RTCtime.Minutes = 33;
  RTCtime.Seconds = 33;
  M5.Rtc.SetTime(&RTCtime); //and writes the set time to the real time clock.  并将设置的时间写入实时时钟

  RTCDate.Year = 2020;  //Set the date.  设置日期
  RTCDate.Month = 11;
  RTCDate.Date = 6;
  M5.Rtc.SetDate(&RTCDate);
}
/* After M5Core2 is started or reset
the program in the setUp () function will be run, and this part will only be run once.
在 M5Core2 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {

  M5.begin(); //Init M5Core2.  初始化 M5Core2
  setupTime();
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  flushTime();
  delay(1000);
}