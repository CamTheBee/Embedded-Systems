#include "DoubleNumber.hpp"
#include "ComplexNumber.hpp"

int main()
{
    //Constructor Function overloading 
    ComplexNumber c0(1.0, 2.0);
    DoubleNumber  d0(3.0);
    ComplexNumber c1;
    DoubleNumber  d1;
    
    //This uses the operator= function
    d1 = 5.0;

    //Write the value as a string
    cout << d1.asString() << endl;

    //Now overrides the default =
    c1 = c0;

    //Now overrides the parent asString
    cout << c1.asString() << endl;

    //Challenge - try and understand this ;)
    cout << c1.asDoubleNumber().asString() << endl; //It takes the complex number, converts it to a DoubleNumber (only takes the imag part) and then converts it to a string to be printed out.

    //TODO - write an overloaded + operator
    DoubleNumber sum1 = c0 + c1 + d0 + d1; //Now adds the real parts of the complexNumber to the DoubleNumbers.
    cout << sum1 << endl;

    ComplexNumber sum2 = c0 + c1;
    cout << sum2.realPart() << sum2.imagPart() << endl;
    while (true) {

    }
}

//Part 2 - Parent Class = DoubleNumber; Child Class = ComplexNumber.