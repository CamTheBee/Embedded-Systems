//Included header files from sample code
#include <stm32f3xx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "adc.h"
#include "leds.h"
#include "usart.h"
#include "systick_delay.h"
#include "PLL_Config.h"
#include "pwm.h"



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//ELEC 344 EXAMPLE CODE FOR MPPT
//BY STUART MACVEIGH		14/02/2023
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//REMEMBER TO TO KEEP ALL FILES TOGETHER, WHEN TRANSPORTING FILES BETWEEN PCs KEEP THEM ALL IN THE FOLDER!!


//ANALOG INPUTS - the two analogue input signals should be connected to PA0 and PA1
//call the "convert_PA0" and "convert_PA1" functions to read analogue values from PA0 and PA1 respectively
//an example is given in the loop below

//PWM OUTPUT - 
//signal is output from PB0
//The pwm frequency is set by parsing the value in Hz into the "init_pwm" function as shown in the init section below
//to control the pwm output, call the "output_pwm" function and parse the desired duty cycle in percent from 0 to 100.

//ON-BOARD LED - call the "led_on" and "led_off" functions to turn the led on and off respectively

//DELAYS - call the "delay_nms" function and parse the number of milliseconds to delay

//TERMINAL OUTPUT - call the "print_terminal" function and parse a string to print it to the PC terminal 
//numeric values can be formatted to a string using the "sprintf" function (an example is shown in the loop below)



int main(void)
{
	//Configuration included in the sample code.
	PLL_Config();
	SystemCoreClockUpdate();
	SysTick_Init();
	init_usart(115200);		//set to use 115200 baud
	ADC_Init();
	init_led();
	

	init_pwm(100000); //Set at 100kHz - Justified
	
	//START OF STUDENT WRITTEN CODE//
	
	//Counter and total for sampling
	int sampleCounter = 0;
	int total = 1000;
	
	
<<<<<<< Updated upstream
	unsigned short n=50;
	output_pwm((float)(n));
	////////////////////////////////
=======
	unsigned short dutyCycle=0; //Duty Cycle value x100
	
	//Variables for voltages
>>>>>>> Stashed changes
	//int32_t prevVoltage = convert_PA0;
	volatile int32_t prevVoltage = 0;
	volatile int32_t voltage = 0;
	volatile int32_t tempVoltage = 0;
	volatile int32_t deltaVoltage = 0;
	
	//Variables for current
	//int32_t prevCurrent = convert_PA1;
	volatile int32_t prevCurrent = 0;
	volatile int32_t current = 0;
	volatile int32_t tempCurrent = 0;
	volatile int32_t deltaCurrent = 0;
	
	
	//Variables for conductance values
	volatile float deltaConductance = 0.0f;
	volatile float currentConductance = 0.0f;
	
	//Counter for state machine 
	int stage = 1;

	//Message array for printing to the terminal
	char term_msg[128];
	
	
	//Start of forever while loop//
	while(1)
	{
<<<<<<< Updated upstream
		delay_nms(100000);
		led_on();
=======
>>>>>>> Stashed changes
		
		delay_us(1000000); //Initial 1 second delay to prevent instability
		led_on(); //Switch the onboard LED on to signal sampling
		
		
		//Sampling while loop
		while (sampleCounter<total) {
			
			tempVoltage = convert_PA0 + tempVoltage; //Voltage measured and summed
			tempCurrent = convert_PA1 + tempCurrent; //Current measured and summed
			sampleCounter++; //Add one to the counter
			delay_us(5); //Delay of 5us, double the PWM frequency of 100kHz.
			
		} //End of sampling while loop
		
		//Current voltage and current calculated from mean
		voltage = tempVoltage/total;
		current = tempCurrent/total;
		
		//Reset of sampling parameters
		sampleCounter = 0;
		tempVoltage = 0;
		tempCurrent = 0;
		
		led_off(); //LED switched off to signal sampling finished
		
		
		//Voltage value printed to the terminal
		sprintf(term_msg,"Voltage	Value: %d\n\r", voltage);
		print_terminal(term_msg);
		
		//Current value printed to the terminal
		sprintf(term_msg,"Current Value: %d\n\r", current);
		print_terminal(term_msg);
		
		//Delta voltage and current values calculated
		deltaVoltage = voltage - prevVoltage;
		deltaCurrent = current - prevCurrent;
		
		//Delta Voltage value printed to the terminal
		sprintf(term_msg,"Delta Voltage	Value: %d\n\r", deltaVoltage);
		print_terminal(term_msg);
		
		//Delta Current value printed to the terminal
		sprintf(term_msg,"Delta Current Value: %d\n\r", deltaCurrent);
		print_terminal(term_msg);
		
		
		//Start of switch case loop
		switch (stage) {
		
			//Checking delta voltage is equal to zero
			case 1:
<<<<<<< Updated upstream
				if (changedVoltage == 0) {
					stage = 4;
				}
				else {
					stage = 2;
					changedcal = (float)changedCurrent/(float)changedVoltage;
					actualcal = (float)current/(float)voltage;
					
				}

=======
>>>>>>> Stashed changes
				
				if (deltaVoltage == 0) {
					stage = 4; //Current check state (Route 2)
				}
				
				else {
					stage = 2; //Next check state (Route 1)
					
					//Conductance calculations required as delta voltage has changed
					deltaConductance = (float)deltaCurrent/(float)deltaVoltage;
					currentConductance = (float)current/(float)voltage;
					
				}
			
			//End of case 1
				
			
			//Route 1//
			//Checking if the delta conductance is equal to the current conductance (Is MMP achieved)
			case 2:
				
				if (deltaConductance == -(currentConductance)) {
					
					stage = 1; //Reset state
					
					break; //No change to duty cycle required so break from case loop and return to sampling
					
				}
				
<<<<<<< Updated upstream
			//Checking if changecal>-actualcal
			case 3:
				if (changedcal > -(actualcal)) { //Might need changing
					if (n != 75) {
						output_pwm((float)(n++)); //Might need changing
						print_terminal("Duty Cycle decreased by 0.01!\n\r");
					}
					else {
						print_terminal("Duty Cycle is at minimum!\n\r");
					}
				}
				else {
					if (n != 25) {
						output_pwm((float)(n--)); //Might need changing
						print_terminal("Duty Cycle increased by 0.01!\n\r");
=======
				else {
					stage = 3; //Next check state as change required		
				}
				
			//End of case 2
				
				
			//Checking if delta conductance has gotten bigger or smaller than the current conductance (Which side of P-V curve)
			case 3:
				
				if (deltaConductance > -(currentConductance)) {
					//If entered so on left side of the curve
					
					if (dutyCycle < 80) { //If statement to check if the duty cycle is currently at its maximum
						//If entered so duty cycle is not at maximum and can be increased
						
						output_pwm((float)(dutyCycle++)); //Duty cycle increased
						
						print_terminal("Duty Cycle increased by 0.01!\n\r"); //Action printed to terminal
						
>>>>>>> Stashed changes
					}
					
					else {
						//Else entered so duty cycle is at maximum, print result to terminal
						print_terminal("Duty Cycle is at maximum!\n\r");	
					}
					
				}
				
				else {
					//Else entered so on right side of the curve
					
					if (dutyCycle > 20) { //If statement to check if the duty cycle is currently at its minimum
						//If entered so duty cycle is not at minimum and can be decreased
						
						output_pwm((float)(dutyCycle--)); //Duty cycle decreased
						
						print_terminal("Duty Cycle decreased by 0.01!\n\r"); //Action printed to terminal
						
					}
					
					else {
						//Else entered so duty cycle is at minimum, print result to terminal
					print_terminal("Duty Cycle is at maximum!\n");
					}
					
				}
				
				stage = 1; //Reset state
				
				break;	//Duty cycle has been changed so break from case loop and return to sampling
			
			//End of case 3
				
			
			//Route 2//
			//Checking if delta current is equal to 0 (If true, then MPP still achieved)
			case 4:
				
				if (deltaCurrent == 0) {
					//If stated entered so no change and MPP still achieved
					
					stage = 1; //Reset state
					
					break; //No change to duty cycle required so break from case loop and return to sampling
					
				}
				
				else {
					//Else entered so the current has changed.
					
					stage = 5; //Next state as changed required
					
				}
				
				//End of case 4
				
				
			//Checking if delta current has gotten bigger or smaller than the zero (Which side of P-V curve)
			case 5:
<<<<<<< Updated upstream
				if (changedCurrent > 0) {
					if (n != 75) {
						output_pwm((float)(n++)); //Might need changing
						print_terminal("Duty Cycle decreased by 0.01!\n\r");
=======
				
				if (deltaCurrent > 0) {
					//If entered so delta current has gotten bigger than zero. On the let side of the curve
					
					if (dutyCycle < 80) { //If statement to check if the duty cycle is currently at its maximum
						//If entered so duty cycle is not at maximum and can be increased
						
						output_pwm((float)(dutyCycle++)); //Duty cycle increased
						
						print_terminal("Duty Cycle increased by 0.01!\n\r"); //Print action to terminal
						
>>>>>>> Stashed changes
					}
					
					else {
						//Else entered so duty cycle is at maximum, print result to terminal
						print_terminal("Duty Cycle is at maximum!\n");
					}
					
				}
				
				else {
					if (dutyCycle > 20) { //If statement to check if the duty cycle is currently at its minimum
						//If entered so duty cycle is not at minimum and can be decreased
						
						output_pwm((float)(dutyCycle--)); //Duty cycle decreased
						
						print_terminal("Duty Cycle decreased by 0.01!\n\r"); //Print action to terminal
					}
					
					else {
						//Else entered so duty cycle is at minimum, print result to terminal
						print_terminal("Duty Cycle is at minimum!\n");
					}
					
				}
<<<<<<< Updated upstream
				else {
					if (n != 25) {
						output_pwm((float)(n--)); //Might need changing
						print_terminal("Duty Cycle increased by 0.01!\n\r");
					}
					else {
						print_terminal("Duty Cycle is at maximum!\n");
					};
				}
			break;
		}
		//WRITE YOUR CODE HERE
		//delay_nms(10);
		sprintf(term_msg,"Duty Cycle: %d\n\n\r", n);
=======
				
				stage = 1; //Reset state
				
				break;	//Duty cycle has been changed so break from case loop and return to sampling
				
				//End of case 5
				
		} //End of case loop
		
		
		//Print current duty cycle to the terminal
		sprintf(term_msg,"Duty Cycle: %d\n\n\r", dutyCycle);
>>>>>>> Stashed changes
		print_terminal(term_msg);
		
		
		//Store previous current and voltage terms
		prevVoltage = voltage;
		prevCurrent = current;
		
	}//end while
	
} //End of main
