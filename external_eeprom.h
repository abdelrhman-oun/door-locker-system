/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : eeprom.h
 * Author     : OUN
 * Created on : Dec 12, 2020
 * description: EEPROM header file driver
 --------------------------------------------------------------------------------------------------------------------------*/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include "i2c.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/

#define ERROR 0
#define SUCCESS 1

/*------------------------------------------------FUNCTIONS PROTOTYPES-----------------------------------------------------*/
/*
 * Description : Function to initialize the EEPROM driver
 * 				 receive i2C configuration structure
 */
void EEPROM_init(TwiConfig *a_TwiConfig_Ptr);

/*
 * Description: Function to write one byte in specific address in EEPROM
 */
uint8 EEPROM_writeOneByte(uint16 a_Address, uint8 a_Data);

/*
 * Description: Function to read one byte in specific address in EEPROM
 */
uint8 EEPROM_readOneByte(uint16 a_Address, uint8 *a_Data_Ptr);

/*
 * Description: Function to write number of bytes given and save it
 */
uint8 EEPROM_writeBytes(uint16 a_Address, uint8 *a_Data_Ptr, uint8 a_numBytes);

/*
 * Description: Function to read number of bytes given form EEPROM
 */
uint8 EEPROM_readBytes(uint16 a_Address, uint8 *a_Data_Ptr, uint8 a_numBytes);

#endif /* EXTERNAL_EEPROM_H_ */
