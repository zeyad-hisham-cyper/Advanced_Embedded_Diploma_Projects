/******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: SysTick.h
 *
 * Description: SysTick Timer Header File
 *
 * Author: Zeyad Hisham Elsayed
 *
 *******************************************************************************/

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "../../std_types.h"

/*******************************************************************************
 *                             PreProcessor Macros                             *
 *******************************************************************************/

#define CPU_CLK 16000000
#define SYSTICK_ENABLE_BIT 0
#define SYSTICK_INTERRUPT_ENABLE_BIT 1
#define SYSTICK_CLK_SELECT_BIT 2
#define SYSTICK_COUNT_FLAG_BIT 16

/*******************************************************************************
 *                                  Functions                                  *
 *******************************************************************************/

/*****************************************************************************************
 * Function Name: SysTick_Init
 * Parameters (in): a_TimeInMilliSeconds - Time in milliseconds for periodic interrupts
 * Parameters (out): None
 * Return value: None
 * Description: Initializes SysTick timer with the given time in milliseconds using interrupts.
 *****************************************************************************************/

void SysTick_Init(uint16 a_TimeInMilliSeconds);

/*****************************************************************************************
 * Function Name: SysTick_StartBusyWait
 * Parameters (in): a_TimeInMilliSeconds - Time in milliseconds for the delay
 * Parameters (out): None
 * Return value: None
 * Description: Starts SysTick in busy-wait mode for the specified time in milliseconds.
 *****************************************************************************************/

void SysTick_StartBusyWait(uint16 a_TimeInMilliSeconds);

/*****************************************************************************************
 * Function Name: SysTick_Handler
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: SysTick interrupt handler that calls the callback function.
 *****************************************************************************************/

void SysTick_Handler(void);

/*****************************************************************************************
 * Service Name: SysTick_SetCallBack
 * Parameters (in): Ptr2Func - Pointer to the callback function
 * Parameters (out): None
 * Return value: None
 * Description: Sets the callback function to be called in the SysTick interrupt.
 *****************************************************************************************/

void SysTick_SetCallBack(volatile void (*Ptr2Func)(void));

/*****************************************************************************************
 * Function Name: SysTick_Stop
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: Stops the SysTick timer by disabling it.
 *****************************************************************************************/

void SysTick_Stop(void);

/*****************************************************************************************
 * Service Name: SysTick_Start
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: Starts the SysTick timer with system clock and enables interrupts.
 *****************************************************************************************/

void SysTick_Start(void);

/*****************************************************************************************
 * Function Name: SysTick_DeInit
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: De-initializing SysTick by disabling it and clearing all registers.
 *****************************************************************************************/

void SysTick_DeInit(void);

#endif
