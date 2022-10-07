#include "mbed.h"

// Hardware Definitions
#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2

#define TRAF_GRN2_PIN PC_9
#define TRAF_YEL2_PIN PC_8
#define TRAF_RED2_PIN PC_7

#define TRAF_WHITE_PIN PF_10

//Task 3 - The default state of the LED is on. This is because the pin has been set low so the pnp transistor is on, turning on the LED.

//BusInOut Traffic_Lights_2(PC_7,PC_8,PC_9);
DigitalInOut grnLED(TRAF_GRN2_PIN, PinDirection::PIN_OUTPUT, PinMode::OpenDrainNoPull, 0);

//Task 4:
DigitalInOut redLED(TRAF_RED2_PIN, PinDirection::PIN_OUTPUT, PinMode::OpenDrainNoPull, 1);

int main()
{
    //Note the logic
    while (true) {
        grnLED = 1;
        redLED = 0; //Task 4
        wait_us(1000000);
        grnLED = 0;
        redLED = 1; //Task 4
        wait_us(1000000);
    }
}

