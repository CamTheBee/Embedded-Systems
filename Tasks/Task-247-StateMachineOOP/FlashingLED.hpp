#include "mbed.h"
#include <chrono>
using namespace std::chrono;

class FlashingLED {

private:
    Timer _tmrLED;
    DigitalOut _led;
    microseconds _currentTime = 0ms;
    microseconds _duration = 0ms;
public:
    FlashingLED(PinName led, microseconds Duration) : _led(led), _duration(Duration){
        _tmrLED.start();
        _led = 0;
    }

    void start(){
        _tmrLED.start();
    }

    void end(){
        _tmrLED.stop();
    }

    void currentTmr(){
        _currentTime = _tmrLED.elapsed_time();
    }

    void ledChange() {
        if (_currentTime >= _duration){
            _led = !_led;
            _currentTime = 0ms;
            _tmrLED.reset();
        }
    }
};

/*
Had to use Nick's solution to guide me through the answer. I will revisit this task next week at some point to try and re-understand it by doing it with no support. 
I believe I now have a better understanding of classes but more support may need to be required.
*/