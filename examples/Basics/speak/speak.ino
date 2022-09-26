/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: Speaker example.  喇叭示例
* Date: 2022/7/26
*******************************************************************************
*/
#include <M5Core2.h>

void DisplayInit(void) {     // Initialize the display. 显示屏初始化
  M5.Lcd.fillScreen(WHITE);  // Set the screen background color to white.
                             // 设置屏幕背景色为白色
  M5.Lcd.setTextColor(
      BLACK);  // Set the text color to black.  设置文字颜色为黑色
  M5.Lcd.setTextSize(2);  // Set font size to 2.  设置字体大小为2
}

//在 M5Core
//启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。
void setup() {
  M5.begin(true, true, true, true);  // Init M5Core2.  初始化 M5Core2
  DisplayInit();
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(10,
                   10);  // Set the cursor at (10,10).  将光标设在（10，10）处
  M5.Lcd.printf("Speak Test!");  // The screen prints the formatted string and
      // wraps it.  屏幕打印格式化字符串并换行
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(10, 26);
  M5.Lcd.printf("Press Left Button to listen DingDong!");
  M5.Spk.begin();     // Initialize the speaker.  初始化扬声器
  M5.Spk.DingDong();  // Play the DingDong sound.  播放 DingDong 声音
  delay(100);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  TouchPoint_t pos =
      M5.Touch.getPressPoint();  // Stores the touch coordinates in pos.
                                 // 将触摸坐标存储在pos.内
  if (pos.y > 240)
    if (pos.x < 109) {
      M5.Axp.SetLDOEnable(3, true);  // Open the vibration.   开启震动马达
      delay(100);
      M5.Axp.SetLDOEnable(3, false);
      delay(100);
      M5.Spk.DingDong();  // Play the DingDong sound.  播放 DingDong 声音
    }
  delay(10);
}
