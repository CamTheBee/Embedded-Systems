#include "mbed.h"
#include "uop_msb.h"
#include "coeffs.hpp"
#include "ComplexNumber.hpp"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <cmath>
using namespace std;

double pi = 3.14159265359;
ComplexNumber Xk(0.0,0.0);
ComplexNumber xntemp(0.0,0.0);
ComplexNumber zero(0.0,0.0);
//double xn = 20;
double powerResult = 0;
double realComp;
double imagComp;

Timer tmr;
float freq=0;
double xInp[200]={40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
40,20,20,20,20,20,20,18,21,17,20,21,20,23,20,21,20,18,21,17,
};

double resultArray[2][20];


//take 20 samples, then this runs, continue taking samples 



//xn needs to be an array of 20 values, that are then permutated each iteration. 

int main()
{
    int k = 0;
    int n = 0;
    int N = 200;
    tmr.start();
    while (k<40-1) {
        while (n<200-1) {

            double xn = xInp[n];
            //check if sin needs to be negative
            ComplexNumber xntemp((xn*cosCoeff[n]),(xn*sineCoeff[n]));
            Xk = Xk.addedTo(xntemp);
            //printf("%f\n", Xk.getReal());
            //printf("%f\n", Xk.getImag());

            n++;
            //printf("Cycle %i:\n", n);
            //Xk.display();
            //wait_us(1000);
        }
        //printf("Cycle %i:\n", k);
        Xk.display();
        realComp = abs(Xk.getImag());
        imagComp = abs(Xk.getReal());
        //printf("%f\n", realComp);
        //printf("%f\n", imagComp);
        powerResult = realComp*realComp + imagComp*imagComp;
        //printf("%f", powerResult);
        resultArray[k-1][0] = powerResult;
        freq = (k*(100.0/N));
        resultArray[k-1][1] = freq;
        //cout << "Cycle " << k << ":" << endl;
        //cout << "Imag Component:" << imagComp << endl;
        //cout << "Real Component:" << realComp << endl;
        //cout << "Power Result (Pk) :" << powerResult << endl;
        //cout << "Power: " << powerResult << "| Freq: " << freq << endl; 
        Xk = zero;
        n = 0;
        k++;
        
    }

    //return resultArray;
tmr.stop();
printf("Time taken: %llu milliseconds\n", duration_cast<milliseconds>(tmr.elapsed_time()).count());
for (int i = 0; i<40; i++) {
    printf("Power: %f | Freq: %f\n", resultArray[i][0], resultArray[i][1]);
}
while (true){

}
}




