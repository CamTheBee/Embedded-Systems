#include "uop_msb.h"
#include "SwitchTimerLedManager.hpp"
#include "FlashingLED.hpp"
#include <chrono>
#include <ratio>
using namespace uop_msb;
using namespace chrono;


SwitchTimerLedManager fsm1(BTN1_PIN, SwitchTimerLedManager::UP);
SwitchTimerLedManager fsm2(BTN2_PIN, SwitchTimerLedManager::DOWN);
FlashingLED green(TRAF_GRN1_PIN, 250ms);
FlashingLED yellow(TRAF_YEL1_PIN, 500ms);
FlashingLED red(TRAF_RED1_PIN, 1000ms);

//Dual Digit 7-segment Display
LatchedLED disp(LatchedLED::SEVEN_SEG);



int main()
{
    int count = 0;

    //Turn ON the 7-segment display
    disp.enable(true);
    disp = 0;

    while (true) {

        fsm1.readInputs();
        fsm2.readInputs();
        green.currentTmr();
        yellow.currentTmr();
        red.currentTmr();

        // **********************************
        // UPDATE "STATE" for buttons A and B
        // **********************************
        fsm1.updateState(count);
        fsm2.updateState(count);
        green.ledChange();
        yellow.currentTmr();
        red.currentTmr();

        // UPDATE OUTPUTS
        disp = count;
  
    }
}





