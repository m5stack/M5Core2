#include "touch.h"

touch::touch()
{
}

touch::~touch()
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

bool touch::ispressed()
{
    return ( digitalRead(39) == LOW );
}

void touch::read()
{
	if (millis() - _lastRead < MIN_INTERVAL) return;
	point[0].x = point[0].y = point[1].x = point[1].y = -1;
	_lastRead = millis();
	if (!ispressed())
	{
		points = 0;
	}
	else
	{
		const uint8_t size = 11;
		uint8_t data[size];
		Wire1.beginTransmission((uint8_t)CST_DEVICE_ADDR);
		Wire1.write(0x02);
		Wire1.endTransmission();
		Wire1.requestFrom((uint8_t)CST_DEVICE_ADDR, size);
		for (uint8_t i = 0; i < size; i++)
		{
			data[i] = Wire1.read();
		}
		points = data[0];
		if (points > 2) points = 0;
		if (points)
		{
			uint8_t id0 = data[3] >> 4;
			point[0].x = ((data[1] << 8) | data[2]) & 0x0fff;
			point[0].y = ((data[3] << 8) | data[4]) & 0x0fff;
			point[1].x = ((data[7] << 8) | data[8]) & 0x0fff;
			point[1].y = ((data[9] << 8) | data[10]) & 0x0fff;
			if (points == 2 && id0 == 1)
			{
				TouchPoint_t tmp = point[0];
				point[0] = point[1];
				point[1] = tmp;
			}
		}
	}
}

bool touch::inBox(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	for (uint8_t n = 0; n < points; n++)
	{
		if (point[n].x >= x0 && point[n].x <= x1 && point[n].y >= y0 && point[n].y <= y1) {
			return true;
		}
	}
	return false;
}

bool touch::hasChanged()
{
	bool flag = false;
	if (point[0].x != _p0.x || point[0].y != _p0.y || \
	  point[1].x != _p1.x || point[1].y != _p1.y) flag = true;
	_p0 = point[0];
	_p1 = point[1];
	return flag;
}

TouchPoint_t touch::getPressPoint()
{
	read();
	return point[0];
}