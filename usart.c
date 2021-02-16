/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : usart.c
 * Author     : OUN
 * Created on : Nov 13, 2020
 * description: USART source file driver
 --------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "usart.h"

/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/


/*-----------------------------------------------FUNCTIONS DEFINITIONS-----------------------------------------------------*/


/*
 * Description : Function to initialize the USART driver
 *  1. Set the required parity (without parity , even parity , odd parity)
 *  2. Set the value of baud rate -> common value 9600
 * 	3. Set the number of stop bit (one or two)
 * 	4. Set the number of bits we want to transfer
 */
void USART_init(Usart_ConfigType *UsartConfig_Ptr){

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	/*
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 */
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/* URSEL   = 1 The URSEL must be one when writing the UCSRC */
	UCSRC = (1<<URSEL);
	switch (UsartConfig_Ptr->s_parity){
	case NO_PARITY:
		/* UPM1:0 = 00 For no parity mode */
		break;
	case EVEN:
		/* UPM1:0 = 10 For even parity mode */
		SET_BIT(UCSRC,UPM1);
		break;
	case ODD:
		/* UPM1:0 = 11 For odd parity mode */
		UCSRC |= (1<<UPM1) | (1<<UPM0);
		break;
	}

	if (UsartConfig_Ptr->s_stopBit == DOUBLE){

		/* USBS = 1 For two stop bit mode */
		SET_BIT(UCSRC,USBS);
	}
	switch (UsartConfig_Ptr->s_bitDataNum){
	case FIVE_BITS:
		/* UCSZ1:0 = 00 For 5-bit data mode */
		break;
	case SIX_BITS:
		/* UCSZ1:0 = 10 For 6-bit data mode*/
		SET_BIT(UCSRC,UCSZ0);
		break;
	case SEVEN_BITS:
		/* UCSZ1:0 = 01 For 7-bit data mode*/
		SET_BIT(UCSRC,UCSZ1);
		break;
	case EIGHT_BITS:
		/* UCSZ1:0 = 11 For 8-bit data mode */
		UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
		break;
	case NINE_BITS:
		/*
		 * UCSZ1:0 = 11 and UCSZ2 = 1 For 9-bit data mode
		 */
		SET_BIT(UCSRB,UCSZ2);
		UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
		break;
	}

	/*calculation of baud scale in terms of given number : Baud Rate*/
	uint16 baudScale = (F_CPU/(8*(UsartConfig_Ptr->s_baudRate)))-1 ;

	/* set lower 8 bits in register UBRRL
	 * set upper 8 bits in register UBRRH  */
	UBRRL = (baudScale & 0x00FF);
	UBRRH = (baudScale >> 8);

}

/*
 * Description: Function to send one byte using usart
 */
void USART_sendByte(uint8 a_data){

	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while (BIT_IS_CLEAR(UCSRA, UDRE));
	/* set required byte in UDR register to send it */
	UDR = a_data;
	/*
	 * another method
	 * UDR = a_data;
	 * while (BIT_IS_CLEAR(UCSRA, TXC));
	 * SET_BIT(UCSRA, TXC);
	 */
}

/*
 * Description: Function to receive one byte using usart
 */
uint8 USART_receiveByte(void){

	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while (BIT_IS_CLEAR(UCSRA, RXC));

	/* Read the received data from the Rx buffer (UDR)
	   After reading the data it will clear the flag automatically */
	return UDR;
}

void USART_sendString(const uint8 *string_Ptr){

	/* loop along string characters and send it byte byte until null character '\0' */
	while(*string_Ptr != '\0'){
		USART_sendByte(*string_Ptr);
		string_Ptr++;
	}
}
void USART_receiveString(uint8 *string_Ptr){ /* receive until $ */
	uint8 i=0;

	/* receive bytes until character '$' and save it in string*/
	while ((string_Ptr[i]) != '$')
	{
		string_Ptr[i] = USART_receiveByte();
		i++;
	}
	/* change last byte in string to null character '\0' */
	string_Ptr[i]='\0';
}

void USART_sendPass(const uint8 *a_pass_Ptr){
	uint8 i;

	/* loop along array bytes and send it one by one */
	for(i = 0 ; i < 6 ; i++){
		USART_sendByte(a_pass_Ptr[i]);
	}
}
void USART_receivePass(uint8 *a_pass_Ptr){
	uint8 i;

	/* receive six bytes and save it in an array */
	for(i = 0 ; i < 6 ; i++){
		a_pass_Ptr[i] = USART_receiveByte();
	}
}
