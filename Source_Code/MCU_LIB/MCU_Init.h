/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... MCU_Init.h
* DATE CREATED.. 12/15/2006 10:53AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__MCU_Init_Define__
#define		__MCU_Init_Define__

#include "msp430x12x2.h"

/*******************************************************************************/
extern void MCU_Port_Init(void);
extern void MCU_Clock_Init(void);
extern void MCU_UART_Init(void);
extern void MCU_TimerA0_Init(void);
extern void MCU_TimerA0_Start(unsigned int TACCR0_Value);
extern void MCU_WDT_Init(void);
/*******************************************************************************/

#endif
