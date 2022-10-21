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

    //DANGER! THIS IS NOT AS IT LOOKS
    //By default, the compiler creates an = operator!
    c1 = c0;

    //Write as a string, but note asString() comes from the parent class
    cout << c1.asString() << endl;
    

    //The plus will work, but it will be the DoubleNumber version
    cout << (c0 + c1 + d0 + d1).asString();

    // Note how this works with both types, but only on the real parts
    DoubleNumber sum1 = c0 + c1 + d0 + d1;
    cout << sum1 << endl;

    while (true) {

    }
}

//Part 3 - The complex number sub-class has the addition of a imagary double parameter which is protected so only the parent and child class can see it.
//Part 4 - The sub-class ComplexNumber gets called but it calls DoubleNumber before running its constructor.
//Part 5 - asString() is part of the DoubleNumber class.
//Part 6 - Because when the + operator is called, it only uses the DoubleNumber class version which has no inclusion of imagnary numbers. A + operator overload will need to be made for the ComplexNumber sub-class to allow for this.