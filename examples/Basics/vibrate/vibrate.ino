/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: vibrate example.  震动电机示例
* Date: 2021/9/18
*******************************************************************************
*/
#include <M5Core2.h>

/* After M5Core2 is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 M5Core2 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();  //Init M5Core2.  初始化 M5Core2
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.Axp.SetLDOEnable(3, true);  //Open the vibration.   开启震动马达
  delay(1000);
  M5.Axp.SetLDOEnable(3, false);  //Open the vibration.   关闭震动马达
  delay(1000);
}
