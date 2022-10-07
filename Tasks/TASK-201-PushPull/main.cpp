#include "mbed.h"

//Part 3 - Before main, the red LED is on, as it is set to 1.


// Hardware Definitions
#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2

// Objects
//DigitalOut grn(TRAF_GRN1_PIN);
//DigitalOut yel(TRAF_YEL1_PIN);
//DigitalOut red(TRAF_RED1_PIN,1);

///////////////////////////////////////////////////////////////////////////////////////////
//Part 4
DigitalOut grn(TRAF_GRN1_PIN,1);
DigitalOut yel(TRAF_YEL1_PIN,1);
DigitalOut red(TRAF_RED1_PIN,1);
///////////////////////////////////////////////////////////////////////////////////////////

//Part 5 - They are push-pull compatible as they have push/pull resistors.

int main()
{
    while (true) {
        red = 0;
        yel = 1;
        grn = 1;
        wait_us(500000);
        red = 1;
        yel = 0;
        grn = 0;  
        wait_us(500000);    
    }
}

/*
Questions:
PUSH-PULL would be the best choice to run 5V logic as it consumes less power than open-drain as it can be switched off rather than just held high or floating.
The main function is not the first bit of code that excutes as the 'DigitalOut' class runs before main to set the variables grn, yel and red to their equiviant LEDs.
Although the LEDs appear to all change at the same time, the actually do not. They all just change very very quickly so they appear to change all at the same time before the wait function. This is due toi hardware speed as everything in the real world takes time to happen.
*/