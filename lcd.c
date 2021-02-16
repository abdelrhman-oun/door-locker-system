/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : lcd.c
 * Author     : OUN
 * Created on : Nov 1, 2020
 * description: lcd source file driver
 --------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "lcd.h"

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

void LCD_init(void){
	LCD_CTRL_PORT_DIR |= (1<<RS) | (1<<RW) | (1<<E);

	#if (DATA_BITS_MODE == 8)
		LCD_DATA_PORT_DIR = 0XFF;
		LCD_sendCommand(TWO_LINE_LCD_EIGHT_BIT_MODE);
	#elif (DATA_BITS_MODE == 4)
		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_DIR |= 0X0F;
		#else
			LCD_DATA_PORT_DIR |= 0XF0;
		#endif
		LCD_sendCommand(FOUR_BITS_DATA_MODE);
		LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
	#endif

	LCD_sendCommand(CURSOT_OFF);
	LCD_sendCommand(CLEAR_SCREEN);
}


void LCD_sendCommand(uint8 command){
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RS);
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT_OUT,E);
	_delay_ms(1);
	#if (DATA_BITS_MODE == 8)

		LCD_DATA_PORT_OUT = command;
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);

	#elif (DATA_BITS_MODE == 4)
		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | ((command & 0xF0) >> 4);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | (command & 0xF0);
		#endif

			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
			_delay_ms(1);
			SET_BIT(LCD_CTRL_PORT_OUT,E);
			_delay_ms(1);

		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | (command & 0x0F);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | ((command & 0x0F) << 4);
		#endif

			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
			_delay_ms(1);

	#endif


}

void LCD_goToRowColumn(uint8 row,uint8 col){

	switch(row)
	{
	case 0:
		LCD_sendCommand( col | SET_CURSOR_LOCATION);
		break;
	case 1:
		LCD_sendCommand( (col+0x40) | SET_CURSOR_LOCATION);
		break;
	case 2:
		LCD_sendCommand( (col+0x10) | SET_CURSOR_LOCATION);
		break;
	case 3:
		LCD_sendCommand( (col+0x50) | SET_CURSOR_LOCATION);
		break;
	}

}

void LCD_displayCharacter(uint8 character){
	SET_BIT(LCD_CTRL_PORT_OUT,RS);
	CLEAR_BIT(LCD_CTRL_PORT_OUT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT_OUT,E);
	_delay_ms(1);

	#if (DATA_BITS_MODE == 8)

		LCD_DATA_PORT_OUT = character;
		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);

	#elif (DATA_BITS_MODE == 4)
		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | ((character & 0xF0) >> 4);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | (character & 0xF0);
		#endif

		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);
		SET_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);

		#ifdef LOWER_PORT_PINS
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0xF0) | (character & 0x0F);
		#else
			LCD_DATA_PORT_OUT = (LCD_DATA_PORT_OUT & 0x0F) | ((character & 0x0F) << 4);
		#endif

		_delay_ms(1);
		CLEAR_BIT(LCD_CTRL_PORT_OUT,E);
		_delay_ms(1);

	#endif
}

void LCD_displayCharacterRowColumn(uint8 row,uint8 col, uint8 character){
	LCD_goToRowColumn(row, col);
	LCD_displayCharacter(character);
}

void LCD_displayString(const char *string){
	uint8 i = 0;
	while(string[i] != '\0'){
		LCD_displayCharacter(string[i]);
		i++;
	}
}
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *string){
	LCD_goToRowColumn(row, col);
	LCD_displayString(string);
}
void LCD_intgerToString(int data){
	char buff[16];
	itoa(data,buff,10);
	LCD_displayString(buff);
}
void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_SCREEN);
}
void LCD_clearRow(uint8 row){
	uint8 i;
	switch(row)
	{
	case 0:
		LCD_sendCommand( (0x00) |SET_CURSOR_LOCATION);
		for (i = 0 ; i <= 0x0F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 1:
		LCD_sendCommand( (0x40) | SET_CURSOR_LOCATION);
		for (i = 0x40 ; i <= 0X4F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 2:
		LCD_sendCommand( (0x10) | SET_CURSOR_LOCATION);
		for (i = 0x10 ; i <= 0X1F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	case 3:
		LCD_sendCommand( (0x50) | SET_CURSOR_LOCATION);
		for (i = 0x50 ; i <= 0X5F ; i++){
			LCD_displayCharacter(' ');
		}
		break;
	}
	LCD_goToRowColumn(row, 0);
}

void LCD_clearRowColumn(uint8 row, uint8 col){
	LCD_goToRowColumn(row, col);
	LCD_displayCharacter(' ');
}

