#include "touch.h"

touch::touch(/* args */)
{
    //Wire1.begin(21,22);  
}
touch::~touch()
{

}

void touch::begin()
{
    Wire1.begin(21,22);  
    pinMode(39, INPUT);
}

bool touch::ispressed()
{
    //bool touch;
    uint16_t pos_x,pos_y;
    //touch = ( digitalRead(39) == LOW ) ? true : false;
    if(  readTouchtoBuff(&pos_x,&pos_y) == 0x02 )
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

TouchPoint_t touch::getPressPoint()
{
    uint16_t pos_x,pos_y;
    if(  readTouchtoBuff(&pos_x,&pos_y) == 0x02 )
    {
        _TouchPoint.x = pos_x;
        _TouchPoint.y = pos_y;
        //Serial.printf("Touch %d:%d\r\n",_TouchPoint.x, _TouchPoint.y);
    }
    else
    {
        _TouchPoint.x = _TouchPoint.y = -1;
    }
    return _TouchPoint;
}

int touch::readTouchtoBuff(uint16_t *posx, uint16_t *posy)
{
    uint8_t buff[5];
    Wire1.beginTransmission(CST_DEVICE_ADDR);
    Wire1.write(0x02);
    if (Wire1.endTransmission() != 0)
        return -1;
    Wire1.requestFrom(CST_DEVICE_ADDR, 5);
    for (int i = 0; i < 5; i++)
    {
        buff[i] = Wire1.read();
        //Serial.printf("%02X ", buff[i]);
    }
    //Serial.println(".");
    if (buff[0] & 0x01)
    {
        *posx = ((buff[1] << 8) | buff[2]) & 0x0fff;
        *posy = ((buff[3] << 8) | buff[4]) & 0x0fff;
        return (buff[1] >> 6) & 0x03;
    }
    return 0;
}