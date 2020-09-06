#include "touch.h"

touch::touch(/* args */)
{
}
touch::~touch(void)
{

}

void touch::begin(void)
{
    Wire1.begin(21,22);

    pinMode(39, INPUT);

    Wire1.beginTransmission(CST_DEVICE_ADDR);
    Wire1.write(0xA4);
    Wire1.write(0x00);     // INT polling mode
    Wire1.endTransmission();
}

bool touch::ispressed(void)
{
    return ( digitalRead(39) == LOW );
}

const TouchPoint_t& touch::getPressPoint(void)
{
    if( ( digitalRead(39) != LOW ) || readTouchtoBuff() != 0x02 )
    {
        _TouchPoint.x = _TouchPoint.y = -1;
    }
    return _TouchPoint;
}

int touch::readTouchtoBuff(void)
{
    Wire1.beginTransmission(CST_DEVICE_ADDR);
    Wire1.write(0x02);
    if (Wire1.endTransmission() != 0)
        return -1;
    uint8_t buff[5];
    Wire1.readTransmission(CST_DEVICE_ADDR, buff, 5);

    _TouchPoint.x = ((buff[1] << 8) | buff[2]) & 0x0fff;
    _TouchPoint.y = ((buff[3] << 8) | buff[4]) & 0x0fff;

    return (buff[1] >> 6) & 0x03;
}