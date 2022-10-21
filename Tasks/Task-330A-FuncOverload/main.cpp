#include "mbed.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>

using namespace std;

class DoubleNumber {
private:

protected:
    double _real;

public:
    //Designated constructor
    DoubleNumber(double r) {
        _real = r;
        cout << "This is the constructor of Base" << endl;
    }

    //Convenience constructor
    DoubleNumber() : DoubleNumber(0.0) { }

    //Magnitude
    double magnitude() {
        return fabs(_real);
    }

    //Three overloaded functions
    void setValue(double u) {
        _real = u;
    }
    void setValue(int u) {
        _real = (double)u;
    }
    void setValue(string strVal) {
        _real = stod(strVal); //Converts string to a double.
    }

    //Part 7
    void setValue(const DoubleNumber& d) { //Allows the user to set a DoubleNumber as a DoubleNumber like n0.setValue(n1);.
        _real = d._real;
    }
    double getValue() {
        return _real;
    }

    string asString() {
        return to_string(_real);
    }
};


int main()
{
    //Constructor Function overloading 
    DoubleNumber n0(1.0); //Calls Designed Constructor as there is a parameter being put in to the function. //Calls the double set value as a double is called.
    DoubleNumber n1; //Calls the Convenience Constructor as no paramter being put in to the function; so it will be set to zero.
    DoubleNumber n2; //Calls the Convenience Constructor as no paramter being put in to the function; so it will be set to zero.
    
    //setValue function overloading  
    //n1.setValue(10); //Calls the int set value as a int is called.
    n0.setValue(n1); //Part 6 - Causes an error as there is currently no matching member function. I.e. get value doesn't look for a DoubleNumber function.
    n2.setValue("-3.0"); //Calls the string set value as a string is called.

    cout << n0.getValue() + n1.getValue() + n2.getValue() << endl;
    cout << n0.getValue() << endl; //Prints the value of n0.
    while (true) {

    }
}

