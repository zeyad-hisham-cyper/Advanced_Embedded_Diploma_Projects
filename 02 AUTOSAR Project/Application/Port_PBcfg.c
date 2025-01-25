/*
 * Port_PBcfg.c
 *
 *  Created on: Jan 25, 2025
 *      Author: zeyad
 */


/* Module Version 1.0.0 */
#define PORT_PBFG_SW_RELEASE_MAJOR_VERSION   (1U)
#define PORT_PBFG_SW_RELEASE_MINOR_VERSION   (0U)
#define PORT_PBFG_SW_RELEASE_PATCH_VERSION   (0U)

/* AUTOSAR Version 4.0.3 */
#define PORT_PBFG_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_PBFG_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_PBFG_AR_RELEASE_PATCH_VERSION   (3U)


#include "Port.h"
/* AUTOSAR Version Check Between the Port_PBfg.h and Port.h files */
#if ((PORT_PBFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_PBfg.h doesn't match the expected version"
#endif

/* Software Version Check Between the Port_PBfg.h and Port.h files */
#if ((PORT_PBFG_SW_RELEASE_MAJOR_VERSION != PORT_SW_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBFG_SW_RELEASE_MINOR_VERSION != PORT_SW_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBFG_SW_RELEASE_PATCH_VERSION != PORT_SW_RELEASE_PATCH_VERSION))
  #error "The SW version of Port_PBfg.h doesn't match the expected version"
#endif

const Port_ConfigType Pins_configuration = {
                                            /* Port A Pins */
                                            PORT_A_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN1_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN4_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN5_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN6_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_A_ID, PORT_PIN7_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,

                                            /* Port B Pins */
                                            PORT_B_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN1_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN4_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN5_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN6_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_B_ID, PORT_PIN7_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,

                                            /* Port C Pins */
                                            PORT_C_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN1_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN4_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN5_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN6_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_C_ID, PORT_PIN7_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,

                                            /* Port D Pins */
                                            PORT_D_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN1_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN4_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN5_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN6_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_D_ID, PORT_PIN7_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,

                                            /* Port E Pins */
                                            PORT_E_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_E_ID, PORT_PIN1_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_E_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_E_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_E_ID, PORT_PIN4_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_E_ID, PORT_PIN5_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,

                                            /* Port F Pins */
                                            PORT_F_ID, PORT_PIN0_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_F_ID, PORT_PIN1_ID,PORT_PIN_OUT,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON, /* LED Pin */
                                            PORT_F_ID, PORT_PIN2_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_F_ID, PORT_PIN3_ID,PORT_PIN_IN,DISABLE_INTERNAL_RES, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,
                                            PORT_F_ID, PORT_PIN4_ID,PORT_PIN_IN,PULL_UP, PORT_PIN_LEVEL_LOW, PORT_PIN_MODE_DIO,STD_ON,STD_ON,               /* Button Pin */
};
