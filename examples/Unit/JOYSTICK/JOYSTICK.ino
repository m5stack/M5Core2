/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/joystick
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/joystick
*
* Product: JOYSTICK.
* Date: 2021/8/30
*******************************************************************************
  Please connect to PortA,Read JOYSTICK Unit X, Y axis offset data and button status
  请连接端口A,读取操纵杆单位X, Y轴偏移数据和按钮状态
*/

#include <M5Core2.h>

#define JOY_ADDR 0x52  //define Joystick I2C address.  定义摇杆的I2C地址

void setup() {
  M5.begin();
  M5.Lcd.setCursor(70, 0, 4);
  M5.Lcd.println(("Joystick Test"));
  dacWrite(25, 0);  //disable the speak noise.  禁用语音噪音
  Wire.begin(32, 33, 400000UL);
}

char data[100];
void loop() {
  static uint8_t x_data, y_data, button_data;
  M5.Lcd.setCursor(100, 50, 4);
  M5.Lcd.printf("X:%d      ", x_data);
  M5.Lcd.setCursor(100, 80, 4);
  M5.Lcd.printf("Y:%d      ", y_data);
  M5.Lcd.setCursor(100, 110, 4);
  M5.Lcd.printf("B:%d      ", button_data);
  Wire.requestFrom(
      JOY_ADDR,
      3);  //Request 3 bytes from the slave device.  向从设备请求3个字节
  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
    sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
    Serial.print(data);
  }
  delay(200);
}
