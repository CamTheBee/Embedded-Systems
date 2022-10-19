#include "mbed.h"
#include "ComplexNumber.hpp"

// main() runs in its own thread in the OS
int main()
{
    ComplexNumber c1(2.0,3.0);
    ComplexNumber c2(1.0,-1.0);
    ComplexNumber c1_copy = c1;
//    c1.add(c2);   //c1 = c1 + c2
//    c1.display();
//    c1_copy.display();
    
    ComplexNumber c3 = c1.addedTo(c2);
    c3.display();
//    c3.conjugate();
//    c3.display();
   ComplexNumber s = c3.conjugated();
    s.display();
    //Create new scope
//    {
//       ComplexNumber test(1.0);
//        test.display();
//   }
    //test is now out of scope and should be destroyed

    //Challenge
    ComplexNumber neg = c1.negated();
    neg.display();
    //c1.subtract(c2);
    //c1.display();
    //c1 = c1.subtractedTo(c2);
    //c1.display();
    //c1.multiply(4);
    //c1.display();
    //c1 = c1.multiplyWith(c2);
    //c1.display();
//    c1.divide(2);
//    c1.display();
//    ComplexNumber c4 = c1.divideWith(c2);
//    c4.display();
    while (true);
}

/*
Part 6 - When copy-constructor is commented out, the results are the same.

Part 7 - c is the temperory function and this is the pointer which points to the address of the variable wanted to be added (c1).

Part 10 - The destructor was prompted by ther exiting of the test.display(); leaving scope. 

*/