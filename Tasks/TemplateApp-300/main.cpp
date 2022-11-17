#include "mbed.h"
#include "uop_msb.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <cmath>
#include <chrono>
#include <ostream>
#include <ratio>
using namespace std;
using namespace chrono;

AnalogIn mic(MIC_AN_PIN);

//Ticker sampleTime;

volatile float xn;
volatile int N[2] = {205, 210};
volatile int k[2][8] = {{18, 20, 22, 24, 31, 34, 38, 42}, {35, 39, 43, 47, 61, 67, 74, 82}}; 
volatile float pi = 3.14159265359;
volatile int homCount = 0;
volatile int arrayCount = 0;

/*
float sampling() {
    xn = mic.read();
    return xn;    
}
*/

double findingvn(float xn, float vn1, float vn2) {

    float vntemp = (((2*cos((2*pi*k[homCount][arrayCount]/N[homCount])))*vn1) - vn2 + xn);  
    return vntemp;

}

double firstHom(float vn, float vn1, volatile int n) {

    volatile int k[8] = {18, 20, 22, 24, 31, 34, 38, 42};
    volatile int N = 205;
        float YkTemp = (vn*vn) + (vn1*vn1) - ((2*cos((2*pi*k[n]/N))*vn*vn1));
    YkTemp = abs(YkTemp)*abs(YkTemp);
    //cout << "1st Value " << n << ": " << YkTemp << endl << endl;
    return YkTemp;
}

double secondHom(float vn, float vn1, volatile int n) {

    volatile int N = 210;
    volatile int k[8] = {35, 39, 43, 47, 61, 67, 74, 82};
    float YkTemp = (vn*vn) + (vn1*vn1) - ((2*cos((2*pi*k[n]/N))*vn*vn1));
    YkTemp = abs(YkTemp)*abs(YkTemp);
    //cout << "2nd Value " << n << ": " << YkTemp << endl << endl;
    return YkTemp;
}

int main() {
    //sampleTime.attach(&sampling, 125us);
    int sample = 0;
    float xsamples[205];

    while (sample<205) {
        xsamples[sample] = mic.read();
        sample++;
        wait_us(125);  
    }

    volatile float vn = 0;
    volatile float vn1 = 0;
    volatile float vn2 = 0;
    volatile int n = 0;
    volatile int arrayPlacement = 0;
    float vnArray[16] = {0};
    float vn1Array[16] = {0};
    float vn2Array[16] = {0};

    while (homCount!=2) {
        while (arrayCount<8) {
            while (n<205) {
                vn2 = vn1;
                vn1 = vn;
                xn = xsamples[n];
                vn = findingvn(xn,vn1,vn2);
                n++;
            }
            n=0;
            arrayCount++;
            vnArray[arrayPlacement] = vn;
            vn1Array[arrayPlacement] = vn1;
            vn2Array[arrayPlacement] = vn2;
            arrayPlacement++;
            vn = 0;
            vn1 = 0;
            vn2 = 0;
        }
        arrayCount=0;
        homCount++;
    }
    ////////////////////////////////////////////////////////////////////////////

    float YkFirHom[2][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {697, 770, 852, 941, 1209, 1336, 1477, 1633}};
    float YkSecHom[2][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {1394, 1540, 1704, 1882, 2418, 2672, 2954, 3266}};

    while (arrayCount<8) {
        vn = vnArray[arrayCount];
        vn1 = vn1Array[arrayCount];
        YkFirHom[0][arrayCount] = firstHom(vn,vn1,(arrayCount));
        vn = vnArray[arrayCount+8];
        vn1 = vn1Array[arrayCount+8];
        YkSecHom[0][arrayCount] = secondHom(vn,vn1,(arrayCount));
        arrayCount++;
    }
    
    /*
    //Checking for biggest magnitude.
    volatile float magCheck1 = 0;
    volatile float freqCheck1 = 0;
    volatile float magCheck2 = 0;
    volatile float freqCheck2 = 0;
    volatile int i = 0;
 
    while (i<8) {
        if (magCheck1 < YkFirHom[0][i]) {
            magCheck1 = YkFirHom[0][i];
            freqCheck1 = YkFirHom[1][i];
        }
        if (magCheck2 < YkSecHom[0][i]) {
            magCheck2 = YkSecHom[0][i];
            freqCheck2 = YkSecHom[1][i];
        }
        i++;
    }
    cout << "Mag Value 1: " << magCheck1 << endl;
    cout << "Freq Value 1 " << freqCheck1 << endl;
    cout << "Mag Value 2: " << magCheck2 << endl; 
    cout << "Freq Value 2: " << freqCheck2 << endl;
    */

    int i = 0;
    while (i<8) {
        cout << "1st Hom Freq Value " << i << ": " << YkFirHom[1][i] << endl << endl;
        cout << "1st Hom Mag Value " << i << ": " << YkFirHom[0][i] << endl << endl;
        
        i++;
    }
    i = 0;
    while (i<8) {
        cout << "2nd Hom Freq Value " << i << ": " << YkSecHom[1][i] << endl << endl;
        cout << "2nd Hom Mag Value " << i << ": " << YkSecHom[0][i] << endl << endl;
        i++;
    }
    
}






























/*

}


// main() runs in its own thread in the OS
int main()
{
    volatile int count = 0;
    volatile int i = 0;
    volatile int n = 0;
    
    float vn;
    float xn;
    float vn1 = 0;
    float vn2 = 0;


    
    float xdata[210] = {0};
    int time = 0;

    while (count<210) {
        xdata[time] = mic.read();
        wait_us(125);
        count++;
    }
    
    while (n<8) {
        vn = findingV1(xn,vn1,vn2,n);
        YkSecHom[0][n] = secondHom(vn,vn1,n);
        cout << "First Hom Value " << n << ": " << YkFirHom[0][n] << endl;
        cout << "Second Hom Value " << n << ": " << YkSecHom[0][n] << endl;
        n++;
    }
    


    n=0;
    while (n<8) {
        YkFirHom[0][n] = firstHom(vn,vn1,n);
        YkSecHom[0][n] = secondHom(vn,vn1,n);
        cout << "First Hom Value " << n << ": " << YkFirHom[0][n] << endl;
        cout << "Second Hom Value " << n << ": " << YkSecHom[0][n] << endl;
        n++;
    }
    


*/



