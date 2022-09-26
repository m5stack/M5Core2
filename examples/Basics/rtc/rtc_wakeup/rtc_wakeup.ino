/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: RTC--时间管理示例
* Date: 2021/7/21
*******************************************************************************
*/
#include <M5Core2.h>

RTC_TimeTypeDef RTCtime;
RTC_TimeTypeDef RTCtime_Now;

char timeStrbuff[64];

/* After M5Core2 is started or reset
the program in the setUp () function will be run, and this part will only be run once.
在 M5Core2 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();  //Init M5Core2.  初始化 M5Stack

  RTCtime.Hours = 12;  // Set the time.  设置时间
  RTCtime.Minutes = 31;
  RTCtime.Seconds = 45;
  RTCtime_Now.Hours = 12;
  RTCtime_Now.Minutes = 30;
  RTCtime_Now.Seconds = 45;

  M5.Rtc.SetTime(&RTCtime);
  M5.Rtc.SetTime(&RTCtime_Now);

  //The screen prints the formatted string and wraps it.
  // 屏幕打印格式化字符串并换行
  M5.Lcd.println("BtnA: shutdown, use power button to turn back on");
  M5.Lcd.println("BtnB: shutdown, wake up after 5 seconds");
  M5.Lcd.printf("BtnC: shutdown, wake up at RTC Time %d:%d:%d", RTCtime.Hours,
                RTCtime.Minutes, RTCtime.Seconds);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.update();  //Read the status of keys A, B, and C.  读取按键 A, B, C 的状态

  if (M5.BtnA
          .wasPressed()) {  //Constantly check the status of keys A, B, and C, if A press.....  不断检测按键A、B、C的状态，如果A按下....
    M5.shutdown();          //Turn off the power.  关闭电源
  } else if (M5.BtnB.wasPressed()) {
    M5.shutdown(
        5);  //Turn off the power and wake up again after 5 seconds.  关闭电源,5秒后再次唤醒
  } else if (M5.BtnC.wasPressed()) {
    M5.shutdown(
        RTCtime);  //Turn off the power and wake up at the specified time.  关闭电源,在指定时间唤醒
  }

  M5.Lcd.setCursor(0, 140);
  M5.Rtc.GetTime(&RTCtime_Now);  //Gets the current time.  获取当前时间
  sprintf(
      timeStrbuff,
      "RTC Time Now is %02d:%02d:%02d",  //Stores real-time time data to timeStrbuff.  将实时时间数据存储至timeStrbuff
      RTCtime_Now.Hours, RTCtime_Now.Minutes, RTCtime_Now.Seconds);
  M5.Lcd.println(
      timeStrbuff);  //Screen printing output timeStrbuff.  输出timeStrbuff
}
