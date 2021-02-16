/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : timers.h
 * Author     : OUN
 * Created on : Dec 8, 2020
 * description: timers header file driver
 --------------------------------------------------------------------------------------------------------------------------*/

#ifndef TIMERS_H_
#define TIMERS_H_


/*-----------------------------------------------------INCLUDES------------------------------------------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/*-------------------------------------------------TYPES DECLARATION-------------------------------------------------------*/

typedef enum{
	TIMER_0, TIMER_1, TIMER_2
}Timer_Number;

typedef enum{
	OVERFLOW, COMPARE_MATCH
}Timer_Mode;

typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Prescaler;


typedef struct{
	Timer_Number s_timer;
	Timer_Mode s_mode;
	Timer_Prescaler s_prescaler;
	uint16 s_initValue;
	uint16 s_compareValue;    //also used as compare value A for timer 1
	uint16 s_compareValue_B;

}Timer_ConfigType;


/*----------------------------------------------------DFINATIONS-----------------------------------------------------------*/


/*-----------------------------------------------FUNCTION DECLARATIONS-----------------------------------------------------*/

/*
 * Description : Function to initialize the timer driver
 *  1. Set the required timer
 *  2. Set the required mode (overflow , compare match)
 * 	3. Set the required prescaler (clock).
 * 	4. Set the initial value of timer.
 * 	5. Set the compare value of timer if chosen compare mode.
 * 	6. Set the compare value of timer channel B if chosen compare mode and timer 1.
 */
void Timer_init(const Timer_ConfigType *a_TimerConfig_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_Number a_Timer, void(*a_Ptr)(void));

/*
 * Description: Function to set the Call Back function address
 * 			    if chosen compare mode and timer 1 and channel B.
 */
void Timer_1_CompB_setCallBack(void(*a_Ptr)(void));

/*
 * Description: Function to get the Timer Value
 */
uint16 Timer_getTimerValue(Timer_Number a_Timer);

/*
 * Description: Function to clear the Timer Value to start count from ZERO
 */
void Timer_clearTimerValue(Timer_Number a_Timer);

/*
 * Description: Function to disable the Timer
 */
void Timer_deInit(Timer_Number a_Timer);

#endif /* TIMERS_H_ */
