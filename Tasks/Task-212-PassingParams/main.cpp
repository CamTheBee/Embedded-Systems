#include "mbed.h"
#include "Complex_C.hpp"

// TASK - write and test complexConjugate, complexNegate, complexSubtract, complexMagnitude, complexMultiply and complexDivide

int main() {
    printf("\n\nTASK312\n");

    //Create instance of a complex number
    ComplexNumber_C p = {2.0, 3.0};
    ComplexNumber_C q = {1.0, 1.0};
    complexDisplay("p", p);
    complexDisplay("q", q);
 
    ComplexNumber_C sum = complexAdd(p, q);
    complexDisplay("p+q", sum);

    ComplexNumber_C conj = complexConjugate(p);
    complexDisplay("p", conj);
    
    ComplexNumber_C neg = complexNegate(p);
    complexDisplay("p", neg);

    ComplexNumber_C sub = complexSubtract(p,q);
    complexDisplay("p-q", neg);

    ComplexNumber_C mag = complexMagnitude(p);
    complexDisplay("p*p", mag);

    ComplexNumber_C mult = complexMultiplication(p,q);
    complexDisplay("p*q", mult);

    ComplexNumber_C div = complexDivision(p,q);
    complexDisplay("p/q", div);

    complexNegateInplace(p);
    complexDisplay("p", p);

    complexConjugateInplace(p);
    complexDisplay("p", p);
    while (true) {
    }
}
