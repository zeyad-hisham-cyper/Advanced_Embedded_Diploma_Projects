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

/*
 * Function to initialize the timer and give a periodic interrupt every specified time in milliseconds
 */
void SysTick_Init(uint16 a_TimeInMilliSeconds);
/*
 * Delay Function using polling technique and SysTick timer
 */
void SysTick_StartBusyWait(uint16 a_TimeInMilliSeconds);
/*
 * Handler for SysTick interrupt use to call the call-back function.
 */
void SysTick_Handler(void);
/*
 * Function to setup the SysTick Timer call back to be executed in SysTick Handler.
 */
void SysTick_SetCallBack(volatile void (*Ptr2Func)(void));
/*
 * Stop the SysTick Timer
 */
void SysTick_Stop(void);
/*
 * Start / Resume the SysTick Timer
 */
void SysTick_Start(void);
/*
 * Function to De-initialize the SysTick Timer.
 */
void SysTick_DeInit(void);

#endif
