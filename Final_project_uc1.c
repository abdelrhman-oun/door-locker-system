/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : Final_project_uc1.c
 * Author     : OUN
 * Created on : Dec 15, 2020
 * description: final project microcontroller 1 source file
 --------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "timers.h"
#include "usart.h"
#include "lcd.h"
#include "keypad.h"

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


/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/


typedef enum{
	STAR,HASH
}User_MenuChose;


typedef enum{
	UNMATCH,MATCH
}User_Result;


/*--------------------------------------------------GLOBAL VARIABLES-------------------------------------------------------*/

/* global variable to hold the password taken from EEPROM */
uint8 g_currentPass[6];

/* global variable to hold the new password taken from user */
uint8 g_newPass[6];

/* global variable to hold the confirmation password taken from user */
uint8 g_confirmPass[6];

/* global variable to hold the user choice */
User_MenuChose g_menuChose;

/* global variable to hold the result of difference of two passes */
User_Result g_result;

/* global variable to hold the message will appear in screen */
char g_message1[] = "Wrong PASS";
char g_message2[] = "accepted";

/* global variable to hold number of wrong tries */
uint8 g_numOfTries = 0;

/* global variable used to count number of compare match interrupt occurrence */
uint8 g_timerCounter = 0;

/*------------------------------------------------FUNCTIONS PROTOTYPES-----------------------------------------------------*/

/*
 * Description: Function to get the password form the user two times and compare them then send the password to uc 2
 */
void step_1(void);

/*
 * Description: Function to handle main menu options and hold the result in global variable g_menuChose
 */
void step_2(void);

/*
 * Description: Function to check if user entered correct pass or not
 */
void step_3(void);

/*
 * Description: Function to handle the case of opening the door choice
 */
void step_4(void);

/*
 * Description: Function to handle the case of wrong pass in entered three times
 */
void step_5(void);

/*
 * Description: Function to get the password form U-controller 2
 */
void get_password(void);

/*
 * Description: Function to send the password to U-controller 2
 */
void save_password(void);

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


int main (void){

	/* variable to hold the condition if there is pass exist in EEPROM */
	uint8 is_first_time = 0;

	/* configuration structure of USART */
	Usart_ConfigType usart_config = {NO_PARITY,9600,SINGLE,EIGHT_BITS};
	/* initialize USART configuration structure */
	USART_init(&usart_config);

	/* initialize LCD */
	LCD_init();
	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* send the code that U-controller 1 is ready to receive data */
	USART_sendByte(I_AM_READY);
	/* get the flag of existence pass form U-controller 2 to handle it */
	is_first_time = USART_receiveByte();

	/* case of password not exist */
	if(is_first_time == CREATE_NEW_PASS){
		/* go to step 1 to get new pass */
		step_1();
	}
	/* case of password exist */
	else if(is_first_time == EXIST_PASS){
		/* get the password form the other U-controller */
		get_password();
	}

	while(1){
		/* infinite loop to control program flow  */
		step_2();
		step_3();
		if((g_result == UNMATCH) && (g_numOfTries == 3)){
			step_5();
		}
		else if((g_result == MATCH) && (g_menuChose == HASH)){
			step_4();
		}
		else if((g_result == MATCH) && (g_menuChose == STAR)){
			step_1();
		}
	}
}

/*
 * Description: Function to get the password form the user two times and compare them then send the password to uc 2
 */
void step_1(void){
	/* variables to hold the message will appear in screen*/
	char first_message[] = "please enter new pass";
	char second_message[] = "please enter pass";
	char third_message[] = "saved..";
	char message[] = "unmatched";
	/* variable to hold the result of saving pass taken from U-controller 2*/
	uint8 i,status;
	/* variable to hold the result of comparing passes */
	User_Result result_1 = UNMATCH;

	/* loop until two passes are matched */
	while(result_1 == UNMATCH){

		/* send the code to LCD to clear screen*/
		LCD_clearScreen();

		/* display the string on LCD */
		LCD_displayString(first_message);

		LCD_goToRowColumn(1, 0);

		/* loop to get the first pass from user */
		for(i = 0 ; i < 6 ; i++){
			g_newPass[i] = KEYPAD_getPressedKey();
			_delay_ms(500);
			LCD_displayCharacter('*');
		}
		_delay_ms(500);

		/* send the code to LCD to clear screen*/
		LCD_clearScreen();

		/* display the string on LCD */
		LCD_displayString(second_message);

		LCD_goToRowColumn(1, 0);

		/* loop to get the second pass from user */
		for(i = 0 ; i < 6 ; i++){
			g_confirmPass[i] = KEYPAD_getPressedKey();
			_delay_ms(500);
			LCD_displayCharacter('*');
		}
		_delay_ms(500);

		/* loop to compare two passes */
		for(i = 0 ; i < 6 ; i++){
			if(g_newPass[i] != g_confirmPass[i]){
				result_1 = UNMATCH;

				/* send the code to LCD to clear screen*/
				LCD_clearScreen();

				/* display the string on LCD */
				LCD_displayString(message);
				_delay_ms(500);
				break;
			}
			else{
				result_1 = MATCH;
			}
		}
	}
	/* loop to hold the pass in global variable */
	for(i = 0 ; i < 6 ; i++){
		g_currentPass[i] = g_newPass[i];
	}

	/* send the code to LCD to clear screen*/
	LCD_clearScreen();

	/* display the string on LCD */
	LCD_displayString(g_message2);

	/* send the pass to U-controller 2 */
	save_password();

	/* send the code that U-controller 1 is ready to receive data */
	USART_sendByte(I_AM_READY);
	/* get the result of saving process in U-controller 2*/
	status = USART_receiveByte();

	if(status == EXIST_PASS){

		/* send the code to LCD to clear screen*/
		LCD_clearScreen();

		/* display the string on LCD */
		LCD_displayString(third_message);
		_delay_ms(500);
	}
}

/*
 * Description: Function to handle main menu options and hold the result in global variable g_menuChose
 */
void step_2(void){
	/* variables to hold the message will appear in screen*/
	char first_message[] = "* : change pass";
	char second_message[] = "# : open door";
	uint8 chosen;

	/* send the code to LCD to clear screen*/
	LCD_clearScreen();

	/* display the string on LCD */
	LCD_displayString(first_message);

	/* change cursor location */
	LCD_goToRowColumn(1, 0);

	/* display the string on LCD */
	LCD_displayString(second_message);

	/* get user choice */
	chosen = KEYPAD_getPressedKey();
	_delay_ms(500);

	/* switch case to handle user choice */
	switch(chosen){
	case '*':
		/* change global variable to hold user choice*/
		g_menuChose = STAR;
		break;
	case '#':
		/* change global variable to hold user choice*/
		g_menuChose = HASH;
		break;
	}

}

/*
 * Description: Function to check if user entered correct pass or not
 */
void step_3(void){
	/* variables to hold the message will appear in screen*/
	char first_message[] = "please enter pass";
	char enterd_bass[6];
	uint8 i;

	g_result = UNMATCH;
	/* clear the global variable */
	g_numOfTries = 0;

	/* loop until result is matched or three times wrong pass*/
	while(g_result == UNMATCH){

		/* increase number of tries every time */
		g_numOfTries++;

		/* send the code to LCD to clear screen*/
		LCD_clearScreen();
		/* display the string on LCD */
		LCD_displayString(first_message);
		LCD_goToRowColumn(1, 0);
		/* loop to get the pass from user */
		for(i = 0 ; i < 6 ; i++){
			enterd_bass[i] = KEYPAD_getPressedKey();
			_delay_ms(500);
			LCD_displayCharacter('*');
		}
		/* loop to check if the entered pass is correct or not */
		for(i = 0 ; i < 6 ; i++){
			if(enterd_bass[i] != g_currentPass[i]){
				g_result = UNMATCH;
				/* send the code to LCD to clear screen*/
				LCD_clearScreen();
				/* display the string on LCD */
				LCD_displayString(g_message1);
				_delay_ms(500);
				break;
			}
			else{
				g_result = MATCH;
			}
		}

		if(g_numOfTries == 3){
			break;
		}

	}

	if(g_result == MATCH){
		g_numOfTries=0;
	}

}

/*
 * Description: Function to handle the case of opening the door choice
 */
void step_4(void){
	/* variables to hold the message will appear in screen*/
	char first_message[] = "door is opening";
	char second_message[] = "door opened";
	char third_message[] = "door is closing";

	/* wait until U-controller 2 is ready to receive data */
	while(USART_receiveByte() != I_AM_READY){
	}
	/* send the code to open the door in U-controller 2 */
	USART_sendByte(OPEN_DOOR);

	/* send the code to LCD to clear screen*/
	LCD_clearScreen();
	/* display the string on LCD */
	LCD_displayString(first_message);
	/* freeze the system for 15 seconds */
	delay_15s();
	/* send the code to LCD to clear screen*/
	LCD_clearScreen();
	/* display the string on LCD */
	LCD_displayString(second_message);
	/* freeze the system for 3 seconds */
	delay_3s();
	/* send the code to LCD to clear screen*/
	LCD_clearScreen();
	/* display the string on LCD */
	LCD_displayString(third_message);
	/* freeze the system for 15 seconds */
	delay_15s();

}

/*
 * Description: Function to handle the case of wrong pass in entered three times
 */
void step_5(void){
	/* variables to hold the message will appear in screen*/
	char first_message[] = "system is locked";

	/* wait until U-controller 2 is ready to receive data */
	while(USART_receiveByte() != I_AM_READY){

	}
	/* send the code of wrong bass is entered three times */
	USART_sendByte(WRONG_BASS);

	/* send the code to LCD to clear screen*/
	LCD_clearScreen();
	/* display the string on LCD */
	LCD_displayString(first_message);
	/* freeze the system for 1 minute */
	delay_1m();

}

/*
 * Description: Function to get the password form U-controller 2
 */
void get_password(void){

	/* send the code that U-controller 1 is ready to receive data */
	USART_sendByte(I_AM_READY);
	/* get the password from U-controller 2*/
	USART_receivePass(g_currentPass);

}

/*
 * Description: Function to increase global variable when compare match interrupt occur
 */
void save_password(void){

	/* wait until U-controller 2 is ready to receive data */
	while(USART_receiveByte() != I_AM_READY){

	}
	/* send the code to change the password in EEPROM */
	USART_sendByte(CREATE_NEW_PASS);

	/* wait until U-controller 2 is ready to receive data */
	while(USART_receiveByte() != I_AM_READY){

	}
	/* send the new pass to U-controller 2 to save it in EEPROM*/
	USART_sendPass(g_currentPass);
}


/*
 * Description: Function to increase global variable when compare match interrupt occur
 */
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
