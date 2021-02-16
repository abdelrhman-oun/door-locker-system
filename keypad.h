
/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : keypad.h
 * Author     : OUN
 * Created on : Oct 31, 2020
 * description: keypad header file driver for 4x3 and 4x4 keypad
 --------------------------------------------------------------------------------------------------------------------------*/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/

#define NO_COL 3
#define NO_ROW 4

#define REG_PORT_DIR DDRA
#define REG_PORT_OUT PORTA
#define REG_PORT_IN  PINA

/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

uint8 KEYPAD_getPressedKey(void);


#endif /* KEYPAD_H_ */
