#include "uop_msb.h"
#include "RunningMean.hpp"

#include <iostream>
using namespace std;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
AnalogIn pot(AN_POT_PIN);

//Defines an object with a fixed-size internal buffer
RunningMean<uint16_t, double, 4> buf4;
//Part 4
RunningMean<uint16_t, double, 64> buf64;

int main()
{
    printf("\n\rStart\n\r");

    while (true) {
        //Read the ADC     
        uint16_t uPot = pot.read_u16() >> 4;    //12-bit Integer 0..4095
        float    fPot = (float)uPot / 4095.0f;  //Scaled 0.0-1.0      
 
        //Add sample to buffer.
        buf4 += uPot;
        buf64 += uPot;

        //Output running mean
        cout << "Mean for buf4: " << buf4 << endl;
        cout << "Mean for buf64: " << buf64 << endl;
        wait_us(500000);
    }
}


/*
Using a constructor resists what can be entered into main and put into the class. It means
the class can only be called when those parameters are stated.
*/

