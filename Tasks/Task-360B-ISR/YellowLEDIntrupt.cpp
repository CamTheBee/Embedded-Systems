#include "YellowLEDIntrupt.hpp"

// Member functions

void toggleYellowISR() {
    yellow_led = !yellow_led;    
}
    
yellowTick.attach(&toggleYellowISR, 500ms);