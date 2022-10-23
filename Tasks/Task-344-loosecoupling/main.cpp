
#include "Flashy.hpp"
#include <chrono>
#include <cstdint>
#include <ratio>

//Comment this next line to use simulated hardware
#define USE_REAL_HW

//Switch between real hardware and mocked
#ifdef USE_REAL_HW

#include "uop_msb.h"
#include "MbedLight.hpp"
#include "MbedTimer.hpp"
MbedLight redLed(LED1);                 //Concrete class
MbedTimer tmr;

ILightNotify& lightObj = redLed;        //Alias
ITimer& tmrObj = tmr;

#else

#include "MockedLight.hpp"
#include "MockedTimer.hpp"
MockedLight redLedMocked("RED LED");    //Concrete class
MockedTimer tmr(1000ms);

ILightNotify& lightObj = redLedMocked;  //Alias
ITimer& tmrObj = tmr;
#endif


int main()
{
    Flashy f(tmrObj, lightObj, 250ms);
    
    while (true) {
        f.flashOnce();
        tmrObj.wait_for(1000ms);
    }
}

/*
Part 6 - Both MbedLight and MockedLight have virtual void functions to turn on and off
the 'led'. However, the the private terms for both classes calls different things. The
MbedLight class calls the DigitalOut pin for the LED whereas the MockLight creates a 
boolean state which gets set to minic turning on and off the 'LED'.

Part 7 - The error that occurs is "Variable type 'MbedLight' is an abstract class". This
is because it must override all of the APIs in order to compile.

Part 8 - Not sure about this; will ask James.
*/

