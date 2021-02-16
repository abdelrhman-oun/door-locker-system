
/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : lcd.h
 * Author     : OUN
 * Created on : Nov 1, 2020
 * description: lcd header file driver
 --------------------------------------------------------------------------------------------------------------------------*/


#ifndef LCD_H_
#define LCD_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/

#define DATA_BITS_MODE 8

#if (DATA_BITS_MODE == 4)
#define LOWER_PORT_PINS
#endif

/* LCD hardware pins*/
#define LCD_CTRL_PORT_DIR DDRB
#define LCD_CTRL_PORT_OUT PORTB
#define RS PB0
#define RW PB1
#define E  PB2

#define LCD_DATA_PORT_DIR DDRC
#define LCD_DATA_PORT_OUT PORTC

/* LCD COMMANDS*/
#define CLEAR_SCREEN        		0X01
#define RETURN_HOME         		0X02
#define SHIFT_CURSOR_LEFT   		0X04
#define SHIFT_CUTSOR_RIGHT  		0X06
#define SHIFT_DISPLAY_LEFT 			0X07
#define SHIFT_DISPLAY_RIGHT 		0X05
#define SHIFT_ENTIRE_DISPLAY_LEFT   0X18
#define SHIFT_ENTIRE_DISPLAY_RIGHT  0X1C
#define CURSOR_ON         		    0X0E
#define CURSOT_OFF          		0X0C
#define SET_CURSOR_LOCATION 		0x80
#define FOUR_BITS_DATA_MODE         0x02
#define TWO_LINE_LCD_FOUR_BIT_MODE  0x28
#define TWO_LINE_LCD_EIGHT_BIT_MODE 0x38

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_goToRowColumn(uint8 row, uint8 col);
void LCD_displayCharacter(uint8 character);
void LCD_displayCharacterRowColumn(uint8 row,uint8 col, uint8 character);
void LCD_displayString(const char *string);
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string);
void LCD_intgerToString(int data);
void LCD_clearScreen(void);
void LCD_clearRow(uint8 row);
void LCD_clearRowColumn(uint8 row, uint8 col);






#endif /* LCD_H_ */
