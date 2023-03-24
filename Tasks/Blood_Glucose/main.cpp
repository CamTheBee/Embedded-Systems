#include "mbed.h"
#include <cstdint>
#include <cstdio>


DigitalOut LED(PB_0);
AnalogIn PDAC(PC_1);
AnalogIn PDDC(PC_0);

int main()
{
    printf("Start!\n");
    LED = 1;
    int n = 0;
    unsigned short PDTotalDC = 0;
    unsigned short PDTotalAC = 0;
    int32_t ACValue = 0;

    while (true) {
        
        PDTotalDC = PDDC.read_u16();
        PDTotalAC = PDAC.read_u16();

        if (PDTotalAC > 32768) {
            ACValue = PDTotalAC - 32768;
        }
        else if (PDTotalAC < 32768) {
            ACValue = (PDTotalAC - 32768)*-1;
        }
        else {
            ACValue = 0;
        }

        printf("DC Value: %hu\n", PDTotalDC);
        printf("AC Value: %i\n\n", ACValue);
        wait_us(1000000);
        
    }
}



