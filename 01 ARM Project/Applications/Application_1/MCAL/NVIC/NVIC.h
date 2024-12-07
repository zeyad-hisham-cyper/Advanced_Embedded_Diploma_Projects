/******************************************************************************
 *
 * Module: NVIC
 *
 * File Name: NVIC.h
 *
 * Description: Header file for the ARM Cortex M4 NVIC driver
 *
 * Author: Zeyad Hisham Elsayed
 *
 *******************************************************************************/

#ifndef NVIC_H_
#define NVIC_H_

#include "../../std_types.h"

/*******************************************************************************
 *                           Preprocessor Definitions                          *
 *******************************************************************************/

/* Mask for priority bits in the NVIC */
#define IRQ_PIRIORITY_BITS_MASK              0xE0

/* Masks and bit positions for system exception priorities */
#define MEM_FAULT_PRIORITY_MASK              0x000000E0
#define MEM_FAULT_PRIORITY_BITS_POS          5

#define BUS_FAULT_PRIORITY_MASK              0x0000E000
#define BUS_FAULT_PRIORITY_BITS_POS          13

#define USAGE_FAULT_PRIORITY_MASK            0x00E00000
#define USAGE_FAULT_PRIORITY_BITS_POS        21

#define SVC_PRIORITY_MASK                    0xE0000000
#define SVC_PRIORITY_BITS_POS                29

#define DEBUG_MONITOR_PRIORITY_MASK          0x000000E0
#define DEBUG_MONITOR_PRIORITY_BITS_POS      5

#define PENDSV_PRIORITY_MASK                 0x00E00000
#define PENDSV_PRIORITY_BITS_POS             21

#define SYSTICK_PRIORITY_MASK                0xE0000000
#define SYSTICK_PRIORITY_BITS_POS            29

/* Masks for enabling system exceptions */
#define MEM_FAULT_ENABLE_MASK                0x00010000
#define BUS_FAULT_ENABLE_MASK                0x00020000
#define USAGE_FAULT_ENABLE_MASK              0x00040000

/* Enable Exceptions ... */
#define Enable_Exceptions()    __asm(" CPSIE I ")

/* Disable Exceptions ... */
#define Disable_Exceptions()   __asm(" CPSID I ")

/* Enable Faults ... */
#define Enable_Faults()        __asm(" CPSIE F ")

/* Disable Faults ... */
#define Disable_Faults()       __asm(" CPSID F ")

/* Wait for Interrupt ... */
#define Wait_For_Interrupt()   __asm(" WFI ")

/*******************************************************************************
 *                           Data Types Declarations                           *
 *******************************************************************************/

typedef uint8 NVIC_IRQType;
typedef uint8 NVIC_IRQPriorityType;

typedef enum
{
    EXCEPTION_RESET_TYPE,
    EXCEPTION_NMI_TYPE,
    EXCEPTION_HARD_FAULT_TYPE,
    EXCEPTION_MEM_FAULT_TYPE,
    EXCEPTION_BUS_FAULT_TYPE,
    EXCEPTION_USAGE_FAULT_TYPE,
    EXCEPTION_SVC_TYPE,
    EXCEPTION_DEBUG_MONITOR_TYPE,
    EXCEPTION_PEND_SV_TYPE,
    EXCEPTION_SYSTICK_TYPE
} NVIC_ExceptionType;

typedef uint8 NVIC_ExceptionPriorityType;

/*******************************************************************************
 *                                  Functions                                  *
 *******************************************************************************/

/*****************************************************************************************
 * Function Name: NVIC_EnableIRQ
 * Parameters (in): IRQ_Num - Number of the IRQ to enable
 * Parameters (out): None
 * Return value: None
 * Description: Enables the specified IRQ in the NVIC.
 *****************************************************************************************/
void NVIC_EnableIRQ(NVIC_IRQType IRQ_Num);

/*****************************************************************************************
 * Function Name: NVIC_DisableIRQ
 * Parameters (in): IRQ_Num - Number of the IRQ to disable
 * Parameters (out): None
 * Return value: None
 * Description: Disables the specified IRQ in the NVIC.
 *****************************************************************************************/
void NVIC_DisableIRQ(NVIC_IRQType IRQ_Num);

/*****************************************************************************************
 * Function Name: NVIC_SetPriorityIRQ
 * Parameters (in): IRQ_Num - Number of the IRQ to set the priority for
 *                  IRQ_Priority - Priority value to set (0-7)
 * Parameters (out): None
 * Return value: None
 * Description: Sets the priority of the specified IRQ in the NVIC.
 *****************************************************************************************/
void NVIC_SetPriorityIRQ(NVIC_IRQType IRQ_Num, NVIC_IRQPriorityType IRQ_Priority);

/*****************************************************************************************
 * Function Name: NVIC_EnableException
 * Parameters (in): Exception_Num - Number of the exception to enable
 * Parameters (out): None
 * Return value: None
 * Description: Enables the specified system exception.
 *****************************************************************************************/
void NVIC_EnableException(NVIC_ExceptionType Exception_Num);

/*****************************************************************************************
 * Function Name: NVIC_DisableException
 * Parameters (in): Exception_Num - Number of the exception to disable
 * Parameters (out): None
 * Return value: None
 * Description: Disables the specified system exception.
 *****************************************************************************************/
void NVIC_DisableException(NVIC_ExceptionType Exception_Num);

/*****************************************************************************************
 * Function Name: NVIC_SetPriorityException
 * Parameters (in): Exception_Num - Number of the exception to set the priority for
 *                  Exception_Priority - Priority value to set (0-7)
 * Parameters (out): None
 * Return value: None
 * Description: Sets the priority of the specified system exception.
 *****************************************************************************************/
void NVIC_SetPriorityException(NVIC_ExceptionType Exception_Num, NVIC_ExceptionPriorityType Exception_Priority);

#endif /* NVIC_H_ */
