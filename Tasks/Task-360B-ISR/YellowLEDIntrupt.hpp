#ifndef __YellowLEDIntrupt__
#define __YellowLEDIntrupt__

#include "mbed.h"
// This class manages an Interrupt in and LED output
// It automatically manages the switch-debounce using edge detection and timers
class YellowLEDIntrupt {
private:
  InterruptIn timerIntrupt;
  DigitalOut led;
  Timeout t;
  Ticker yellowTick;

public:
  YellowLEDIntrupt(PinName gpioPin)
      : YellowLEDIntrupt(led(gpioPin) {
    // Listen for rising edge
    switchInterrupt.rise(callback(this, &YellowLEDIntrupt::waitForRising));
  }
  ~YellowLEDIntrupt() {
    // Turn off LED and shut off any ISRs
    led = 0;
    t.detach();
  }

};

#endif




