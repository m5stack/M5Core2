// Copyright (c) M5Core2. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2017, M5Core2
 * \class M5Core2
 * \brief   M5Core2 library.
 * @file    M5Core2.h
 * @author  M5Core2
 * @version V0.0.1 Beta
 * @date    2020/08/12
 * @brief   Header for M5Core2.cpp module
 *
 * \par Description
 * This file is a drive for M5Core2.
 *
 * \par Method List:
 *
 *  System:
        M5.begin();
    LCD:
        M5.lcd.setBrightness(uint8_t brightness);
        M5.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        M5.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        M5.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        M5.Lcd.fillScreen(uint16_t color);
        M5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        M5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        M5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        M5.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        M5.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        M5.Lcd.setCursor(uint16_t x0, uint16_t y0);
        M5.Lcd.setTextColor(uint16_t color);
        M5.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        M5.Lcd.setTextSize(uint8_t size);
        M5.Lcd.setTextWrap(boolean w);
        M5.Lcd.printf();
        M5.Lcd.print();
        M5.Lcd.println();
        M5.Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        M5.Lcd.drawRightString(const char *string, int dX, int poY, int font);
        M5.Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        M5.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        M5.Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Hades2001         2020/08/12         0.0.1          Rebuild the new.
 * </pre>
 *
 */
// #define ESP32

#ifndef _M5Core2_H_
  #define _M5Core2_H_
  
  #if defined(ESP32)

    #include <Arduino.h>
    #include <Wire.h>
    #include <SPI.h>
    #include "FS.h"
    #include "SD.h"

    #include "M5Display.h"
    #include "utility/Config.h"
    #include "utility/CommUtil.h"
    #include "utility/MPU6886.h"
    #include "touch.h"
    #include "AXP192.h"
    #include "RTC.h"


    class M5Core2
    {
      public:
        M5Core2();
        void begin(bool LCDEnable = true, bool SDEnable = true, bool SerialEnable = true, bool I2CEnable = false);
        void update();


        void shutdown();
        int shutdown( int seconds );
        int shutdown( const RTC_TimeTypeDef &RTC_TimeStruct);
        int shutdown( const RTC_DateTypeDef &RTC_DateStruct, const RTC_TimeTypeDef &RTC_TimeStruct);

        // LCD
        M5Display Lcd = M5Display();

        //Power
        AXP192 Axp;

        touch Touch;
        TouchButton BtnA = TouchButton(10,241,110,40);
        TouchButton BtnB = TouchButton(130,241,70,40);
        TouchButton BtnC = TouchButton(230,241,80,40);

        MPU6886 IMU = MPU6886();

        // I2C
        CommUtil I2C = CommUtil();
        
        RTC  Rtc;
        /**
        * Function has been move to Power class.(for compatibility)
        * This name will be removed in a future release.
        */
        void setPowerBoostKeepOn(bool en) __attribute__((deprecated));
        void setWakeupButton(uint8_t button) __attribute__((deprecated));
        void powerOFF() __attribute__((deprecated));
        
      private:
          bool isInited;
    };
    
    extern M5Core2 M5;
    #define m5 M5
    #define lcd Lcd
  #else
    #error “This library only supports boards with ESP32 processor.”
  #endif
#endif
