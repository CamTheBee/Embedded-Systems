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
	PLL_Config();
	SystemCoreClockUpdate();
	SysTick_Init();
	init_usart(115200);		//set to use 115200 baud
	
	
	ADC_Init();
	init_led();
	init_pwm(10000);
	
	
	
	unsigned short n=0;
	////////////////////////////////
	int32_t prevVoltage = convert_PA0;
	volatile int32_t voltage = 0;
	volatile int32_t changedVoltage = 0;
	int32_t prevCurrent = convert_PA1;
	volatile int32_t current = 0;
	volatile int32_t changedCurrent = 0;
	
	volatile float changedcal = 0.0f;
	volatile float actualcal = 0.0f;
	
	int stage = 1;
	////////////////////////////////
	char term_msg[128];
	
	while(1)
	{
		delay_nms(100);
		led_on();
		//--------------------------------------------
		//MPPT ALGORITHM BEGIN
		voltage = convert_PA0;
		current = convert_PA1;
		
		sprintf(term_msg,"Voltage	Value: %d\n\r", voltage);
		print_terminal(term_msg);
		
		sprintf(term_msg,"Current Value: %d\n\r", current);
		print_terminal(term_msg);
		
		changedVoltage = voltage - prevVoltage;
		changedCurrent = current - prevCurrent;
		
		sprintf(term_msg,"Delta Voltage	Value: %d\n\r", changedVoltage);
		print_terminal(term_msg);
		
		sprintf(term_msg,"Delta Current Value: %d\n\r", changedCurrent);
		print_terminal(term_msg);
		
		switch (stage) {
		
			//Checking change of voltage
			case 1:
				if (changedVoltage == 0) {
					stage = 4;
				}
				else {
					stage = 2;
					changedcal = (float)changedCurrent/(float)changedVoltage;
					actualcal = (float)current/(float)voltage;
				}

				
			//Checking if changecal=-actualcal
			case 2:
				if (changedcal == -(actualcal)) { //might need changing
					stage = 1;
					break;
				}
				else {
					stage = 3;		
				}
				
			//Checking if changecal>-actualcal
			case 3:
				if (changedcal > -(actualcal)) { //Might need changing
					if (n != 100) {
						output_pwm((float)(n++)); //Might need changing
						print_terminal("Duty Cycle increased by 0.1!\n\r");
					}
					else {
						print_terminal("Duty Cycle is at maximum!\n\r");
					}
				}
				else {
					if (n != 0) {
						output_pwm((float)(n--)); //Might need changing
						print_terminal("Duty Cycle decreased by 0.1!\n\r");
					}
					else {
					print_terminal("Duty Cycle is at minimum!\n");
					}
				}
				stage = 1;
			break;	
				
			//Checking if delta current is equal to 0.
			case 4:
				if (changedCurrent == 0) {
					stage = 1;
					break;
				}
				else {
					stage = 5; 
				}
				
			//Checking if delta current is greater than 0.
			case 5:
				if (changedCurrent > 0) {
					if (n != 100) {
						output_pwm((float)(n++)); //Might need changing
						print_terminal("Duty Cycle increased by 0.1!\n\r");
					}
					else {
						print_terminal("Duty Cycle is at maximum!\n");
					}
				}
				else {
					if (n != 0) {
						output_pwm((float)(n--)); //Might need changing
						print_terminal("Duty Cycle decreased by 0.1!\n\r");
					}
					else {
						print_terminal("Duty Cycle is at minimum!\n");
					};
				}
			break;
			
				/*
			default:
					stage = 1;
			break;
				*/
		}
		//WRITE YOUR CODE HERE
		
		delay_nms(10);
		//output_pwm((float)(n++));
		//if(n==100)n=0;
		
		
		
		//Saving previous terms
		prevVoltage = voltage;
		prevCurrent = current;
		
		
		//--------------------------------------------
		//MPPT ALGORITHM END
		led_off();
	
	
	}//end while
}
