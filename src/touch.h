#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include <Wire.h>

#define CST_DEVICE_ADDR 0x38

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
private:
    /* data */
public:
    
    TouchPoint_t _TouchPoint;
    
public:
    touch(/* args */);
    ~touch();
    void begin();
    bool ispressed();
    TouchPoint_t getPressPoint();
    HotZone_t* creatHotZone(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h);
private:
    int readTouchtoBuff(uint16_t *posx, uint16_t *posy);
};


#endif