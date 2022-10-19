#include "TrafficLight.h"
#include <chrono>

//Contructor
TrafficLight::TrafficLight(PinName redPin, PinName yellowPin, PinName greenPin) 
                                                        : redLED(redPin), yellowLED(yellowPin), greenLED(greenPin)
{
    redLED.output();
    yellowLED.output();
    greenLED.output();
    // These objects are already initialised in the member initialisation list above
    redLED = 1;
    yellowLED = 0;
    greenLED = 0;
    // Timer off
    LEDChooser=0;
    flashYellow(false);
}

//Contructor
TrafficLight::TrafficLight(int open, PinName redPin, PinName yellowPin, PinName greenPin) 
                                                        : redLED(redPin), yellowLED(yellowPin), greenLED(greenPin)
{
    redLED.output();
    yellowLED.output();
    greenLED.output();
    redLED.mode(OpenDrainNoPull);
    yellowLED.mode(OpenDrainNoPull);
    greenLED.mode(OpenDrainNoPull);
    // These objects are already initialised in the member initialisation list above
    redLED = 1;
    yellowLED = 1;
    greenLED = 0;
    LEDChooser = 1;
    // Timer off
    flashYellow(false);
}

//Destructor
TrafficLight::~TrafficLight()
{
    redLED = 1;
    yellowLED = 0;
    greenLED = 0;
} 

// Interrupt Service Routine (ISR)
void TrafficLight::yellowFlashISR() {
    yellowLED = !yellowLED;
}

// Private member function to switch flasher on or off
void TrafficLight::flashYellow(bool flash) {
    t.detach(); //Turn off ticker
    if (flash) {
        //Turn on ticker ..... Hmmm, interrupts!
        t.attach(callback(this, &TrafficLight::yellowFlashISR), speed);
    }
}

void TrafficLight::stop(){
    redLED = 1;
    yellowLED = 0;
    greenLED = 0; 
    State = STOP; 
}

void TrafficLight::setFlashSpeed(double setSpeed) {
    speed = (int)setSpeed * 1ms; //Forces double setSpeed to an int so it can be times by microseconds.
    findingSpeed = setSpeed;
}

double TrafficLight::getFlashSpeed(){
    double findingFlashSpeed = findingSpeed;
    //double flashSpeed = 1000*duration<double>(speed).count(); //Convert from microseconds to milliseconds and output as double - James Solution.
    //return flashSpeed;
    return findingFlashSpeed; 
}

// Moore Machine - update outputs
void TrafficLight::updateOutput()
{
    switch (State)
    {
        case STOP:
            if (LEDChooser==0){
                flashYellow(false);
                redLED = 1;
                yellowLED = 0;
                greenLED = 0;
            }
            if (LEDChooser==1){  
                flashYellow(false);   
                redLED = 1;
                yellowLED = 1;
                greenLED = 0;
            }
            break;
        case READY:
            if (LEDChooser==0) {
                flashYellow(false);
                redLED = 1;
                yellowLED = 1;
                greenLED = 0;
            }
            if (LEDChooser==1){
            redLED = 1;
            flashYellow(true);
            greenLED = 1;
            } 
            break;
        case GO:
            if (LEDChooser==0) {
                flashYellow(false);
                redLED = 0;
                yellowLED = 0;
                greenLED = 1;
            }
            if (LEDChooser==1) {
                flashYellow(false);
                redLED = 0;
                yellowLED = 1;
                greenLED = 1;
            }
            break;
        case WARNING:
            if (LEDChooser==0) {
                redLED = 0;
                flashYellow(true);
                greenLED = 0;
            }
            if (LEDChooser==1) {
                redLED = 0;
                yellowLED = 0;
                greenLED = 1;
            }
            break;         
    }       
}

// Moore machine - next state logic
TrafficLight::LIGHT_STATE TrafficLight::nextState()
{
    // Update State
    switch (State)
    {
        case STOP:
            State = READY;
            break;
        case READY:
            State = GO;
            break;
        case GO:
            State = WARNING;
            break;
        case WARNING:
            State = STOP;
            break;
    }

    //As it says
    updateOutput();

    //Return the current state (for information)
    return State; 
} 

// Moore machine - next state logic to reset the machine.
TrafficLight::LIGHT_STATE TrafficLight::reset()
{
    // Update State
    switch (State)
    {
        case STOP:
            State = STOP;
            break;
        case READY:
            State = STOP;
            break;
        case GO:
            State = STOP;
            break;
        case WARNING:
            State = STOP;
            break;
    }

    //As it says
    updateOutput();

    //Return the current state (for information)
    return State; 
} 