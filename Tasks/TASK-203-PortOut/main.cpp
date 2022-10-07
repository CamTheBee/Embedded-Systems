#include "mbed.h"

// Hardware Definitions
#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2

//Task 6
#define ON_BOARD_LED1 PB_0
#define ON_BOARD_LED2 PB_7
#define ON_BOARD_LED3 PB_14

//Task 3 - Before main, all the LEDs are set to 1. The reason it looks like the binary is wrong is because the port is PC_0 to PC_15; not 1 to 16.
//Task 4 - Bits 3,4 and 7 are set to 1 to turn the LEDs on as the port has 16 pins, starting from 0 to 15, as explained above.
//#define LEDMASK 0b0000000001001100
//Task 5:
#define LEDMASK 0b0000000001001000 //PC_2 (Red LED should not start off as off because the pin it is connected to is now pulled low.

//Task 6 LEDMASK
#define ONBOARDLEDMASK 0b0100000010000001

// Objects
//BusOut leds(TRAF_RED1_PIN, TRAF_YEL1_PIN, TRAF_GRN1_PIN);
PortOut leds(PortC, LEDMASK);

//Task 6 PortOut
PortOut onboardleds(PortB, LEDMASK);

int main()
{
    /*
    while (true) {
        leds = 0;   //Binary 000
        onboardleds = 0; //Task 6 Binary = 0
        wait_us(500000);
        leds = 0xFF;   //Binary 11111111
        onboardleds = 0xFFFF; //Task 6 - Binary 1111111111111111
        wait_us(500000);    
    }
    */
    //Task 7 - Need to check.
    while (true) {
        leds = leds ^ LEDMASK; //Alternates first set of LEDs
        onboardleds = onboardleds ^ ONBOARDLEDMASK; //Alternates second set of LEDs
        wait_us(500000); //Waits half a second
    }
}




