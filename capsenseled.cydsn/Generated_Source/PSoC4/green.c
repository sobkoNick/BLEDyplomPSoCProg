/*******************************************************************************
* File Name: green.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "green.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        green_PC =   (green_PC & \
                                (uint32)(~(uint32)(green_DRIVE_MODE_IND_MASK << (green_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (green_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: green_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void green_Write(uint8 value) 
{
    uint8 drVal = (uint8)(green_DR & (uint8)(~green_MASK));
    drVal = (drVal | ((uint8)(value << green_SHIFT) & green_MASK));
    green_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: green_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  green_DM_STRONG     Strong Drive 
*  green_DM_OD_HI      Open Drain, Drives High 
*  green_DM_OD_LO      Open Drain, Drives Low 
*  green_DM_RES_UP     Resistive Pull Up 
*  green_DM_RES_DWN    Resistive Pull Down 
*  green_DM_RES_UPDWN  Resistive Pull Up/Down 
*  green_DM_DIG_HIZ    High Impedance Digital 
*  green_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void green_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(green__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: green_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro green_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 green_Read(void) 
{
    return (uint8)((green_PS & green_MASK) >> green_SHIFT);
}


/*******************************************************************************
* Function Name: green_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 green_ReadDataReg(void) 
{
    return (uint8)((green_DR & green_MASK) >> green_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(green_INTSTAT) 

    /*******************************************************************************
    * Function Name: green_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 green_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(green_INTSTAT & green_MASK);
		green_INTSTAT = maskedStatus;
        return maskedStatus >> green_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
