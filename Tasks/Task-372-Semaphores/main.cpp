#include "uop_msb.h"
#include "PushSwitch.hpp"

#include <chrono>
#include <cstdint>
#include <cstdio>
using namespace uop_msb;
 
Semaphore sem1(5); //How many down increments occur for  init value.
Semaphore sem2(5); //How many up increments occur from init value.
Mutex countLock;
uint16_t counter = 5;
Thread t1;
Thread t2;

void climb()
{
    PushSwitch sw(BTN1_PIN);
    DigitalOut led(LED1);

    while (true) {
        sw.waitForPress(); //Blocking

        led = 1;
        sem2.acquire(); //Decrement
        countLock.lock();
        counter++;
        printf("%u\n", counter);
        countLock.unlock();
        sem1.release(); //Increment
        led = 0;

        //Debounce
        ThisThread::sleep_for(50ms);
        sw.waitForRelease();
        ThisThread::sleep_for(50ms);
    }
}

void descend()
{
    PushSwitch sw(BTN2_PIN);
    DigitalOut led(LED2);

    while (true) {
        sw.waitForPress(); //Blocking

        led = 1;
        sem1.acquire(); //Decrement
        countLock.lock();
        counter--;
        printf("%u\n", counter);
        countLock.unlock();
        sem2.release(); //Increment
        led = 0;

        //Debounce
        ThisThread::sleep_for(50ms);
        sw.waitForRelease();
        ThisThread::sleep_for(50ms);        
    }
}

int main(void)
{
    DigitalOut led(LED3);
    t1.start(climb);
    t2.start(descend);

    while (true) {
        ThisThread::sleep_for(1000ms);
        led = !led;
    }
    
}   

/*
Part 6 - You cannot get pass 0 or 10 because the semaphore locks outs to waiting
before this can occur.
*/