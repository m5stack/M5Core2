/*
*******************************************************************************
*Copyright (c) 2021 by M5Stack
*                          配套  M5Stack 示例源代码
*获取更多资料请访问：https://docs.m5stack.com/zh_CN/api/m5paper/system_api
*
* 描  述：LCD--绘制示例
* 版本号：v1.0
*******************************************************************************
*/
#include <M5Core2.h>

//在 M5Stack 启动或者复位重启后， setup部分的程序只会运行一次
void setup() {
  M5.begin(); //初始化 M5Stack
  M5.Lcd.fillScreen(WHITE); //设置屏幕背景色为白色
}

void loop() {
  M5.update();  //读取按键 A, B, C 的状态
  Event& e = M5.Buttons.event;  
  if (e & (E_MOVE | E_RELEASE)) circle((e & E_MOVE) ? e.from : e.to, WHITE);
  if (e & (E_TOUCH | E_MOVE)) circle(e.to, (e.finger) ? BLUE : RED);
}

void circle(Point point, uint16_t color) {
  M5.Lcd.drawCircle(point.x, point.y, 50, color);   //在手指触摸的地方作一个R=50
  M5.Lcd.drawCircle(point.x, point.y, 52, color);   //颜色为color的圆
}
