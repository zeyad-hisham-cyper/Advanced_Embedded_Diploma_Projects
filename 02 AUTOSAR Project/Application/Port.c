/*
 * Port.c
 *
 *  Created on: Jan 25, 2025
 *      Author: zeyad
 */

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"

/* AUTOSAR Version Checking between Det and Port */
#if((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
  ||(DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  ||(DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
    #error "The AR version of Det.h doesn't match the expected version"
#endif

#endif

#define REGISTER(BASE,OFFSET) *(volatile uint32*)((volatile uint8*)BASE + OFFSET)
#define GPIO_UNLOCK_VALUE 0x4C4F434B

STATIC const Port_ConfigPinType *pins_configPtr = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
/******************************************************************************
 * Service name     : Port_Init
 * Service ID[hex]  : 0x00
 * Sync/Async       : Synchronous
 * Reentrancy       : Non Reentrant
 * Parameters (in)  : ConfigPtr --> Pointer to configuration set.
 * Parameters(inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Initializes the Port Driver module.
 ******************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    Port_PinType pinIndex = PORT_PIN0_ID;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if null config ptr */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
    }
    else
    {
        /* Do Nothing */
    }
#endif
    pins_configPtr = ConfigPtr->Pins;
    /* Loop on all pins to init it */
    for(pinIndex = PORT_PIN0_ID;pinIndex< PORT_CONFIGURED_PINS;pinIndex++){
        /* Get the base address for the pin's port @ pinIndex number */
        switch (pins_configPtr[pinIndex].port_num)
        {
        case PORT_A_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_B_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_C_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_D_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_E_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_F_ID:
            Port_BaseAdressPtr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS;
            break;
        default:
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_PIN);
#endif
            break;
        }
        /* Checking if the pin is locked (PD7 or PF0) */
        if ((pins_configPtr[pinIndex].port_num == PORT_D_ID && pins_configPtr[pinIndex].pin_num == PORT_PIN7_ID)
                || (pins_configPtr[pinIndex].port_num == PORT_F_ID && pins_configPtr[pinIndex].pin_num == PORT_PIN0_ID))
        {
            REGISTER(Port_BaseAdressPtr,PORT_LOCK_REG_OFFSET) = GPIO_UNLOCK_VALUE; /* Unlock the GPIOCR Register */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_LOCK_REG_OFFSET), pins_configPtr[pinIndex].pin_num);/* Set the corresponding bit in GPIOCR register to allow changes on this pin */
        }/* Checking if the pin is JTAG pin */
        else if (pins_configPtr[pinIndex].port_num == PORT_C_ID && pins_configPtr[pinIndex].pin_num <= PORT_PIN3_ID)
        {
            CONTINUE;
            /* Do nothing if the pin is JTAG pin */
        }
        else
        {
            /* Do Nothing */
        }
        /* Set pin mode */
        if (pins_configPtr[pinIndex].mode == PORT_PIN_MODE_DIO)
        {
            /* For DIO pins */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET), pins_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (pins_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(REG(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),pins_configPtr[pinIndex].pin_num);

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET), pins_configPtr[pinIndex].pin_num);

        }
        else if (pins_configPtr[pinIndex].mode == PORT_PIN_MODE_ADC){
            /* For ADC Mode */

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET), pins_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (pins_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(REG(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),pins_configPtr[pinIndex].pin_num);

            /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET), pins_configPtr[pinIndex].pin_num);

        }else{
            /* For any other alternative mode */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET), pins_configPtr[pinIndex].pin_num);

            /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(REG(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),pins_configPtr[pinIndex].pin_num);

            /* Set the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) |= (0x0000000F << (pins_configPtr[pinIndex].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET), pins_configPtr[pinIndex].pin_num);
        }
        /* Set the pin direction */
        if(pins_configPtr[pinIndex].direction == PORT_PIN_IN){

            /*Clear the Corresponding bit in GPIODIR to configure it as input pin*/
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            if (pins_configPtr[pinIndex].resistor== PULL_DOWN){
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_DOWN_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }else if (pins_configPtr[pinIndex].resistor== PULL_UP){
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_UP_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }else{
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_UP_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_DOWN_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }
        }else if(pins_configPtr[pinIndex].direction == PORT_PIN_OUT){

            /*Set the Corresponding bit in GPIODIR to configure it as output pin*/
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            if (pins_configPtr[pinIndex].initial_value == PORT_PIN_LEVEL_HIGH){
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DATA_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }else if (pins_configPtr[pinIndex].initial_value == PORT_PIN_LEVEL_LOW){
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_DATA_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }else{
                /* Do Nothing */
            }

        }else{
            /* Do Nothing */
        }
    }
    Port_Status = PORT_INITIALIZED;
}
