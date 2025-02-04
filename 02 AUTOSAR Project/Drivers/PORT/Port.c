/******************************************************************************
 *
 * Module: PORT
 *
 * File Name: Port.c
 *
 * Description: Source file for AUTOSAR PORT Driver.
 *              This module initializes the port pins, allows for setting the
 *              pin directions and modes, refreshes port directions, and returns
 *              the version information of the driver.
 *
 * Author: Zeyad Hisham
 *
 ******************************************************************************/

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

/* Macro to compute register address given a base address and offset.
   This casts the base address to a pointer to uint8 and adds the offset,
   then casts the result to a pointer to volatile uint32. */
#define REGISTER(BASE,OFFSET) *(volatile uint32*)((volatile uint8*)BASE + OFFSET)

/* Value required to unlock the GPIO registers for specific pins (e.g., PD7, PF0) */
#define GPIO_UNLOCK_VALUE 0x4C4F434B

/* Pointer to the configuration structure that holds all the pin settings */
STATIC const Port_ConfigPinType *pins_configPtr = NULL_PTR;

/* Global variable to track the initialization state of the Port driver */
STATIC uint8 Port_state = PORT_NOT_INITIALIZED;


/******************************************************************************
 * Service name     : Port_Init
 * Service ID[hex]  : 0x00
 * Sync/Async       : Synchronous
 * Reentrancy       : Non Reentrant
 * Parameters (in)  : ConfigPtr --> Pointer to configuration set.
 * Parameters (inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Initializes the Port Driver module.
 *                    This includes setting the mode (DIO, ADC, or alternate mode),
 *                    direction, internal resistor configuration, and initial value
 *                    for each configured pin.
 ******************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    /* Pointer to hold the base address of the port registers for the current pin */
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    volatile Port_PinType pinIndex;

#if(PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the configuration pointer is NULL.
       If it is, report an error and do not proceed with initialization. */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
                        PORT_E_PARAM_CONFIG);
        return;
    }
    /* Else, configuration pointer is valid and initialization can proceed */
#endif

    /* Save the configuration pointer in a global variable for use by other functions */
    pins_configPtr = ConfigPtr->Pins;

    /* Loop on all configured pins to initialize each one according to the configuration */
    for (pinIndex = PORT_PIN0_ID; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
    {
        /* Determine the base address for the current pin's port based on the port number */
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
            /* In case of an invalid port, no action is taken. */
            break;
        }

        /* For pins that are locked (for example, PD7 or PF0), perform unlock sequence */
        if ((pins_configPtr[pinIndex].port_num == PORT_D_ID
                && pins_configPtr[pinIndex].pin_num == PORT_PIN7_ID)
                || (pins_configPtr[pinIndex].port_num == PORT_F_ID
                        && pins_configPtr[pinIndex].pin_num == PORT_PIN0_ID))
        {
            /* Write the unlock value to the LOCK register */
            REGISTER(Port_BaseAdressPtr, PORT_LOCK_REG_OFFSET) = GPIO_UNLOCK_VALUE;
            /* Set the bit corresponding to the pin number in the commit register to allow changes */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_LOCK_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);
        }
        /* If the pin is used for JTAG (typically on PORT_C for pins 0-3), skip any changes */
        else if (pins_configPtr[pinIndex].port_num == PORT_C_ID
                && pins_configPtr[pinIndex].pin_num <= PORT_PIN3_ID)
        {
            /* Continue to next iteration without configuring JTAG pins */
            continue;
        }
        else
        {
            /* No special action required for other pins */
        }

        /* Configure the pin mode */
        if (pins_configPtr[pinIndex].mode == PORT_PIN_MODE_DIO)
        {
            /* For Digital I/O pins (DIO):
               - Disable analog functionality.
               - Clear PMCx bits to select GPIO.
               - Disable alternate function.
               - Enable digital functionality.
            */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<
                        (pins_configPtr[pinIndex].pin_num * 4));

            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);
        }
        else if (pins_configPtr[pinIndex].mode == PORT_PIN_MODE_ADC)
        {
            /* For ADC mode pins:
               - Enable analog functionality.
               - Clear PMCx bits.
               - Disable alternate function.
               - Disable digital functionality.
            */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<
                        (pins_configPtr[pinIndex].pin_num * 4));

            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);
        }
        else
        {
            /* For alternative functions:
               - Disable analog functionality.
               - Enable alternate function.
               - Set the PMCx bits for the desired alternate mode.
               - Enable digital functionality.
            */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) |= (0x0000000F <<
                        (pins_configPtr[pinIndex].pin_num * 4));

            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);
        }

        /* Configure the pin direction and initial value or resistor if input */
        if (pins_configPtr[pinIndex].direction == PORT_PIN_IN)
        {
            /* Set the pin as input by clearing the corresponding bit in the direction register */
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                      pins_configPtr[pinIndex].pin_num);

            /* Configure the internal resistor based on configuration:
               PULL_DOWN, PULL_UP, or no resistor */
            if (pins_configPtr[pinIndex].resistor == PULL_DOWN)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_PULL_DOWN_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].resistor == PULL_UP)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_PULL_UP_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* Disable both pull-up and pull-down resistors */
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_PULL_UP_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_PULL_DOWN_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
            }
        }
        else if (pins_configPtr[pinIndex].direction == PORT_PIN_OUT)
        {
            /* Set the pin as output by setting the corresponding bit in the direction register */
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                    pins_configPtr[pinIndex].pin_num);

            /* Set the initial output value as HIGH or LOW */
            if (pins_configPtr[pinIndex].initial_value == PORT_PIN_LEVEL_HIGH)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DATA_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].initial_value == PORT_PIN_LEVEL_LOW)
            {
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DATA_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* If no initial value is specified, take no action */
            }
        }
        else
        {
            /* For any unspecified direction, do nothing */
        }
    }
    /* Mark the Port driver as initialized */
    Port_state = PORT_INITIALIZED;
}


/******************************************************************************
 * Service name     : Port_SetPinDirection
 * Service ID[hex]  : 0x01
 * Sync/Async       : Synchronous
 * Reentrancy       : Reentrant
 * Parameters (in)  : Pin       --> Port Pin ID number
 *                    Direction --> Port Pin Direction (PORT_PIN_IN or PORT_PIN_OUT)
 * Parameters (inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Sets the port pin direction during runtime.
 *                    This function verifies the following before changing the pin direction:
 *                    - The pin ID is within the configured range.
 *                    - The pin's direction is changeable.
 *                    - The Port module has been initialized.
 ******************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    uint8 det_ErrorState = FALSE;

    /* Check if the pin number is within the valid configured range */
    if (Pin >= PORT_CONFIGURED_PINS)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
#endif
        return;
    }

    /* Check if the pin direction is changeable */
    if (pins_configPtr[Pin].pin_direction_changeable == PORT_PIN_DIRECTION_NOT_CHANGEABLE)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
#endif
        return;
    }

    /* Check if the Port module has been initialized */
    if (Port_state == PORT_NOT_INITIALIZED)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
#endif
        return;
    }

    /* If no error was detected, proceed with changing the pin direction */
    if (det_ErrorState == FALSE)
    {
        /* Determine the base address for the corresponding port */
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
            /* If port number is invalid, exit without taking any action */
            return;
        }

        /* Set or clear the bit in the GPIO direction register based on the requested direction */
        if(Direction == PORT_PIN_OUT)
        {
            SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                    pins_configPtr[Pin].pin_num);
        }
        else if(Direction == PORT_PIN_IN)
        {
            CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                      pins_configPtr[Pin].pin_num);
        }
        else
        {
            /* If an unknown direction is provided, do nothing */
        }
    }
    else
    {
        /* In case of an error, no action is taken */
    }
}
#endif


/******************************************************************************
 * Service Name     : Port_RefreshPortDirection
 * Service ID[hex]  : 0x02
 * Sync/Async       : Synchronous
 * Reentrancy       : Not Reentrant
 * Parameters (in)  : None
 * Parameters (inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Refreshes the port directions for all configured pins.
 *                    Only the pins with changeable direction are updated to their
 *                    configuration-specified direction.
 ******************************************************************************/
void Port_RefreshPortDirection(void)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    Port_PinType pinIndex;

    /* Check if the Port module has been initialized */
    if (Port_state == PORT_NOT_INITIALIZED)
    {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
#endif
        return; /* Exit the function if the PORT is not initialized */
    }

    /* Loop through all configured pins and update their direction if changeable */
    for (pinIndex = 0; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
    {
        /* Get the base address for the current pin's port */
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
            /* If an invalid port number is encountered, skip to the next pin */
            continue;
        }

        /* Update the direction only for pins that are allowed to change direction */
        if (pins_configPtr[pinIndex].pin_direction_changeable == PORT_PIN_DIRECTION_CHANGEABLE)
        {
            if (pins_configPtr[pinIndex].direction == PORT_PIN_OUT)
            {
                SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                        pins_configPtr[pinIndex].pin_num);
            }
            else if (pins_configPtr[pinIndex].direction == PORT_PIN_IN)
            {
                CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIR_REG_OFFSET),
                          pins_configPtr[pinIndex].pin_num);
            }
            else
            {
                /* If direction is undefined, take no action */
            }
        }
        else
        {
            /* If the pin direction is not changeable, do nothing */
        }
    }
}


/******************************************************************************
 * Service Name     : Port_GetVersionInfo
 * Service ID[hex]  : 0x03
 * Sync/Async       : Synchronous
 * Reentrancy       : Not Reentrant
 * Parameters (in)  : None
 * Parameters (inout): None
 * Parameters (out) : versioninfo --> Pointer to a Std_VersionInfoType structure
 *                                     where the version information is stored.
 * Return value     : None
 * Description      : Returns the version information of the PORT module.
 *                    If the provided pointer is NULL, an error is reported.
 ******************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    /* Check if the input pointer is NULL */
    if(NULL_PTR == versioninfo)
    {
        /* Report error if pointer is NULL */
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
                        DIO_GET_VERSION_INFO_SID, DIO_E_PARAM_POINTER);
    }
    else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
    {
        /* Fill in the version information structure */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        versioninfo->sw_major_version = (uint8)PORT_SW_RELEASE_MAJOR_VERSION;
        versioninfo->sw_minor_version = (uint8)PORT_SW_RELEASE_MINOR_VERSION;
        versioninfo->sw_patch_version = (uint8)PORT_SW_RELEASE_PATCH_VERSION;
    }
}
#endif


/************************************************************************************
 * Service Name     : Port_SetPinMode
 * Service ID[hex]  : 0x04
 * Sync/Async       : Synchronous
 * Reentrancy       : Reentrant
 * Parameters (in)  : Pin  --> Port Pin ID number.
 *                    Mode --> New mode to be set for the port pin.
 * Parameters (inout): None
 * Parameters (out) : None
 * Return value     : None
 * Description      : Sets the mode of the port pin during runtime.
 *                    The function verifies that:
 *                      - The Port module is initialized.
 *                      - The provided pin ID is valid.
 *                      - The pin's mode is changeable.
 *                    Then, based on the specified mode, the function configures
 *                    the analog/digital functionality, alternate function, and PMCx bits.
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    volatile uint32 *Port_BaseAdressPtr = NULL_PTR;
    volatile uint8 det_ErrorState = FALSE;

    /* Verify that the Port module has been initialized */
    if(Port_state == PORT_NOT_INITIALIZED)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
#endif
    }

    /* Verify that the provided pin ID is within the valid range */
    if (Pin >= PORT_CONFIGURED_PINS)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
#endif
    }

    /* Check if the pin mode is changeable */
    if (pins_configPtr[Pin].pin_mode_changeable == PORT_PIN_MODE_NOT_CHANGEABLE)
    {
        det_ErrorState = TRUE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
#endif
    }

    /* Obtain the base address for the pin's port */
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
        /* If port number is invalid, exit without taking any action */
        return;
    }

    /* Apply the new mode settings based on the requested mode */
    if (Mode == PORT_PIN_MODE_DIO)
    {
        /* For Digital I/O:
           - Disable analog mode.
           - Disable alternate function.
           - Clear PMCx bits to select GPIO mode.
           - Enable digital functionality.
        */
        CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);

        CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);

        REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<
                  (pins_configPtr[Pin].pin_num * 4));

        SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);
    }
    else if (Mode == PORT_PIN_MODE_ADC)
    {
        /* For ADC mode:
           - Enable analog functionality.
           - Disable alternate function.
           - Clear PMCx bits.
           - Disable digital functionality.
        */
        SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ANALOG_MODE_SEL_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);

        CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);

        REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<
                  (pins_configPtr[Pin].pin_num * 4));

        CLEAR_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);
    }
    else
    {
        /* For alternative functions:
           - Enable alternate function.
           - Set PMCx bits for the selected mode.
           - Enable digital functionality.
        */
        SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_ALT_FUNC_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);

        REGISTER(Port_BaseAdressPtr, PORT_CTL_REG_OFFSET) |= (0x0000000F <<
                  (pins_configPtr[Pin].pin_num * 4));

        SET_BIT(REGISTER(Port_BaseAdressPtr, PORT_DIGITAL_ENABLE_REG_OFFSET),
                  pins_configPtr[Pin].pin_num);
    }
}
#endif
