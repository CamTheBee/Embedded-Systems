#include "uop_msb.h"
using namespace uop_msb;

// Inputs
DigitalIn SW2(BTN1_PIN);
DigitalIn SW3(BTN2_PIN);
DigitalIn BlueButton(USER_BUTTON);

// Outputs
DigitalOut ledRed(TRAF_RED1_PIN);
DigitalOut ledYel(TRAF_YEL1_PIN);
DigitalOut ledGrn(TRAF_GRN1_PIN);

// Timers (modified version from Timer)
TimerCompat tmr_flash;
TimerCompat tmr_red;
TimerCompat tmr_grn;
TimerCompat tmr_user;
// THE CODE BELOW IS NOT A SOLUTION
//
// IT IS FUNDAMENTALLY FLAWED (AND INCOMPLETE)
//
//
int button2 = 1;
int button3 = 1;
int userbutton = 1;
int delay_ms;
int parsed = scanf("%d", &delay_ms);

int main()
{
    //Start flashing timer
    tmr_flash.start();
    while (true) {

        if (tmr_flash.read_ms() > delay_ms * 1000) {
            ledYel = !ledYel;
            tmr_flash.reset();
            tmr_flash.start();
        }
        switch (userbutton){
            case 1:
                if (BlueButton.read() == 1){
                tmr_user.reset();
                tmr_user.start();
                userbutton = 2;
                }
            break;

            case 2:
                if (tmr_user.read_ms() > 50) {
                    userbutton = 3;   
                }
            break;

            case 3:
                if (BlueButton.read() == 0) {
                    tmr_user.reset();
                    tmr_user.start();
                    userbutton = 4;   
                }
            break;

            case 4:
                if (tmr_user.read_ms() > 50) {
                    //scanf
                    printf("\n\nTo set the delay (in ms), type in an integer number and press return\n");
                    printf("You entered %d correct integer values. The value was %dms\n", parsed, delay_ms);    
                } 

                printf("Invalid - please restart and try again\n");
                while(1); 
                userbutton = 1;   
            break;
        }

        switch (button2){
            case 1:
                if (SW2.read() == 1){
                    ledRed = !ledRed;
                    tmr_red.reset();
                    tmr_red.start();
                    button2 = 2;
                }
            break;

            case 2:
                if (tmr_red.read_ms() > 50) {
                    button2 = 3;   
                }
            break;

            case 3:
                if (SW2.read() == 0) {
                    tmr_red.reset();
                    tmr_red.start();
                    button2 = 4;   
                }
            break;

            case 4:
                if (tmr_red.read_ms() > 50) {
                    button2 = 1;   
                }
            break;
        }

        switch (button3){
            case 1:
                if (SW3.read() == 1) {
                    ledGrn = !ledGrn;
                    tmr_grn.reset();
                    tmr_grn.start();
                    button3 = 2;
                }
            break;

            case 2:
                if (tmr_grn.read_ms() > 50) {
                    button3 = 3;   
                }
            break;

            case 3:
                if (SW3.read() == 0) {
                    tmr_grn.reset();
                    tmr_grn.start();
                    button3 = 4;   
                }
            break;

            case 4:
                if (tmr_grn.read_ms() > 50) {
                    button3 = 1;   
                }
            break;
        }


    }
}

//Part 7 - I do not think it would be possilbe to keep the yellow button flashing using this method. However, by use of another method to access the USART with blocking, the LED could remain flashing.



