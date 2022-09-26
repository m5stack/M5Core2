/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/accel
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/accel
*
* Product: Accel.  加速度计
* Date: 2022/7/5
*******************************************************************************
  Read ACCEL Unit three-axis acceleration
  读取ACCEL 单位三轴加速度
  Libraries:
    - [Arduino-ADXL345](https://github.com/jakalada/Arduino-ADXL345)
*/

#include <M5Core2.h>
#include <ADXL345.h>
ADXL345 accel(ADXL345_ALT);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin();
  M5.Lcd.setCursor(140, 10, 4);
  M5.Lcd.println("ACC");

  M5.Lcd.setCursor(40, 100);
  M5.Lcd.print(" x ");
  M5.Lcd.setCursor(140, 100);
  M5.Lcd.print(" y ");
  M5.Lcd.setCursor(240, 100);
  M5.Lcd.print(" z ");

  byte deviceID = accel.readDeviceID();
  if (deviceID != 0) {
    Serial.print("0x");
    Serial.print(deviceID, HEX);
    Serial.println("");
  } else {
    Serial.println("read device id: failed");
    while (1) {
      delay(100);
    }
  }

  // Data Rate
  // - ADXL345_RATE_3200HZ: 3200 Hz
  // - ADXL345_RATE_1600HZ: 1600 Hz
  // - ADXL345_RATE_800HZ:  800 Hz
  // - ADXL345_RATE_400HZ:  400 Hz
  // - ADXL345_RATE_200HZ:  200 Hz
  // - ADXL345_RATE_100HZ:  100 Hz
  // - ADXL345_RATE_50HZ:   50 Hz
  // - ADXL345_RATE_25HZ:   25 Hz
  // - ...
  if (!accel.writeRate(ADXL345_RATE_200HZ)) {
    Serial.println("write rate: failed");
    while (1) {
      delay(100);
    }
  }

  // Data Range
  // - ADXL345_RANGE_2G: +-2 g
  // - ADXL345_RANGE_4G: +-4 g
  // - ADXL345_RANGE_8G: +-8 g
  // - ADXL345_RANGE_16G: +-16 g
  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("write range: failed");
    while (1) {
      delay(100);
    }
  }

  if (!accel.start()) {
    Serial.println("start: failed");
    while (1) {
      delay(100);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (accel.update()) {
    M5.Lcd.fillRect(0, 130, 360, 30, BLACK);
    M5.Lcd.setCursor(35, 130);
    M5.Lcd.print((int)(1000 * accel.getX()));
    M5.Lcd.setCursor(135, 130);
    M5.Lcd.print((int)(1000 * accel.getY()));
    M5.Lcd.setCursor(235, 130);
    M5.Lcd.print((int)(1000 * accel.getZ()));
    //M5.Lcd.setCursor(300, 130); M5.Lcd.print("mg");
  } else {
    Serial.println("update failed");
    while (1) {
      delay(100);
    }
  }
  delay(100);
}
