/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : Final_project_uc2.c
 * Author     : OUN
 * Created on : Dec 15, 2020
 * description: final project microcontroller 2 source file
 --------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "timers.h"
#include "usart.h"
#include "external_eeprom.h"


/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/


#define DELAY_3_S 	23438 		/* compare match 1 time */
#define DELAY_15_S	39062		/* compare match 3 times */
#define DELAY_1_M	46875		/* compare match 10 times */

/* code messages sent form U-controller to the other one*/
#define I_AM_READY 		'R'		/* U-controller is ready to receive data */
#define EXIST_PASS		'E'		/* EEPROM have a saved password already */
#define CREATE_NEW_PASS '*'		/* the user choice is to create new password */
#define OPEN_DOOR		'#'		/* the user choice is to open the door*/
#define WRONG_BASS		'!'		/* the user entered wrong pass*/

#define PASS_ADDRESS 	0x0311	/* the address of first byte of password in EEPROM */
#define STATUS_ADDRESS	0x0211	/* the address of the flag which is created with creation of new password in EEPROM */
#define MY_ADDRESS		0x01	/* the slave address of this U-controller used in I2C */
#define TWI_FREQUANCY	400000	/* the frequency rate of sending data using I2C */

/*--------------------------------------------------GLOBAL VARIABLES-------------------------------------------------------*/

/* global variable used to count number of compare match interrupt occurrence */
uint8 g_timerCounter = 0;

/* global variable to hold the password taken from EEPROM */
uint8 g_password[6];



/*------------------------------------------------FUNCTIONS PROTOTYPES-----------------------------------------------------*/

/*
 * Description: Function to save the password in EEPROM taken form user
 */
void save_pass(void);

/*
 * Description: Function to turn on motor to open the door and at the end turn it off again
 */
void open_door(void);

/*
 * Description: Function to activate the buzzer for 1 minute
 */
void wrong_pass(void);

/*
 * Description: Function to increase global variable when compare match interrupt occur
 */
void increase_counter(void);

/*
 * Description: Function to turn on timer 1 and freeze the program for 3 seconds
 */
void delay_3s(void);

/*
 * Description: Function to turn on timer 1 and freeze the program for 15 seconds
 */
void delay_15s(void);

/*
 * Description: Function to turn on timer 1 and freeze the program for 1 minute
 */
void delay_1m(void);

/*-----------------------------------------------FUNCTIONS DEFINITIONS-----------------------------------------------------*/


int main(void){
	/*
	 * instruction : variable to hold the instruction taken form the other U-controller
	 * 			     (create new pass, open door, wrong pass)
	 * status	   : variable to hold the flag of the exist of pass in EEPROM or not
	 */
	uint8 instruction, status = 0;

	/* configuration structure of I2C */
	TwiConfig Twi_config = {TWI_FREQUANCY,MY_ADDRESS};
	/* initialize EEPROM with I2C configuration structure */
	EEPROM_init(&Twi_config);
	/* configuration structure of USART */
	Usart_ConfigType usart_config = {NO_PARITY,9600,SINGLE,EIGHT_BITS};
	/* initialize USART configuration structure */
	USART_init(&usart_config);

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* read the flag in EEPROM if there is password or not*/
	EEPROM_readOneByte(STATUS_ADDRESS, &status);

	/* case of exist pass in EEPROM */
	if(status == EXIST_PASS){
		/* wait until U-controller 1 is ready to receive data */
		while(USART_receiveByte() != I_AM_READY){
		}
		/* send the code of exist password in EEPROM */
		USART_sendByte(EXIST_PASS);

		/* get the password saved in EEPROM and hold it in the global variable */
		EEPROM_readBytes(PASS_ADDRESS, g_password, 6);
		/* wait until U-controller 1 is ready to receive data */
		while(USART_receiveByte() != I_AM_READY){
		}
		/* send the password to U-controller 1*/
		USART_sendPass(g_password);
	}
	else{
		/* wait until U-controller 1 is ready to receive data */
		while(USART_receiveByte() != I_AM_READY){
		}
		/* send the code of no password exist and ask the user to create one */
		USART_sendByte(CREATE_NEW_PASS);
	}

	while(1){
		/* clear the variable each time to get another instruction every time */
		instruction = 0;

		/* send the code that U-controller 2 is ready to receive data */
		USART_sendByte(I_AM_READY);
		/* get the instruction form U-controller 1 to handle it*/
		instruction = USART_receiveByte();

		/* switch case to handle the instruction got from user */
		switch (instruction){
		case CREATE_NEW_PASS:
			save_pass();
			break;
		case OPEN_DOOR:
			open_door();
			break;
		case WRONG_BASS:
			wrong_pass();
			break;
		default :
			/* the buzzer will activate unexpectedly if something wrong happened */
			/* configure pin PA0 as output pin */
			DDRA |= 0x01;

			/* activate buzzer */
			PORTA |= (1<<PA0);
		}
	}
}


void save_pass(void){

	/* send the code that U-controller 2 is ready to receive data */
	USART_sendByte(I_AM_READY);

	/* receive the password taken from user and hold it in the global variable */
	USART_receivePass(g_password);

	/* save the password in EEPROM */
	EEPROM_writeBytes(PASS_ADDRESS, g_password, 6);

	/* create the flag in EEPROM indicate that there is password saved in EEPROM */
	EEPROM_writeOneByte(STATUS_ADDRESS, EXIST_PASS);

	/* wait until U-controller 1 is ready to receive data */
	while(USART_receiveByte() != I_AM_READY){
	}
	/* send the code of exist password in EEPROM */
	USART_sendByte(EXIST_PASS);

}
void open_door(void){
	/* configure pin PA1 and PA2 as output pins */
	DDRA |= 0x06;

	/* Rotate the motor --> clock wise */
	PORTA &= (~(1<<PA1));
	PORTA |= (1<<PA2);
	delay_15s();

	/* Stop the motor */
	PORTA &= (~(1<<PA1));
	PORTA &= (~(1<<PA2));
	delay_3s();

	/* Rotate the motor --> anti-clock wise */
	PORTA |= (1<<PA1);
	PORTA &= (~(1<<PA2));
	delay_15s();

	/* Stop the motor */
	PORTA &= (~(1<<PA1));
	PORTA &= (~(1<<PA2));

}
void wrong_pass(void){
	/* configure pin PA0 as output pin */
	DDRA |= 0x01;

	/* activate buzzer */
	PORTA |= (1<<PA0);
	delay_1m();

	/* stop buzzer */
	PORTA &= ~(1<<PA0);

}

void increase_counter(){
	/* increase the global variable each compare match interrupt occur */
	g_timerCounter++;
}

void delay_3s(void){

	/* configuration structure of timer 1 */
	Timer_ConfigType timer_config_3s = {TIMER_1,COMPARE_MATCH,F_CPU_1024,0,DELAY_3_S,0};
	/* set the address of function increase_counter() to call back in ISR */
	Timer_setCallBack(TIMER_1, increase_counter);
	/* initialize timer with configuration structure */
	Timer_init(&timer_config_3s);
	/* wait to g_timerCounter = 1 to freeze the program for 3 seconds */
	while(g_timerCounter < 1){

	}
	/* turn off the timer */
	Timer_deInit(TIMER_1);
	/* clear the global variable*/
	g_timerCounter = 0;
}
void delay_15s(void){

	/* configuration structure of timer 1 */
	Timer_ConfigType timer_config_15s = {TIMER_1,COMPARE_MATCH,F_CPU_1024,0,DELAY_15_S,0};
	/* set the address of function increase_counter() to call back in ISR */
	Timer_setCallBack(TIMER_1, increase_counter);
	/* initialize timer with configuration structure */
	Timer_init(&timer_config_15s);
	/* wait to g_timerCounter = 3 to freeze the program for 15 seconds */
	while(g_timerCounter < 3){

	}
	/* turn off the timer */
	Timer_deInit(TIMER_1);
	/* clear the global variable*/
	g_timerCounter = 0;

}
void delay_1m(void){

	/* configuration structure of timer 1 */
	Timer_ConfigType timer_config_1m = {TIMER_1,COMPARE_MATCH,F_CPU_1024,0,DELAY_1_M,0};
	/* set the address of function increase_counter() to call back in ISR */
	Timer_setCallBack(TIMER_1, increase_counter);
	/* initialize timer with configuration structure */
	Timer_init(&timer_config_1m);
	/* wait to g_timerCounter = 10 to freeze the program for 1 minute */
	while(g_timerCounter < 10){

	}
	/* turn off the timer */
	Timer_deInit(TIMER_1);
	/* clear the global variable*/
	g_timerCounter = 0;

}
