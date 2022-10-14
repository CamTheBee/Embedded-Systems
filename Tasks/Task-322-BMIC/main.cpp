#include "mbed.h"
#include "Human.h"

int main(int argc, const char * argv[]) {


    // ***************
    // USING THE C API
    // ***************
   
    //Designated initialiser
    Human person = initHuman(45, 97, 1.9); //Value Semantics
    displayHuman(&person);
    
    //After a major lifestyle change
    updateWeight(&person, 75.0);
    displayHuman(&person);
    return 0;
}

//Reference Semantics - Is when you take the address of a varaible and point to it and edit the varaible directly via the address.
//Value Semantics - Is when you take a copy of a variable and edit the copy, then go back to the varaible.
//bmi cannot be modified as it is private.
