/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: NS4168--I2S power amplifier.  功放示例
* Date: 2023/7/31
*******************************************************************************
*/
#include <M5Core2.h>

uint8_t microphonedata0[1024 * 80];
int data_offset = 0;

void DisplayInit(void) {       // Initialize the display. 显示屏初始化
    M5.Lcd.fillScreen(WHITE);  // Set the screen background color to white.
                               // 设置屏幕背景色为白色
    M5.Lcd.setTextColor(
        BLACK);  // Set the text color to black.  设置文字颜色为黑色
    M5.Lcd.setTextSize(2);  // Set font size to 2.  设置字体大小为2
}

/* After M5Core2 is started or reset
the program in the setUp () function will be run, and this part will only be run
once. 在 M5Core2
启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin(true, true, true, true, kMBusModeOutput,
             true);             // Init M5Core2.  初始化 M5Core2
    M5.Axp.SetSpkEnable(true);  // Enable speaker power.  启用扬声器电源
    DisplayInit();
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(10,
                     10);  // Set the cursor at (10,10).  将光标设在（10，10）处
    M5.Lcd.printf("Recorder!");  // The screen prints the formatted string and
                                 // wraps it.  屏幕打印格式化字符串并换行
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setCursor(10, 26);
    M5.Lcd.printf("Press Left Button to recording!");
    delay(100);  // delay 100ms.  延迟100ms
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
    TouchPoint_t pos =
        M5.Touch.getPressPoint();  // Stores the touch coordinates in pos.
                                   // 将触摸坐标存储在pos.内
    if (pos.y > 240) {
        if (pos.x < 109) {
            M5.Axp.SetVibration(true);  // Open the vibration.   开启震动马达
            delay(100);
            M5.Axp.SetVibration(false);
            data_offset = 0;
            M5.Spk.InitI2SSpeakOrMic(MODE_MIC);
            size_t byte_read;
            while (1) {
                i2s_read(Speak_I2S_NUMBER,
                         (char *)(microphonedata0 + data_offset), DATA_SIZE,
                         &byte_read, (100 / portTICK_RATE_MS));
                data_offset += 1024;
                if (data_offset == 1024 * 80 || M5.Touch.ispressed() != true)
                    break;
            }
            size_t bytes_written;
            M5.Spk.InitI2SSpeakOrMic(MODE_SPK);
            i2s_write(Speak_I2S_NUMBER, microphonedata0, data_offset,
                      &bytes_written, portMAX_DELAY);
        }
    }
}
