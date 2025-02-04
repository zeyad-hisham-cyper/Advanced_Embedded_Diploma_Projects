 /******************************************************************************
 *
 * Module: PORT
 *
 * File Name: Port_Types.h
 *
 * Description: Header file for Port Types - PORT Driver Registers
 *
 * Author: Zeyad Hisham
 ******************************************************************************/


#ifndef PORT_TYPES_H_
#define PORT_TYPES_H_

/* Module Version 1.0.0 */
#define PORT_TYPES_SW_RELEASE_MAJOR_VERSION           (1U)
#define PORT_TYPES_SW_RELEASE_MINOR_VERSION           (0U)
#define PORT_TYPES_SW_RELEASE_PATCH_VERSION           (0U)

/* AUTOSAR Version 4.0.3 */
#define PORT_TYPES_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_TYPES_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_TYPES_AR_RELEASE_PATCH_VERSION   (3U)

/* Include standard AUTOSAR types */
#include "Std_Types.h"


/* AUTOSAR Checking between Std Types and Port Module */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_TYPES_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_TYPES_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_TYPES_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h doesn't match the expected version"
#endif

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Description: Data type for the symbolic name of a port pin number */
TYPEDEF uint8 Port_PinType;

/* Description: Data type for the symbolic name of a port number */
TYPEDEF uint8 Port_PortType;

/* Description: Different port pin modes */
TYPEDEF uint8 Port_PinModeType;

/* Description: Enum for possible pin directions  */
TYPEDEF enum{
    PORT_PIN_IN,
    PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
TYPEDEF enum
{
    DISABLE_INTERNAL_RES,PULL_UP,PULL_DOWN
}Port_InternalResistorType;

/* Description: Enum to hold the pin level value */
TYPEDEF enum{
    PORT_PIN_LEVEL_HIGH,
    PORT_PIN_LEVEL_LOW
}Port_PinLevelValueType;

/* Description: Enum contain the modes list*/
TYPEDEF enum{
    PORT_PIN_MODE_ADC,
    PORT_PIN_MODE_CAN,
    PORT_PIN_MODE_DIO,
    PORT_PIN_MODE_DIO_GPT,
    PORT_PIN_MODE_DIO_WDG,
    PORT_PIN_MODE_FLEXRAY,
    PORT_PIN_MODE_ICU,
    PORT_PIN_MODE_LIN,
    PORT_PIN_MODE_MEM,
    PORT_PIN_MODE_PWM,
    PORT_PIN_MODE_SPI
}Port_PinInitialModeType;

/* Description: Structure to configure each individual PIN:
 *  1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *  2. the number of the pin in the PORT.
 *  3. the direction of pin --> INPUT or OUTPUT
 *  4. the internal resistor --> Disable, Pull up or Pull down
 *  5. the pin initial value --> High or Low
 *  6. the pin mode
 *  7. Is the direction changeable during the runtime --> True or False
 *  8. Is the mode changeable during the runtime --> True or False
 */

TYPEDEF struct
{
    Port_PortType port_num;
    Port_PinType pin_num;
    Port_PinDirectionType direction;
    Port_InternalResistorType resistor;
    Port_PinLevelValueType initial_value;
    Port_PinModeType mode;
    uint8 pin_direction_changeable;
    uint8 pin_mode_changeable;
}Port_ConfigPinType;

#endif /* PORT_TYPES_H_ */
