/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/iso485
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/iso485
*
* Product: iso485.
* Date: 2021/8/30
*******************************************************************************
  Please connect to PortC,Pressed ButtonA :send "hello world"
  请连接端口C,Pressed ButtonA :send "hello world"
*/

#include <M5Core2.h>

String str = "";

void setup() {
  M5.begin(true, false, true, false);
  M5.Lcd.drawString("ISO485", 20, 0, 2);
  Serial2.begin(115200, SERIAL_8N1, 14, 13);
  M5.Lcd.setCursor(0, 20);
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    Serial2.write("Hello World\r\n");
    Serial.println("1");
  }
  if (Serial2.available()) {
    char ch = Serial2.read();
    str += ch;
    Serial.println("2");

    if (str.endsWith("\r\n")) {
      Serial.print(str);
      M5.Lcd.print(str);
      str = "";
    }
  }
  M5.update();
}
