#ifndef __TRAFFICLIGHT__
#define __TRAFFICLIGHT__

#include "mbed.h"
#include <chrono>
using namespace chrono;

#define TRAF_GRN1_PIN PC_6
#define TRAF_YEL1_PIN PC_3
#define TRAF_RED1_PIN PC_2
#define TRAF_GRN2_PIN PC_9
#define TRAF_YEL2_PIN PC_8
#define TRAF_RED2_PIN PC_7

class TrafficLight 
{
    public:
    typedef enum {STOP, READY, GO, WARNING} LIGHT_STATE;

    private:
    DigitalInOut redLED;
    DigitalInOut yellowLED;
    DigitalInOut greenLED;

    Ticker t;
    LIGHT_STATE State;
    microseconds speed = 300ms;
    double findingSpeed;
    void yellowFlashISR();
    void flashYellow(bool flash);
    void updateOutput();
    int LEDChooser=0;

    public:
    //Constructor
    TrafficLight(PinName redPin = TRAF_RED1_PIN, PinName yellowPin = TRAF_YEL1_PIN, PinName greenPin = TRAF_GRN1_PIN); 
    TrafficLight(int open, PinName redPin = TRAF_RED2_PIN, PinName yellowPin = TRAF_YEL2_PIN, PinName greenPin = TRAF_GRN2_PIN);
    //Destructor
    ~TrafficLight();

    //Advance the traffic lights to the next state
    LIGHT_STATE nextState();

    //Challenge Tasks
    LIGHT_STATE reset();
    void setFlashSpeed(double setSpeed);
    void stop();
    double getFlashSpeed();
};


#endif