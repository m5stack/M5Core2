/*
*******************************************************************************
*Copyright (c) 2021 by  M5Stack
*                 Equipped with M5Core2 sample source code
*Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Hello World
* date：2021/7/15
*******************************************************************************
*/
#include <M5Core2.h>

// After M5Core2 is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core2

  M5.Lcd.print("Hello World");  // Print text on the screen (string)
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {

}
