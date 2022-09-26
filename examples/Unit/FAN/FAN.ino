/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/unit_fan
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/unit_fan
*
* Product: Fan.  风扇
* Date: 2021/8/16
*******************************************************************************
  Please connect to Port B, Adjust the speed of FAN Unit through PWM.
  请连接端口B,通过PWM调节风扇单元的转速。
*/
#include <M5Core2.h>

#define motor_pin 26

int freq = 10000;
int ledChannel = 0;
int resolution = 10;
void setup() {
  M5.begin();             //Init M5Core2.  初始化M5Core2
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.setCursor(80, 10);  //Set the cursor at (80,10).  将光标设置在(80,10)处
  M5.Lcd.println("Fan");
  ledcSetup(
      ledChannel, freq,
      resolution);  //Sets the frequency and number of counts corresponding to the channel.  设置通道对应的频率和计数位数
  ledcAttachPin(
      motor_pin,
      ledChannel);  //Binds the specified channel to the specified I/O port for output.  将指定通道绑定到指定 IO 口上以实现输出
}

void loop() {
  ledcWrite(ledChannel, 1024);  //Output PWM.  输出PWM
  delay(1000);
  ledcWrite(ledChannel, 0);
  delay(1000);
}
