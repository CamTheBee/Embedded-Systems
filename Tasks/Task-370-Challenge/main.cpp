#include "uop_msb.h"
#include <chrono>
#include <cstdio>
using namespace uop_msb;

//Used this to 
Ticker tick;
Timer average;
Thread t1;
osThreadId_t mainThreadID;

float adcValue = 0;
float total = 0;
AnalogIn ldr(AN_LDR_PIN);

void readADC() {
    adcValue = ldr.read();
    ThisThread::flags_clear(1);
    //ThisThread::sleep_for(1ms);
}

void threadFlag() {
    t1.flags_set(1);
}



int main(void)
{
/*
1. Create an ISR - called by a Ticker every 1ms. This ISR simply has the job of signalling a waiting thread to perform an ADC conversion
2. Create a thread that waits for a signal from the ISR. This thread should ideally have the highest priority
3. Each time it is unblocked by the signal, it should read the ADC (Use `AnalogIn`) for the LDR and add it to a running sum.
4. Every 1s, it should print out the average of the past 1000 samples to the terminal. 
*/
    t1.start(readADC);
    mainThreadID = t1.get_id();
    average.reset();
    average.start();
    tick.attach(&threadFlag, 1ms);
    
   
    while (true) {
        total = total + adcValue;
        microseconds averageTime = average.elapsed_time();
        if (averageTime >= 1000ms) {
            average.stop();
            printf("Current second Average: %f\n", total);
            total = 0;
            average.reset();
            average.start();

    }
        ThisThread::sleep_for(1000ms);
        
    }
    
}   