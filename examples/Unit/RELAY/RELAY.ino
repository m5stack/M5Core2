/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/core2
*
* describe：Relay.  继电器
* date：2021/8/16
*******************************************************************************
  Please connect to Port B,Use RELAY to switch on and off the circuit.
  请连接端口B,使用继电器开关电路。
*/

#include <M5Core2.h>

void setup() {
  M5.begin(); //Init M5Core2.  初始化 M5Core2
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.setCursor(50, 0);
  M5.Lcd.println(("Relay Example"));
  dacWrite(25, 0);  //disable the speak noise
  pinMode(26, OUTPUT);
}

void loop(void) {
  M5.Lcd.setCursor(100, 40);
  M5.Lcd.print("ON");
  digitalWrite(26, HIGH);
  delay(1000);
  M5.Lcd.fillRect(100,40,60,50,BLACK);
  M5.Lcd.print("OFF");
  digitalWrite(26, LOW);
  delay(1000);
  M5.Lcd.fillRect(100,40,60,50,BLACK);
}
