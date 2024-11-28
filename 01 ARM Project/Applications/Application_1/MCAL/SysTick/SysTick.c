/******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: SysTick.c
 *
 * Description: SysTick Timer Source File
 *
 * Author: Zeyad Hisham Elsayed
 *
 *******************************************************************************/

#include "SysTick.h"
#include "../../tm4c123gh6pm_registers.h"

static volatile void (*g_SysTickCallBack)(void) = NULL_PTR;

/*****************************************************************************************
 * Function Name: SysTick_Init
 * Parameters (in): a_TimeInMilliSeconds - Time in milliseconds for periodic interrupts
 * Parameters (out): None
 * Return value: None
 * Description: Initializes SysTick timer with the given time in milliseconds using interrupts.
 *****************************************************************************************/
void SysTick_Init(uint16 a_TimeInMilliSeconds) {
    SYSTICK_CTRL_REG = 0;  /* Disable SysTick Timer */
    SYSTICK_RELOAD_REG = ((CPU_CLK / 1000) * a_TimeInMilliSeconds) - 1; /* Set reload value */
    SYSTICK_CURRENT_REG = 0;  /* Clear current value */
    SysTick_Start();  /* Start SysTick timer */
}

/*****************************************************************************************
 * Function Name: SysTick_DeInit
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: De-initializing SysTick by disabling it and clearing all registers.
 *****************************************************************************************/
void SysTick_DeInit(void) {
    SYSTICK_CTRL_REG = 0;
    SYSTICK_RELOAD_REG = 0;
    SYSTICK_CURRENT_REG = 0;
}

/*****************************************************************************************
 * Function Name: SysTick_StartBusyWait
 * Parameters (in): a_TimeInMilliSeconds - Time in milliseconds for the delay
 * Parameters (out): None
 * Return value: None
 * Description: Starts SysTick in busy-wait mode for the specified time in milliseconds.
 *****************************************************************************************/
void SysTick_StartBusyWait(uint16 a_TimeInMilliSeconds) {
    SYSTICK_CTRL_REG = 0;  /* Disable SysTick Timer */
    SYSTICK_RELOAD_REG = ((CPU_CLK / 1000) * a_TimeInMilliSeconds) - 1;
    SYSTICK_CURRENT_REG = 0;  /* Clear current value */
    SYSTICK_CTRL_REG = (1 << SYSTICK_CLK_SELECT_BIT) | (1 << SYSTICK_ENABLE_BIT);
    while (!(SYSTICK_CTRL_REG & (1 << SYSTICK_COUNT_FLAG_BIT)));  /* Wait until COUNTFLAG is set */
    SYSTICK_CTRL_REG = 0;  /* Disable SysTick after wait */
}

/*****************************************************************************************
 * Function Name: SysTick_Stop
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: Stops the SysTick timer by disabling it.
 *****************************************************************************************/
void SysTick_Stop(void) {
    SYSTICK_CTRL_REG &= ~(1 << SYSTICK_ENABLE_BIT);  /* Disable SysTick */
}

/*****************************************************************************************
 * Service Name: SysTick_Start
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: Starts the SysTick timer with system clock and enables interrupts.
 *****************************************************************************************/
void SysTick_Start(void) {
    SYSTICK_CTRL_REG = (1 << SYSTICK_CLK_SELECT_BIT) |
                       (1 << SYSTICK_INTERRUPT_ENABLE_BIT) |
                       (1 << SYSTICK_ENABLE_BIT);
}

/*****************************************************************************************
 * Service Name: SysTick_SetCallBack
 * Parameters (in): Ptr2Func - Pointer to the callback function
 * Parameters (out): None
 * Return value: None
 * Description: Sets the callback function to be called in the SysTick interrupt.
 *****************************************************************************************/
void SysTick_SetCallBack(volatile void (*Ptr2Func)(void)) {
    g_SysTickCallBack = Ptr2Func;
}

/*****************************************************************************************
 * Function Name: SysTick_Handler
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: SysTick interrupt handler that calls the callback function.
 *****************************************************************************************/
void SysTick_Handler(void) {
    if (g_SysTickCallBack != NULL_PTR) {
        (*g_SysTickCallBack)();
    }
}
