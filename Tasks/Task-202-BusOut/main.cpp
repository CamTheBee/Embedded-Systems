#include "mbed.h"

// Hardware Definitions
#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2
//Task 6 pins
#define TRAF_GRN2_PIN PC_14
#define TRAF_YEL2_PIN PC_7
#define TRAF_RED2_PIN PC_0

//Task 3 - When the code enters main, the state of the LEDs depends on what they were previously stated as since they were only defined, not set before entering main.
// Objects
// DigitalOut grn(TRAF_GRN1_PIN);
// DigitalOut yel(TRAF_YEL1_PIN);
// DigitalOut red(TRAF_RED1_PIN,1);
BusOut leds(TRAF_RED1_PIN, TRAF_YEL1_PIN, TRAF_GRN1_PIN, TRAF_RED2_PIN, TRAF_YEL2_PIN, TRAF_RED2_PIN);

int main()
{
    /*
    while (true) {
        leds = 0;   //Binary 000
        wait_us(500000);
        leds = 7;   //Binary 111
        wait_us(500000);    
    }
    */
    /*
    //Task 5
    while (true) {
        for (leds = 0; leds<8; leds=leds+1) {
            wait_us(500000);
        }
        leds = 0;
    }
    */
    //Task 6
    while (true) { //Forever loop
        leds =63; //Binary = 111111
        wait_us(500000); //Half a second wait
        leds = 0; //Binary = 000000
        wait_us(500000); //Half a second wait   
    }
}

/*
Questions:
The first LED listed will be the least significant bit.
The LED parameters are provided LSB first as this will be the first bit of data read and it makes it easier for humans to read.
No, not all the LED pins are from the same port. This is to make the class flexable so 16 GPIO pins can be controlled at one time. This works as it is simply an array of digital outs. This does mean though, like digital out, the pins will not change at the
same time and there is a small delay between each one.
*/
