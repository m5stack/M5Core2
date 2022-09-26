/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/laser-rx
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/laser-rx
*
* Product: laser-rx/laser-tx.
* Date: 2021/8/18
*******************************************************************************
  Please connect to Port B,LASER Unit wireless UART application: burn the program to two M5Core2
  And connect LASER.TX and LASER.RX
  Point LASER.TX to LASER.RX and press the button on the panel to send characters to the receiver of LASER.RX.
  请连接端口B,在屏幕上显示字符串。LASER Unit无线UART应用:刻录程序到两个M5Core2 并分别连接 LASER RX 和 LASER TX
  然后按下LASER TX面板上的按钮，将字符发送到LASER.RX接收器。
*/

#include <M5Core2.h>

char ch;
#define RX
void setup() {
  M5.begin();  //Init M5Core2.  初始化M5Core2

  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(9600, SERIAL_8N1, 13, 14);
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);

  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(60, 50);
#ifdef RX
  M5.Lcd.print("LASER RX");
#elif defined TX
  M5.Lcd.print("LASER TX");
#else
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("LASER TX/RX");
  M5.Lcd.setCursor(50, 200);
  M5.Lcd.print('A');
  M5.Lcd.setCursor(150, 200);
  M5.Lcd.print('B');
  M5.Lcd.setCursor(240, 200);
  M5.Lcd.print('C');
#endif
  M5.Lcd.setCursor(0, 100);
}

void loop() {
#ifdef RX
  M5.update();
  if (Serial2.available()) {
    char ch = Serial2.read();
    M5.Lcd.print(ch);
  }

  if (M5.BtnA.wasReleased()) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
  }
#elif defined TX
  Serial2.write('A');
  delay(50);
#else
  if (M5.BtnA.wasReleased()) {
    ch = 'A';
    Serial2.write(ch);
  } else if (M5.BtnB.wasReleased()) {
    ch = 'B';
    Serial2.write(ch);
  } else if (M5.BtnC.wasReleased()) {
    ch = 'C';
    Serial2.write(ch);
  }
  M5.update();
  if (Serial2.available()) {
    char ch = Serial2.read();
    M5.Lcd.print(ch);
  }
#endif
}
