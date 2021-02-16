/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : usart.h
 * Author     : OUN
 * Created on : Nov 13, 2020
 * description: USART header file driver
 --------------------------------------------------------------------------------------------------------------------------*/

#ifndef USART_H_
#define USART_H_

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/

typedef enum
{
	NO_PARITY,EVEN,ODD
}Usart_ParityType;

typedef enum
{
	SINGLE,DOUBLE
}Usart_StopBitNum;

typedef enum
{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS,NINE_BITS
}Usart_DataNum;

typedef struct
{
	Usart_ParityType s_parity;
	uint16 s_baudRate;
	Usart_StopBitNum s_stopBit;
	Usart_DataNum s_bitDataNum;
}Usart_ConfigType;


/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/

/*
 * Description : Function to initialize the USART driver
 *  1. Set the required parity (without parity , even parity , odd parity)
 *  2. Set the value of baud rate -> common value 9600
 * 	3. Set the number of stop bit (one or two)
 * 	4. Set the number of bits we want to transfer
 */
void USART_init(Usart_ConfigType *UsartConfig_Ptr);

/*
 * Description: Function to send one byte using usart
 */
void USART_sendByte(uint8 a_data);

/*
 * Description: Function to receive one byte using usart
 */
uint8 USART_receiveByte(void);

/*
 * Description: Function to send string until '\0' character
 */
void USART_sendString(const uint8 *string_Ptr);

/*
 * Description: Function to receive string until '$' character
 */
void USART_receiveString(uint8 *string_Ptr); /* receive until $ */

/*
 * Description: Function to send array of six bytes
 */
void USART_sendPass(const uint8 *a_pass_Ptr);

/*
 * Description: Function to receive an array of six bytes
 */
void USART_receivePass(uint8 *a_pass_Ptr); /* receive 6 bytes*/


#endif /* USART_H_ */
