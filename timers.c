/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : timers.c
 * Author     : OUN
 * Created on : Dec 8, 2020
 * description: timers source file driver
 --------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "timers.h"

/*--------------------------------------------------GLOBAL VARIABLES-------------------------------------------------------*/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1CompBCallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2CallBackPtr)(void) = NULL_PTR;


/*--------------------------------------------INTERRUPT SERVICE ROUTINES---------------------------------------------------*/

ISR (TIMER0_OVF_vect){
	if(g_timer0CallBackPtr != NULL_PTR){

		/* Call the Call Back function in the application*/
		g_timer0CallBackPtr();
		/* (*g_timer0CallBackPtr)();  another method to call the function  */
	}
}

ISR (TIMER0_COMP_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer0CallBackPtr();
	}
}

ISR (TIMER1_OVF_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer1CallBackPtr();
	}
}

ISR (TIMER1_COMPA_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer1CallBackPtr();
	}
}

ISR (TIMER1_COMPB_vect){
	if(g_timer1CompBCallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer1CompBCallBackPtr();
	}
}


ISR (TIMER2_OVF_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer2CallBackPtr();
	}
}

ISR (TIMER2_COMP_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		/* Call the Call Back function in the application */
		g_timer2CallBackPtr();
	}
}


/*-----------------------------------------------FUNCTIONS DEFINITIONS-----------------------------------------------------*/

/*
 * Description : Function to initialize the timer driver
 *  1. Set the required timer
 *  2. Set the required mode (overflow , compare match)
 * 	3. Set the required prescaler (clock).
 * 	4. Set the initial value of timer.
 * 	5. Set the compare value of timer if chosen compare mode.
 * 	6. Set the compare value of timer channel B if chosen compare mode and timer 1.
 */
void Timer_init(const Timer_ConfigType *a_TimerConfig_Ptr){

	switch (a_TimerConfig_Ptr->s_timer){
	case TIMER_0:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW){

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR0 Register
			 */
			TCCR0 =  (TCCR0 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 0 operates in over flow mode
			 * and set the rest of register bits to 0
			 */
			TCCR0 = (TCCR0 & 0X07) | (1<<FOC0);

			/* Initial Value for Timer 0 */
			TCNT0 = a_TimerConfig_Ptr->s_initValue;

			/* enable overflow interrupt of timer 0
			 * disable other interrupt of timer 0
			 */
			TIMSK |= (1<<TOIE0);
			TIMSK &= ~(1<<OCIE0);
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH){

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR0 Register
			 */
			TCCR0 =  (TCCR0 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 0 operates in compare mode
			 * and set the rest of register bits to 0
			 */
			TCCR0 = (TCCR0 & 0X07) | (1<<WGM01) | (1<<FOC0);

			/* Initial Value for Timer 0 */
			TCNT0 = a_TimerConfig_Ptr->s_initValue;

			/* compare value for timer 0*/
			OCR0 = a_TimerConfig_Ptr->s_compareValue;

			/* enable compare match interrupt of  timer 0
			 * disable other interrupt of timer 0
			 */
			TIMSK |= (1<<OCIE0);
			TIMSK &= ~(1<<TOIE0);
		}
		break;
	case TIMER_1:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW){

			/* Timer1 operates in Normal Mode */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 1 operates in over flow mode
			 * and set the rest of register bits to 0
			 */
			TCCR1B = (TCCR1B & 0X07);

			/* Initial Value for Timer1 */
			TCNT1 = a_TimerConfig_Ptr->s_initValue;

			/* enable overflow interrupt of timer 1
			 * disable other interrupts of timer 1
			 */
			TIMSK |= (1<<TOIE1);
			TIMSK &= ~(1<<OCIE1A) & ~(1<<OCIE1B) & ~(1<<TICIE1);
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH){

			/*
			 * Timer1 operates in Normal Mode
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 1 operates in compare mode
			 * and set the rest of register bits to 0
			 */
			TCCR1B = (TCCR1B & 0X07) | (1<<WGM12);

			/* Initial Value for Timer1 */
			TCNT1 = a_TimerConfig_Ptr->s_initValue;

			/*
			 * compare value for channel A in timer 1
			 * compare value for channel B in timer 1
			 */
			OCR1A = a_TimerConfig_Ptr->s_compareValue;
			OCR1B = a_TimerConfig_Ptr->s_compareValue_B;

			/* enable compare match interrupt of channel A & B in timer 1
			 * disable other interrupts of timer 1
			 */
			TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
			TIMSK &= ~(1<<TOIE1) & ~(1<<TICIE1);
		}
		break;
	case TIMER_2:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW){

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR2 Register
			 */
			TCCR2 =  (TCCR2 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 2 operates in over flow mode
			 * and set the rest of register bits to 0
			 */
			TCCR2 = (TCCR2 & 0X07) | (1<<FOC2);

			/* Initial Value for Timer 2 */
			TCNT2 = a_TimerConfig_Ptr->s_initValue;

			/* enable overflow interrupt of timer 2
			 * disable other interrupt of timer 2
			 */
			TIMSK |= (1<<TOIE2);
			TIMSK &= ~(1<<OCIE2);
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH){

			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR2 Register
			 */
			TCCR2 =  (TCCR2 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler);

			/*
			 * set timer 2 operates in compare mode
			 * and set the rest of register bits to 0
			 */
			TCCR2 = (TCCR2 & 0X07)  | (1<<WGM21) | (1<<FOC2);

			/* Initial Value for Timer 2 */
			TCNT2 = a_TimerConfig_Ptr->s_initValue;

			/* compare value for timer 2*/
			OCR2 =  a_TimerConfig_Ptr->s_compareValue;

			/* enable compare match interrupt of  timer 2
			 * disable other interrupt of timer 2
			 */
			TIMSK |= (1<<OCIE2);
			TIMSK &= ~(1<<TOIE2);
		}
		break;
	}
}


/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_Number a_Timer, void(*a_Ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	switch (a_Timer){
	case TIMER_0:
		g_timer0CallBackPtr = a_Ptr;
		break;
	case TIMER_1:
		g_timer1CallBackPtr = a_Ptr;
		break;
	case TIMER_2:
		g_timer2CallBackPtr = a_Ptr;
		break;
	}
}

/*
 * Description: Function to set the Call Back function address
 * 			    if chosen compare mode and timer 1 and channel B.
 */
void Timer_1_CompB_setCallBack(void(*a_Ptr)(void)){
	g_timer1CompBCallBackPtr = a_Ptr;
}

/*
 * Description: Function to get the Timer Value
 */
uint16 Timer_getTimerValue(Timer_Number a_Timer){
	switch (a_Timer){
	case TIMER_0:
		/* return value of number 0*/
		return TCNT0;
		break;
	case TIMER_1:
		/* return value of number 1*/
		return TCNT1;
		break;
	case TIMER_2:
		/* return value of number 2*/
		return TCNT2;
		break;
	default:
		return 0;
	}
}


/*
 * Description: Function to clear the Timer Value to start count from ZERO
 */
void Timer_clearTimerValue(Timer_Number a_Timer){
	switch (a_Timer){
	case TIMER_0:
		/* clear timer 0*/
		TCNT0 = 0;
		break;
	case TIMER_1:
		/* clear timer 1*/
		TCNT1 = 0;
		break;
	case TIMER_2:
		/* clear timer 2*/
		TCNT2 = 0;
		break;
	}
}

/*
 * Description: Function to disable the Timer
 */
void Timer_deInit(Timer_Number a_Timer){
	switch (a_Timer){
	case TIMER_0:
		/* Clear All Timer0 Registers */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;

		/* Disable the interrupt */
		TIMSK &= ~(1<<OCIE0) & ~(1<<TOIE0);
		break;
	case TIMER_1:
		/* Clear All Timer1 Registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
		OCR1B = 0;

		/* Disable the interrupt */
		TIMSK &= ~(1<<OCIE1A) & ~(1<<OCIE1B) & ~(1<<TOIE1);
		break;
	case TIMER_2:
		/* Clear All Timer2 Registers */
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;

		/* Disable the interrupt */
		TIMSK &= ~(1<<OCIE2) & ~(1<<TOIE2);
		break;
	}
}

