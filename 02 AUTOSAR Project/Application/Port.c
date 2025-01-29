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
STATIC uint8 Port_state = PORT_NOT_INITIALIZED;
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
    volatile Port_PinType pinIndex;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if null config ptr */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
        PORT_E_PARAM_CONFIG);
    }
    else
    {
        /* Do Nothing */
    }
#endif
    pins_configPtr = ConfigPtr->Pins;
    /* Loop on all pins to init it */
    for (pinIndex = PORT_PIN0_ID; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
    {
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
            break;
        }
        /* Checking if the pin is locked (PD7 or PF0) */
        if ((pins_configPtr[pinIndex].port_num == PORT_D_ID
                && pins_configPtr[pinIndex].pin_num == PORT_PIN7_ID)
                || (pins_configPtr[pinIndex].port_num == PORT_F_ID
                        && pins_configPtr[pinIndex].pin_num == PORT_PIN0_ID))
        {
            REGISTER(Port_BaseAdressPtr,PORT_LOCK_REG_OFFSET) =
            GPIO_UNLOCK_VALUE; /* Unlock the GPIOCR Register */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_LOCK_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);/* Set the corresponding bit in GPIOCR register to allow changes on this pin */
        }/* Checking if the pin is JTAG pin */
        else if (pins_configPtr[pinIndex].port_num == PORT_C_ID
                && pins_configPtr[pinIndex].pin_num <= PORT_PIN3_ID)
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
            CLEAR_BIT(
                    REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (pins_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),pins_configPtr[pinIndex].pin_num);

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

        }
        else if (pins_configPtr[pinIndex].mode == PORT_PIN_MODE_ADC)
        {
            /* For ADC Mode */

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            /* Clear the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (pins_configPtr[pinIndex].pin_num * 4));

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(
                    REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

        }
        else
        {
            /* For any other alternative mode */

            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(
                    REGISTER(Port_BaseAdressPtr,PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_ALT_FUNC_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            /* Set the PMCx bits for this pin */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) |= (0x0000000F
                    << (pins_configPtr[pinIndex].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);
        }
        /* Set the pin direction */
        if (pins_configPtr[pinIndex].direction == PORT_PIN_IN)
        {

            /*Clear the Corresponding bit in GPIODIR to configure it as input pin*/
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);
            if (pins_configPtr[pinIndex].resistor == PULL_DOWN)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_DOWN_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].resistor == PULL_UP)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_UP_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_PULL_UP_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
                CLEAR_BIT(
                        REGISTER(Port_BaseAdressPtr,PORT_PULL_DOWN_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
        }
        else if (pins_configPtr[pinIndex].direction == PORT_PIN_OUT)
        {

            /*Set the Corresponding bit in GPIODIR to configure it as output pin*/
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);
            if (pins_configPtr[pinIndex].initial_value == PORT_PIN_LEVEL_HIGH)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DATA_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].initial_value
                    == PORT_PIN_LEVEL_LOW)
            {
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_DATA_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* Do Nothing */
            }

        }
        else
        {
            /* Do Nothing */
        }
    }
    Port_state = PORT_INITIALIZED;
}
/******************************************************************************
 * Service name     : Port_SetPinDirection
 * Service ID[hex]  : 0x01
 * Sync/Async       : Synchronous
 * Reentrancy       : Reentrant
 * Parameters (in)  : Pin       --> Port Pin ID number
 Direction --> Port Pin Direction
 * Parameters(inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Sets the port pin direction
 ******************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    uint8 det_ErrorState = FALSE;

    if (Pin >= PORT_CONFIGURED_PINS)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_SET_PIN_DIRECTION_SID,
                        PORT_E_PARAM_PIN);
#endif
        return;
    }
    else
    {
        /* Do Nothing */
    }
    if (pins_configPtr[Pin].pin_direction_changeable
            == PORT_PIN_DIRECTION_NOT_CHANGEABLE)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_SET_PIN_DIRECTION_SID,
                        PORT_E_DIRECTION_UNCHANGEABLE);
#endif
        return;
    }
    else
    {
        /* Do Nothing */
    }
    if (Port_state == PORT_NOT_INITIALIZED)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_SET_PIN_DIRECTION_SID,
                        PORT_E_UNINIT);
#endif
        return;
    }
    else
    {
        /* Do Nothing */
    }
    if (det_ErrorState == FALSE)
    {
        switch (pins_configPtr[Pin].port_num)
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
            break;
        }
        if(Direction == PORT_PIN_OUT)
        {
            SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIR_REG_OFFSET),pins_configPtr[Pin].pin_num);
        }
        else if(Direction == PORT_PIN_IN)
        {
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIR_REG_OFFSET),pins_configPtr[Pin].pin_num);

        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
    /* Do Nothing */
    }
}
#endif

/******************************************************************************
* Service Name      : Port_RefreshPortDirection
* Service ID[hex]   : 0x02
* Sync/Async        : Synchronous
* Reentrancy        : Not Reentrant
* Parameters (in)   : None
* Parameters (inout): None
* Parameters (out)  : None
* Return value      : None
* Description       : Refreshes port direction.
*******************************************************************************/
void Port_RefreshPortDirection(void)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    Port_PinType pinIndex;

    /* Check if the PORT is initialized */

    if (Port_state == PORT_NOT_INITIALIZED)
    {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_REFRESH_PORT_DIRECTION_SID,
                        PORT_E_UNINIT);
#endif
        return; /* Exit the function if the PORT is not initialized */
    }

    /* Loop through all configured pins and update their direction */
    for (pinIndex = 0; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
    {
        /* Get the base address of the port for the current pin */
        switch (pins_configPtr[pinIndex].port_num)
        {
        case PORT_A_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_B_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_C_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_D_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_E_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_F_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
            break;
        default:
            /* Invalid port number continue to the next pin */
            continue;
        }

        /* Update the direction only if the pin is direction changeable */
        if (pins_configPtr[pinIndex].pin_direction_changeable == PORT_PIN_DIRECTION_CHANGEABLE)
        {
            if (pins_configPtr[pinIndex].direction == PORT_PIN_OUT)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr,PORT_DIR_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].direction == PORT_PIN_IN)
            {
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* Do Nothing */
        }
    }
}

/******************************************************************************
* Service Name      : Port_GetVersionInfo
* Service ID[hex]   : 0x03
* Sync/Async        : Synchronous
* Reentrancy        : Not Reentrant
* Parameters (in)   : None
* Parameters (inout): None
* Parameters (out)  : versioninfo --> Pointer to where to store the version
*                                     information of this module.
* Return value      : None
* Description       : Returns the version information of this module.
*******************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
                DIO_GET_VERSION_INFO_SID, DIO_E_PARAM_POINTER);
    }
    else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_RELEASE_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_RELEASE_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_RELEASE_PATCH_VERSION;
    }
}
#endif

/************************************************************************************
* Service Name      : Port_SetPinMode
* Service ID[hex]   : 0x04
* Sync/Async        : Synchronous
* Reentrancy        : Reentrant
* Parameters (in)   : Pin - Port Pin ID number
*                           Mode - New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out)  : None
* Return value      : None
* Description       : Sets the port pin mode.
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    volatile uint8 det_ErrorState = FALSE;
    if(Port_state == PORT_NOT_INITIALIZED){
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
#endif
    }
    else
    {
        /* Do Nothing */
    }
    if (Pin >= PORT_CONFIGURED_PINS)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
#endif
    }
    else
    {
        /* Do Nothing */
    }
    if(pins_configPtr[Pin].pin_mode_changeable == PORT_PIN_MODE_NOT_CHANGEABLE){
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
#endif
    }
    else
    {
        /* Do Nothing */
    }
    /* Get the base address of the corresponding port */
        switch (pins_configPtr[Pin].port_num)
        {
        case PORT_A_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_B_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_C_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_D_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_E_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_F_ID:
            Port_BaseAdressPtr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
            break;
        default:
            return;
        }

        /* Apply the new mode */
        if (Mode == PORT_PIN_MODE_DIO)
        {
            /* Disable analog mode */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                      pins_configPtr[Pin].pin_num);

            /* Disable alternate function */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                      pins_configPtr[Pin].pin_num);

            /* Clear the PMCx bits (set to GPIO mode) */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (pins_configPtr[Pin].pin_num * 4));

            /* Enable digital function */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[Pin].pin_num);
        }
        else if (Mode == PORT_PIN_MODE_ADC)
        {
            /* Enable analog mode */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    pins_configPtr[Pin].pin_num);

            /* Disable alternate function */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                      pins_configPtr[Pin].pin_num);

            /* Clear the PMCx bits */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (pins_configPtr[Pin].pin_num * 4));

            /* Disable digital function */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                      pins_configPtr[Pin].pin_num);
        }
        else
        {
            /* For alternative modes, enable alternate function */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                    pins_configPtr[Pin].pin_num);

            /* Set the PMCx bits for this mode */
            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) |= (0x0000000F
                    << (pins_configPtr[Pin].pin_num * 4));

            /* Enable digital function */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[Pin].pin_num);
        }
}

#endif

