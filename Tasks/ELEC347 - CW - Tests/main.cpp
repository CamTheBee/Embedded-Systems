#include "mbed.h"
#include "uop_msb.h"
#include "findingV.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <cmath>
#include <chrono>
#include <ostream>
#include <ratio>
using namespace std;
using namespace chrono;
using namespace uop_msb;

//ISR
Ticker sampleTimer;
Ticker storingTimer;

//Threads
Thread thread1;
Thread thread2;
Mutex lock;

//Mic Input
AnalogIn mic(MIC_AN_PIN);

//Variables for threads
float xn;
float xnStoreN1[205] = {0};
float xnStoreN2[210] = {0};
int storeValueN1 = 0;
int storeValueN2 = 0;

//Variables for maths
float pi = 3.14159265359;
int N[2] = {205, 210};
int k[2][8] = {{18, 20, 22, 24, 31, 34, 38, 42}, {35, 39, 43, 47, 61, 67, 74, 82}}; 

void sampling() {

    while (1) {
        ThisThread::flags_wait_any(1);
        lock.trylock_for(100ms);
        xn = mic.read();
        lock.unlock();
        ThisThread::flags_clear(1);
    }
}

void storing() {

    while (1) {
        ThisThread::flags_wait_any(1);
        if (storeValueN1!=205) {
            lock.trylock_for(10s);
            xnStoreN1[storeValueN1] = xn;
            storeValueN1++;
            lock.unlock();
        }
        if (storeValueN2!=210) {
            lock.trylock_for(10s);
            xnStoreN2[storeValueN2] = xn;
            storeValueN2++;
            lock.unlock();
        }
        ThisThread::flags_clear(1);
    }
}

void samplingISR() {
    thread1.flags_set(1);
}

void storingISR() {
    thread2.flags_set(1);
}

int main(void)
{
    thread1.start(sampling);
    thread2.start(storing);
    sampleTimer.attach(&samplingISR, 125us);
    storingTimer.attach(&storingISR, 125us);

    float xnTemp;

    float vnFir[8];
    float vnNFir[8];
    float vn1Fir[8];
    float vn1NFir[8];
    float vn2Fir[8];
    float vn2NFir[8];
    float vnSec[8];
    float vnNSec[8];
    float vn1Sec[8];
    float vn1NSec[8];
    float vn2Sec[8];
    float vn2NSec[8];
    

    while (1) {
        int kCycle = 0;
        int kCurrent;
        int NCurrent = N[0];

        if (storeValueN1==0) {
            lock.trylock_for(100ms);
            for (int i = 0; i<8; i++) {
                vn1Fir[i] = 0;
                vn2Fir[i] = 0;
            }
            lock.unlock();
        }

        if (storeValueN2==0) {
            lock.trylock_for(100ms);
            for (int i = 0; i<8; i++) {
                vn1Sec[i] = 0;
                vn2Sec[i] = 0;
            }
            lock.unlock();
        }

        if (storeValueN1==205) {
            lock.trylock_for(100ms);
            storeValueN1 = 0;
            lock.unlock();
            for (int i = 0; i<8; i++) {
                vnNFir[i] = vnFir[i];
                vn1NFir[i] = vn1Fir[i];
                vn2NFir[i] = vn2Fir[i];
                cout << "vnNFir " << i << ": " << vnNFir[i] << endl << "vn1NFir " << i << ": " << vn1NFir[i] << endl << "vn2NFir " << i << ": " << vn2NFir[i] << endl << endl;
            }
            //lock.unlock();
            
        }
        
        if (storeValueN2==210) {
            lock.trylock_for(100ms);
            storeValueN2 = 0;
            lock.unlock();
            for (int i = 0; i<8; i++) {
                vnNSec[i] = vnSec[i];
                vn1NSec[i] = vn1Sec[i];
                vn2NSec[i] = vn2Sec[i];
                cout << "vnNFir " << i << ": " << vnNSec[i] << endl << "vn1NFir " << i << ": " << vn1NSec[i] << endl << "vn2NFir " << i << ": " << vn2NSec[i] << endl << endl;
            
            }
            
        }
        lock.trylock_for(100ms);
        while (kCycle<8) {
            xnTemp = xnStoreN1[storeValueN1];
            kCurrent = k[0][kCycle];
            float vn1FirTemp = vn1Fir[kCycle];
            float vn2FirTemp = vn2Fir[kCycle];
            float vnFirTemp = findingV(xnTemp, vn1FirTemp, vn2FirTemp, kCurrent, 205, pi);

            xnTemp = xnStoreN2[storeValueN2];
            kCurrent = k[1][kCycle];
            float vn1SecTemp = vn1Sec[kCycle];
            float vn2SecTemp = vn2Sec[kCycle];
            float vnSecTemp = findingV(xnTemp, vn1SecTemp, vn2SecTemp, kCurrent, 210, pi);
            
            kCycle++;
            vn2Fir[kCycle] = vn1FirTemp;
            vn2Sec[kCycle] = vn1SecTemp;
            vn1Fir[kCycle] = vnFirTemp;
            vn1Sec[kCycle] = vnSecTemp;
        }
        lock.unlock();
    }
}   