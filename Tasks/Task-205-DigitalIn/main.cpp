#include "mbed.h"
// Hardware Definitions
#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2

#define TRAF_GRN2_PIN PC_9
#define TRAF_YEL2_PIN PC_8
#define TRAF_RED2_PIN PC_7

#define TRAF_WHITE_PIN PF_10

#define BTN1_PIN PG_0 
#define BTN2_PIN PG_1
#define BTN3_PIN PG_2 // No pull down
#define BTN4_PIN PG_3 // No pull down

// Inputs
DigitalIn SW2(BTN1_PIN);
DigitalIn SW3(BTN2_PIN);
DigitalInOut SW4(BTN3_PIN,PIN_INPUT,PullDown,0);
DigitalInOut SW5(BTN4_PIN,PIN_INPUT,PullDown,0);

// Outputs
DigitalOut ledRed(TRAF_RED1_PIN);
DigitalOut ledYel(TRAF_YEL1_PIN);
DigitalOut ledGrn(TRAF_GRN1_PIN);

int main()
{
    while (true) {
        //if (SW2 == 1) {
        if (SW4 == 1) { //Task 3
            ledRed = 1;
        } else {
            ledRed = 0;
        }

        //if (SW3.read() == 1) {
        if (SW5 == 1) { //Task 3
            ledYel = 1;
        } else {
            ledYel = 0;
        }
    }
}

//Task 4 - PullDown is required for switches SW4 and SW5 as they are configured as open drain switches.