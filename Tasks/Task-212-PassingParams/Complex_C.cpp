#include "mbed.h"
#include "Complex_C.hpp"

ComplexNumber_C complexAdd(const ComplexNumber_C a, const ComplexNumber_C b) {
    ComplexNumber_C y = a;
    y.real += b.real;
    y.imag += b.imag;
    return y;
}

ComplexNumber_C complexConjugate(const ComplexNumber_C a) {
    ComplexNumber_C y = a;
    y.imag *= - 1.0;
    return y;
}

ComplexNumber_C complexNegate(const ComplexNumber_C a) {
    ComplexNumber_C y = a;
    y.real *= - 1.0;
    y.imag *= - 1.0;
    return y;
}

ComplexNumber_C complexSubtract(const ComplexNumber_C a, const ComplexNumber_C b) {
    ComplexNumber_C y = b;
    y.real *= - 1.0;
    y.imag *= - 1.0;
    y.real += a.real;
    y.imag += a.imag;
    return y;
}

ComplexNumber_C complexMagnitude(const ComplexNumber_C a) {
    ComplexNumber_C y = a;
    y.real *= y.real;
    y.imag *= y.imag;
    return y;
}

ComplexNumber_C complexMultiplication(const ComplexNumber_C a, const ComplexNumber_C b) {
    ComplexNumber_C y = a;
    ComplexNumber_C x = a;
    
    //First Bracket
    y.real *= b.real;
    y.imag *= b.imag;
    y.real -= y.imag;

    //Second Bracket
    x.real *= b.imag;
    x.imag *= b.real;
    x.real += x.imag;

    //Final Add
    y.real += x.real;
    y.imag += x.imag;
    return y;
}

ComplexNumber_C complexDivision(const ComplexNumber_C a, const ComplexNumber_C b) {
    ComplexNumber_C y;
    ComplexNumber_C aa = a;
    ComplexNumber_C bb = b;
    ComplexNumber_C aaj = a;
    ComplexNumber_C squ = b;


    //Real Top Fraction - y.real should equal
    aa.real *= bb.real;
    aa.imag *= bb.imag;
    aa.real += aa.imag;
    y.real = aa.real;

    //Squaring of bottom fraction terms.
    squ.real *= squ.real;
    squ.imag *= squ.imag;
    squ.real += squ.imag;

    //y.real element
    y.real /= squ.real;


    //Imag Top Fraction - y.imag should equal
    aaj.imag *= bb.real;
    aaj.real *= bb.imag;
    aaj.real -= aa.imag;
    y.imag = aaj.real;

    //y.imag element
    y.imag /= squ.real;
    return y;
}

void complexNegateInplace(ComplexNumber_C& y) {
    y.imag = -y.imag;
    y.real = -y.real;
}

void complexConjugateInplace(ComplexNumber_C& y) {
    y.imag *= - 1.0;
}

void complexDisplay(const char *strName, const ComplexNumber_C u) {
    printf("%s = %f + j%f\n", strName, u.real, u.imag);
}
