/*******************************************************************************
* File Name: green.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_green_H) /* Pins green_H */
#define CY_PINS_green_H

#include "cytypes.h"
#include "cyfitter.h"
#include "green_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    green_Write(uint8 value) ;
void    green_SetDriveMode(uint8 mode) ;
uint8   green_ReadDataReg(void) ;
uint8   green_Read(void) ;
uint8   green_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define green_DRIVE_MODE_BITS        (3)
#define green_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - green_DRIVE_MODE_BITS))

#define green_DM_ALG_HIZ         (0x00u)
#define green_DM_DIG_HIZ         (0x01u)
#define green_DM_RES_UP          (0x02u)
#define green_DM_RES_DWN         (0x03u)
#define green_DM_OD_LO           (0x04u)
#define green_DM_OD_HI           (0x05u)
#define green_DM_STRONG          (0x06u)
#define green_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define green_MASK               green__MASK
#define green_SHIFT              green__SHIFT
#define green_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define green_PS                     (* (reg32 *) green__PS)
/* Port Configuration */
#define green_PC                     (* (reg32 *) green__PC)
/* Data Register */
#define green_DR                     (* (reg32 *) green__DR)
/* Input Buffer Disable Override */
#define green_INP_DIS                (* (reg32 *) green__PC2)


#if defined(green__INTSTAT)  /* Interrupt Registers */

    #define green_INTSTAT                (* (reg32 *) green__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define green_DRIVE_MODE_SHIFT       (0x00u)
#define green_DRIVE_MODE_MASK        (0x07u << green_DRIVE_MODE_SHIFT)


#endif /* End Pins green_H */


/* [] END OF FILE */
