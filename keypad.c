/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : keypad.c
 * Author     : OUN
 * Created on : Oct 31, 2020
 * description: keypad source file driver for 4x3 and 4x4 keypad
 --------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "keypad.h"

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

#if (NO_COL == 3)
static uint8 KEYPAD_4x3_adjustSwitchNumber(uint8 key);
#elif (NO_COL == 4)
static uint8 KEYPAD_4x4_adjustSwitchNumber(uint8 key);
#endif

/*-------------------------------------------------FUNCTION DEFINITION-----------------------------------------------------*/

uint8 KEYPAD_getPressedKey(void){
	/* the usage of these variables to loop for columns and rows */
	uint8 col,row;

	while(1){

		for (col = 0 ; col < NO_COL ; col++){  /* loop for columns and put 0 in each column to find which button is pressed*/
			#if (NO_COL == 3)

			REG_PORT_DIR = (REG_PORT_DIR & (0X80)) | (0b00010000<<col); /* set only one bin as output pin*/
			REG_PORT_OUT = (REG_PORT_OUT | (0X7F)) & (~(0b00010000<<col));

			#elif (NO_COL == 4)
			REG_PORT_DIR = (0b00010000<<col); /* set only one bin as output pin*/
			REG_PORT_OUT = (~(0b00010000<<col));
			#endif
			for(row = 0 ; row < NO_ROW ; row++){
				if(BIT_IS_CLEAR(REG_PORT_IN,row)){
					#if (NO_COL == 3)
					return KEYPAD_4x3_adjustSwitchNumber((NO_COL*row)+col+1);
					#elif (NO_COL == 4)
					return KEYPAD_4x4_adjustSwitchNumber((NO_COL*row)+col+1);
					#endif
				}
			}
		}
	}
	return col;
}
#if (NO_COL == 3)
static uint8 KEYPAD_4x3_adjustSwitchNumber(uint8 key){
	switch (key){
	case 10:
		return '*';
		break;
	case 11:
		return 0 ;
		break;
	case 12:
		return '#';
		break;
	default :
		return key;
	}
}
#elif (NO_COL == 4)
static uint8 KEYPAD_4x4_adjustSwitchNumber(uint8 key)
{
	switch(key)
	{
	case 1:
		return 7;
		break;
	case 2:
		return 8;
		break;
	case 3:
		return 9;
		break;
	case 4:
		return '%'; // ASCII Code of %
		break;
	case 5:
		return 4;
		break;
	case 6:
		return 5;
		break;
	case 7:
		return 6;
		break;
	case 8:
		return '*'; /* ASCII Code of '*' */
		break;
	case 9:
		return 1;
		break;
	case 10:
		return 2;
		break;
	case 11:
		return 3;
		break;
	case 12:
		return '-'; /* ASCII Code of '-' */
		break;
	case 13:
		return 13;  /* ASCII of Enter */
		break;
	case 14:
		return 0;
		break;
	case 15:
		return '='; /* ASCII Code of '=' */
		break;
	case 16:
		return '+'; /* ASCII Code of '+' */
		break;
	default: return key;
	}
}
#endif




