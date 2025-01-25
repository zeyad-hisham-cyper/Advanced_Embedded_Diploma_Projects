/*
 * Module: Port
 * File Name: Port.h
 * Description: Header File for AUTOSAR PORT Driver
 * Author: Zeyad Hisham
 */
s
#ifndef PORT_H_
#define PORT_H_

/* Id for the company in the AUTOSAR
 * for example Zeyad Hisham's ID = 1000 :) */

#define PORT_VENDOR_ID                      (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID                      (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID                    (0U)

/* Module Version 1.0.0 */
#define PORT_SW_RELEASE_MAJOR_VERSION               (1U)
#define PORT_SW_RELEASE_MINOR_VERSION               (0U)
#define PORT_SW_RELEASE_PATCH_VERSION               (0U)

/* AUTOSAR Version 4.0.3*/
#define PORT_AR_RELEASE_MAJOR_VERSION       (4U)
#define PORT_AR_RELEASE_MINOR_VERSION       (0U)
#define PORT_AR_RELEASE_PATCH_VERSION       (3U)

/* Macros for PORT Status */
#define PORT_INITIALIZED                    (1U)
#define PORT_NOT_INITIALIZED                (0U)

/* Include Port Types */
#include "Port_Types.h"

/* AUTOSAR Checking between Port Types and Port Module */
#if ((PORT_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Types.h doesn't match the expected version"
#endif
/* Software Version Check Between the Port_Cfg.h and Port.h files */
#if ((PORT_TYPES_SW_RELEASE_MAJOR_VERSION != PORT_SW_RELEASE_MAJOR_VERSION)\
 ||  (PORT_TYPES_SW_RELEASE_MINOR_VERSION != PORT_SW_RELEASE_MINOR_VERSION)\
 ||  (PORT_TYPES_SW_RELEASE_PATCH_VERSION != PORT_SW_RELEASE_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h doesn't match the expected version"
#endif


/* Include Pre-Compile Configuration Header File */
#include "Port_Cfg.h"

/* AUTOSAR Version Check Between the Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h doesn't match the expected version"
#endif

/* Software Version Check Between the Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_RELEASE_MAJOR_VERSION != PORT_SW_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_RELEASE_MINOR_VERSION != PORT_SW_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_SW_RELEASE_PATCH_VERSION != PORT_SW_RELEASE_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h doesn't match the expected version"
#endif

/* Non AUTOSAR Files */
#include "Common_Macros.h"

/******************************************************************************
 *                               API Service Id                               *
 ******************************************************************************/

/* Service ID for Port_Init API */
#define PORT_INIT_SID                       (uint8)0X00

/* Service ID for Port_SetPinDirection API */
#define PORT_SET_PIN_DIRECTION_SID          (uint8)0x01

/* Service ID for Port_RefreshPortDirection API */
#define PORT_REFRESH_PORT_DIRECTION_SID     (uint8)0x02

/* Service ID for Port_GetVersionInfo API */
#define PORT_GET_VERSION_INFO_SID           (uint8)0x03

/* Service ID for Port_SetPinMode API */
#define PORT_SET_PIN_MODE_SID               (uint8)0x04

/******************************************************************************
 *                              DET Error Codes                              *
 ******************************************************************************/

/* Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                    (uint8)0x0A

/* Port Pin not configured as changeable*/
#define PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B

/* API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                 (uint8)0x0C

/* API Port_SetPinMode service called with Invalid Mode */
#define PORT_E_PARAM_INVALID_MODE           (uint8)0x0D

/* API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE            (uint8)0x0E

/*API service called without module initialization*/
#define PORT_E_UNINIT                       (uint8)0x0F

/*APIs called with a Null Pointer*/
#define PORT_E_PARAM_POINTER                (uint8)0x10


#endif
