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
using namespace uop_msb;






float findingV(float xn, float vn1, float vn2, int kCurrent, int NCurrent, int pi) {
    float vntemp = (((2*cos((2*pi*kCurrent/NCurrent)))*vn1) - vn2 + xn);  
    return vntemp;
}