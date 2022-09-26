/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: Sleep--Axp电源休眠
* Date: 2022/3/12
*******************************************************************************
*/
#include <M5Core2.h>

/* After M5Core2 is started or reset
the program in the setUp () function will be run, and this part will only be run once.
在 M5Core2 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();  //Init M5Core2.  初始化 M5Core2

  M5.Lcd.setTextFont(2);  //Set font size to 2.  设置字体大小为2

  Serial.println(
      "Light / Deep Sleep Test.");  //The serial port prints the formatted string with a newline.  串口输出格式化字符串并换行
  M5.Lcd.println(
      "Light / Deep Sleep Test.");  //The screen prints the formatted string and wraps it.  屏幕打印格式化字符串并换行

  Serial.println("Going to light sleep for 5 seconds.");
  M5.Lcd.println("Going to light sleep for 5 seconds.");
  delay(2500);  //delay 2500ms.  延迟2500ms

  M5.Axp.LightSleep(SLEEP_SEC(
      5));  //Wake up after 5 seconds of light sleep, the CPU will reboot and the program will start from the beginning.  轻度睡眠10秒后重新启动，程序从下一行继续执行

  Serial.println("Wakeup from light sleep.");
  M5.Lcd.println("Wakeup from light sleep.");
  delay(1000);

  Serial.println("Going to deep sleep for 5 seconds.");
  M5.Lcd.println("Going to deep sleep for 5 seconds.");
  delay(2500);

  M5.Axp.DeepSleep(SLEEP_SEC(
      5));  //Wake up after 5 seconds of deep sleep, the CPU will reboot and the program will start from the beginning.  深度睡眠5秒后唤醒,CPU将重新启动，程序将从头开始执行
}

void loop() {}
