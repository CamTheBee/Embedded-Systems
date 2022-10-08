#include "uop_msb.h"
#include <chrono>
#include <ratio>
using namespace uop_msb;
using namespace chrono;

//Buzzer
Buzzer buzz;

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led1(LED1);
static DigitalOut led2(LED2);

// Create a UnbufferedSerial object with a default baud rate.
static UnbufferedSerial serial_port(USBTX, USBRX);

//Ticker
Ticker ledTicker;

//Flash rate (us)
volatile long long T = 500000;

//ISR for flashing blue LED
void onTick()
{
    led2 = !led2;
}

void blueFlashUp() {

    char x;
    if (serial_port.read(&x, 1)) {

        //Check the character input
        switch (x) {
            case 13:
            //User pressed return
                serial_port.write("\n\r",2); //Echo a newline
                break;

            case 'P':
                serial_port.write(&x,1);
                T += 100000;
                ledTicker.detach();
                ledTicker.attach(&onTick, microseconds(T));
                break;
            
            default:
                //Echo typed character to the terminal
                serial_port.write(&x,1);
                break;    
        };
    }
}

void blueFlashDown() {

    char y;
    int downCounter = 0;
    if (serial_port.read(&y, 1)) {

        //Check the character input
        switch (y) {
            case 13:
            //User pressed return
                serial_port.write("\n\r",2); //Echo a newline
                downCounter = 0;
                break;
            
            case 'O':
                downCounter++;
                serial_port.write(&y,1);
                break;
            
            case 'W':
                if (downCounter == 1){
                    downCounter++;
                    serial_port.write(&y,1);
                }
                else {
                    downCounter = 0;
                    serial_port.write(&y,1);
                }
                break;

            case 'N':
                if (downCounter == 2) {
                    downCounter = 0;
                    serial_port.write(&y,1);
                    T += 100000;
                    ledTicker.detach();
                    ledTicker.attach(&onTick, microseconds(T));
                }
                else {
                    downCounter = 0;
                    serial_port.write(&y,1);
                }
                break;
            
            default:
                //Echo typed character to the terminal
                serial_port.write(&y,1);
                downCounter = 0;
                break;    
        };
    }
}

void on_rx_interrupt()
{
    CriticalSectionLock lock;
    char p;

    // Read the data to clear the receive interrupt.
    if (serial_port.read(&p, 1)) {

        //Check the character input
        switch (p) {
        case 13:
            //User pressed return
            serial_port.write("\n\r",2); //Echo a newline
            break;
        case '1':
            //Switch on buzzer
            buzz.playTone("A");
            break;
        case '2':
            //Switch off buzzer
            buzz.rest();
            break;

        case '+':
            T += 100000;
            ledTicker.detach();
            ledTicker.attach(&onTick, microseconds(T));
            break;
        case '-':
            if (T >= 200000) {
                T -= 100000;
                ledTicker.detach();
                ledTicker.attach(&onTick, microseconds(T));
            }
            break;  
  
/*
//Challenge Task//
        case 'U':
            serial_port.write(&p,1);
            blueFlashUp();
            break;

        case 'D':            
            serial_port.write(&p,1);
            blueFlashDown();
            break;            

        default:
            //Echo typed character to the terminal
            serial_port.write(&p,1);
            break;
        */
        };
    }

    // Toggle the LED.
    led1 = !led1; 
}




LCD_16X2_DISPLAY disp;

int main(void)
{
    // Set desired properties (115200-8-N-1).
    serial_port.baud(115200);
    serial_port.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    // Register a callback to process a Rx (receive) interrupt.
    serial_port.attach(&on_rx_interrupt, SerialBase::RxIrq);

    //ISR to flash led
    CriticalSectionLock::enable();
    ledTicker.attach(&onTick, microseconds(T));
    CriticalSectionLock::disable();

    while (true) {
        sleep(); 
        disp.cls();
        disp.printf("T=%Ldms", T/1000);    
    }
    
}