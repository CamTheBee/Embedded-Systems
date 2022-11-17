#include "uop_msb.h"
#include <chrono>
#include <cstdint>
#include "ComplexNumber.hpp"
#include <complex>
#include <iostream>

using namespace uop_msb;
using namespace chrono;

std::complex<unsigned short> x1;
std::complex<unsigned short> yn;


AnalogIn mic(MIC_AN_PIN);

int main()
{
    //Sampling Mic
    /*
    int n = 0;
    unsigned short micTotal[1000];
    while (n<1000) {

        unsigned short micVal   = mic.read_u16(); 
        micTotal[n] = micVal; 

        //Wait 0.25 seconds
        wait_us(125);
        n++;
    }
    printf("Done\n"); 
    n=0;
    while (n<80) {
        printf("Mic Value %i is: %X\n", n, micTotal[n]);
        n++;
        wait_us(1000);
    }  
    */

    x1.real(10);
    x1.imag(20);
    yn=x1;
    printf("Done: ");
    std::cout << "Real Part: " << yn.real() << '\n';
    std::cout << "Imagnary Part: " << yn.imag() << '\n';

}

