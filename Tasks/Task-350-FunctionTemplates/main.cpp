#include "uop_msb.h"
#include "util_functions.hpp"

#include <cstdint>
#include <iostream>
using namespace std;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
AnalogIn pot(AN_POT_PIN);

#define N 64

int main()
{
    printf("\n\rStart\n\r");

    while (true) {
        //Read the ADC     
        uint16_t uPot = pot.read_u16() >> 4;    //12-bit Integer 0..4095
        float    fPot = (float)uPot / 4095.0f;  //Scaled 0.0-1.0      
        
        //Floating point version
        if (hasIncreasedBy(fPot, 0.1f)) {
            led1 = 1;
        }
        else if (hasDecreasedBy<float>(fPot, 0.1f)) {
            led1 = 0;
        }

        //16-bit unsigned integer version
        if (hasIncreasedBy<uint16_t>(uPot, 410u)) {
            led2 = 1;
        }
        else if (hasDecreasedBy<uint16_t>(uPot, 410u)) {
            led2 = 0;
        }
 
        cout << "Mean: " << addToBuffer<uint16_t, double, 20>(uPot) << endl;
        wait_us(500000);
    }
}

/*
Part 4 - Warning of int occurs stating that the float number '0.1' will be rounded down to
'0'.
Part 5 - Get the error "No matching function for call to 'hasIncreasedBy' or 
'hasDecreasedBy'" so cannot build when the <types> are missing.
Part 6 - Same error occurs as Part 5 as it doesn't know the variable type.
Part 7 - I believe I updated to sample 20 times but will need to check when I have my 
board.
Part 8 - R sets the type of sum and N to double to add precision, makes decimal numbers 
possible.
*/


