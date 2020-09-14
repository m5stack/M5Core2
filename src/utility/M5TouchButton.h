#ifndef M5TouchButton_h
#define M5TouchButton_h
// #if ARDUINO >= 100
#include <Arduino.h>
// #else
// #include <WProgram.h>
// #endif

#include "touch.h"

class TouchButton {
	public:
		TouchButton(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, void (*fnPress_)(TouchButton &) = nullptr, void (*fnRelease_)(TouchButton &) = nullptr, uint8_t id_ = 0);
		~TouchButton();
		uint16_t x0;
		uint16_t y0;
		uint16_t x1;
		uint16_t y1;
		void (*fnPress)(TouchButton &);
		void (*fnRelease)(TouchButton &);
		uint8_t id;
		
		uint8_t setState(uint8_t);
		uint8_t isPressed();
		uint8_t isReleased();
		uint8_t wasPressed();
		uint8_t wasReleased();
		uint8_t pressedFor(uint32_t ms);
		uint8_t pressedFor(uint32_t ms, uint32_t continuous_time);
		uint8_t releasedFor(uint32_t ms);
		uint8_t wasReleasefor(uint32_t ms);
		uint32_t lastChange();

	private:
		uint8_t _state;					//current TouchButton state
		uint8_t _lastState;			//previous TouchButton state
		uint8_t _changed;				//state changed since last read
		uint32_t _time;					//time of current state (all times are in ms)
		uint32_t _lastChange;		//time of last state change
		uint32_t _lastLongPress;	 //time of last state change
		uint32_t _pressTime;		//press time
		uint32_t _hold_time;		//hold time call wasreleasefor
};

class TouchButtons {
	friend class TouchButton;
	public:
		void read(touch &touchDevice);
	private:
		static std::vector<TouchButton*> _buttons;
};

#endif
