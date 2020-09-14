#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include <Wire.h>

#define CST_DEVICE_ADDR 0x38
#define MIN_INTERVAL 5

typedef struct point
{
    int x;
    int y;
}TouchPoint_t;

typedef struct HotZone
{
    HotZone(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, void (*fun)() = nullptr){
        _x0 = x0; 
        _y0 = y0;
        _x1 = x1;
        _y1 = y1;
        _fun = fun;

    }
    inline void setZone(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, void (*fun)() = nullptr )
    {
        _x0 = x0; 
        _y0 = y0;
        _x1 = x1;
        _y1 = y1;
        _fun = fun;
    }
    inline bool inHotZone(TouchPoint_t point)
    {
        if(( point.x >= _x0 )&&( point.x <=  _x1 )&&
           ( point.y >= _y0 )&&( point.y <=  _y1 ))
        {
            return true;
        }
        return false;
    }
    inline bool inHotZoneDoFun(TouchPoint_t point)
    {
        if(( point.x >= _x0 )&&( point.x <=  _x1 )&&
           ( point.y >= _y0 )&&( point.y <=  _y1 ))
        {
            if( _fun != nullptr )
            {
                _fun();
            }
            return true;
        }
        return false;
    }
    uint16_t _x0;
    uint16_t _y0; 
    uint16_t _x1;
    uint16_t _y1;

    void (*_fun)();

}HotZone_t;

class touch
{
public:
    touch();
    ~touch();
    void begin();
    bool ispressed();
    void read();
    bool inBox(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool hasChanged();
    uint8_t points;
    TouchPoint_t point[2];
	    
    TouchPoint_t getPressPoint();
    
private:
	uint32_t _lastRead;
	TouchPoint_t _p0;
	TouchPoint_t _p1;
};

#endif