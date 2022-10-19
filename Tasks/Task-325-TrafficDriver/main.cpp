#include "mbed.h"
#include "TrafficLight.h"

TrafficLight lights;
TrafficLight lightsOpenDrain(1);
DigitalIn sw(USER_BUTTON);
DigitalIn ButtonA(PG_0);
Timer stop;

TrafficLight::LIGHT_STATE s;
TrafficLight::LIGHT_STATE r;
int main()
{
    microseconds stopRead = stop.elapsed_time();   //Part of the C++ chrono class
    while (true) {
        //lights.setFlashSpeed(2000);
        //lights.getFlashSpeed();
        //Wait for switch press
        while (sw==0) {
/*
            //Instant Red Button.
            if (ButtonA==1) {
                stop.start();
                stopRead = stop.elapsed_time();
                while (stopRead < 300ms) {
                    stopRead = stop.elapsed_time();
                }
                stop.reset();
                stopRead = 0ms;
                //s = lights.reset();
                lights.stop();
                while(ButtonA==1)
                stop.start();
                stopRead = stop.elapsed_time();
                while (stopRead < 300ms) {
                    stopRead = stop.elapsed_time();
                }
                stop.reset();

            }
            */
        }

        //Update lights
        s = lights.nextState();
        r = lightsOpenDrain.nextState();

        //Debounce switch
        wait_us(300000);

        //Wait for switch release
        while (sw==1);

        //Switch debounce
        wait_us(300000);
        
    }
}


//Part 5 - The constructor runs first, then main as DigitalIn is called before main.
