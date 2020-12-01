#include <M5Core2.h>

#define I2C_ADDR_RTC (0x51)

void WriteNByte(uint8_t addr, uint8_t reg, uint8_t num, uint8_t *data)
{
  Wire1.beginTransmission(addr);
  Wire1.write(reg);
  Wire1.write(data, num);
  Wire1.endTransmission();
}

uint8_t ReadNByte(uint8_t addr, uint8_t reg, uint8_t num, uint8_t *data)
{
  uint8_t i = 0;

  Wire1.beginTransmission(addr);
  Wire1.write(reg);
  Wire1.endTransmission(false);
  Wire1.requestFrom((int) addr, (int) num);
  while(Wire1.available())
  {
    data[i++] = Wire1.read();
  }
  return i;
}

void setup()
{
  uint8_t data = 0;

  // Check poweron reason before calling M5.begin()
  //  which calls Rtc.begin() which clears the timer flag.
  Wire1.begin(21, 22);
  ReadNByte(I2C_ADDR_RTC, 0x01, 1, &data);

  M5.begin();

  M5.Lcd.setTextFont(4);
  M5.Lcd.println("==== RTC Power On Test ====");
  M5.Lcd.println("");
  // Check timer flag
  if((data & 0b00000100) == 0b00000100)
  {
    M5.Lcd.println("Power on by: RTC timer");
  }
  else
  {
    M5.Lcd.println("Power on by: power button");
  }
  M5.Lcd.println("");
  M5.Lcd.println("BtnA:  shutdown, auto power on by timer in 5 seconds");
  M5.Lcd.println("");
  M5.Lcd.println("BtnC:  shutdown, use power button to turn back on");

  // Disable timer, set timer clock source to 1/16 Hz for power saving (see datasheet)
  data = 0b00000011;
  WriteNByte(I2C_ADDR_RTC, 0x0E, 1, &data);
}

void loop()
{
  uint8_t data;

  M5.update();

  if(M5.BtnA.wasPressed() == true)
  {
    // Enable timer interrupt
    data = 0b00000001;
    WriteNByte(I2C_ADDR_RTC, 0x01, 1, &data);
    // Enable timer, set timer clock source to 1 Hz
    data = 0b10000010;
    WriteNByte(I2C_ADDR_RTC, 0x0E, 1, &data);
    // Set timer value
    data = 5;
    WriteNByte(I2C_ADDR_RTC, 0x0F, 1, &data);
    // Shutdown
    M5.Axp.PowerOff();
  }
  else if(M5.BtnC.wasPressed() == true)
  {
    // Shutdown
    M5.Axp.PowerOff();
  }
}
