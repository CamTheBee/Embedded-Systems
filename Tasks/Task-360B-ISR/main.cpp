#include "mbed.h"
#include "SwitchManager.hpp"
#include "YellowLEDIntrupt.hpp"

#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2
#define BTN1_PIN PG_0
#define BTN2_PIN PG_1


int main() {
    
    //Interrupt controlled red led using BTN1
    SwitchManager sm1(BTN1_PIN, TRAF_RED1_PIN);

    //Interrupt controlled green led using BTN2    
    SwitchManager sm2(BTN2_PIN, TRAF_GRN1_PIN);    
    
    //Simple ticker for the yellow LED
    YellowLEDIntrupt inn(TRAF_YEL1_PIN);
    
    //Now loop forever
    while(1) { 
        sleep();
        printf("count=%u\n",SwitchManager::getCount());
    };
}

/*
Part 4 - sm1 and sm2 both use the same constructor which is SwitchManager.
The reason count is constantly displayed with the same number is due to the processor being woken up by an interupt every 500ms.

Part 5: 
Count def - static volatile uint32_t count;
getCount def - static uint32_t getCount() { return count; }
They are both static so they belong to the class and not any single object.
*/