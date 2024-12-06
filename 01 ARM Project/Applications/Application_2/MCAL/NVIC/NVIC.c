/******************************************************************************
 *
 * Module: NVIC
 *
 * File Name: NVIC.c
 *
 * Description: Source file for the ARM Cortex M4 NVIC driver
 *
 * Author: Zeyad Hisham Elsayed
 *
 *******************************************************************************/
#include "NVIC.h"
#include "../../tm4c123gh6pm_registers.h"

void NVIC_EnableIRQ(NVIC_IRQType IRQ_Num)
{
    uint8 bit_num = IRQ_Num % 32;
    uint8 reg_num = IRQ_Num / 32;
    *((volatile uint32*) (NVIC_EN0_REG + (reg_num * 4))) |= (1 << bit_num);
}
void NVIC_DisableIRQ(NVIC_IRQType IRQ_Num)
{
    uint8 bit_num = IRQ_Num % 32;
    uint8 reg_num = IRQ_Num / 32;
    *((volatile uint32*) (NVIC_DIS0_REG + (reg_num * 4))) |= (1 << bit_num);
}
void NVIC_SetPriorityIRQ(NVIC_IRQType IRQ_Num,
                         NVIC_IRQPriorityType IRQ_Priority)
{
    volatile uint32 *priority_reg = (volatile uint32*) (NVIC_PRI0_REG
            + ((IRQ_Num / 4) * 4));
    uint8 IRQ_priority_location = (IRQ_Num % 4) * 8;
    *priority_reg = (*priority_reg
            & ~(IRQ_PIRIORITY_BITS_MASK << IRQ_priority_location))
            | (IRQ_Priority << (IRQ_priority_location + 5));
}

void NVIC_EnableException(NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL = BUS_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL = MEM_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL = USAGE_FAULT_ENABLE_MASK;
        break;
    }
}
void NVIC_DisableException(NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~BUS_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~MEM_FAULT_ENABLE_MASK;
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_SYSHNDCTRL &= ~USAGE_FAULT_ENABLE_MASK;
        break;
    }
}
void NVIC_SetPriorityException(NVIC_ExceptionType Exception_Num,
                               NVIC_ExceptionPriorityType Exception_Priority)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG
                & (MEM_FAULT_PRIORITY_MASK))
                | (Exception_Priority << MEM_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_BUS_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG
                & (BUS_FAULT_PRIORITY_MASK))
                | (Exception_Priority << BUS_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_USAGE_FAULT_TYPE:
        NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG
                & (USAGE_FAULT_PRIORITY_MASK))
                | (Exception_Priority << USAGE_FAULT_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_SVC_TYPE:
        NVIC_SYSTEM_PRI2_REG = ( NVIC_SYSTEM_PRI2_REG & (SVC_PRIORITY_MASK))
                | (Exception_Priority << SVC_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_DEBUG_MONITOR_TYPE:
        NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG
                & (DEBUG_MONITOR_PRIORITY_MASK))
                | (Exception_Priority << DEBUG_MONITOR_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_PEND_SV_TYPE:
        NVIC_SYSTEM_PRI3_REG = ( NVIC_SYSTEM_PRI3_REG & (PENDSV_PRIORITY_MASK))
                | (Exception_Priority << PENDSV_PRIORITY_BITS_POS);
        break;
    case EXCEPTION_SYSTICK_TYPE:
        NVIC_SYSTEM_PRI3_REG = ( NVIC_SYSTEM_PRI3_REG & (SYSTICK_PRIORITY_MASK))
                | (Exception_Priority << SYSTICK_PRIORITY_BITS_POS);
        break;
    }
}
