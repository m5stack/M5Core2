/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: SPIFFS Delete
* Date: 2021/8/4
******************************************************************************
*/

#include <M5Core2.h>
#include <SPIFFS.h>

String file_name =
    "/M5Stack/notes.txt";  //Sets the location and name of the file to be operated on.  设置被操作的文件位置和名称
void setup() {
  M5.begin();             //Init M5Core2.  初始化 M5Core2
  M5.Lcd.setTextSize(2);  //Set the font size to 2.  设置字号大小为2
  if (SPIFFS
          .begin()) {  // Start SPIFFS, return 1 on success.  启动闪存文件系统,若成功返回1
    M5.Lcd.println(
        "\nSPIFFS Started.");  //Screen prints format String.  屏幕打印格式化字符串
  } else {
    M5.Lcd.println("SPIFFS Failed to Start.");
  }

  if (SPIFFS.remove(
          file_name)) {  //Delete file_name file from flash, return 1 on success.  从闪存中删除file_name文件,如果成功返回1
    M5.Lcd.print(file_name);
    M5.Lcd.println(" Remove sucess");
  } else {
    M5.Lcd.print(file_name);
    M5.Lcd.println(" Remove fail");
  }
}

void loop() {}