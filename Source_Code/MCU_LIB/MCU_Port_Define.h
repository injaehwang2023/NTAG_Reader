/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... MCU_Port_Define.h
* DATE CREATED.. 12 /15 2006
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#include "msp430x12x2.h"

#ifndef		__MCU_Port_Define__
#define		__MCU_Port_Define__

// *****************************************************************************/
#define		TRF796xMode_Parallel
//#define		TRF796xMode_WithOut_SS
// *****************************************************************************/

// *****************************************************************************/
// MSP430F1232RHB Port1 Define
#define		IO0				BIT0
#define		IO1				BIT1
#define		IO2				BIT2
#define		IO3				BIT3
#define		P1_NC0			BIT4
#define		P1_NC1			BIT5
#define		P1_NC2			BIT6
#define		P1_NC3			BIT7

// MSP430F1232RHB Port2 Define
#define		IO4				BIT0
#define		IO5				BIT1
#define		IO6				BIT2
#define		IO7				BIT3
#define		CLK				BIT4
#define		IRQ				BIT5
#define		P2_NC0			BIT6
#define		P2_NC1			BIT7

// MSP430F1232RHB Port3 Define
#define		MOD				BIT0
#define		OOK				BIT1
#define		LED				BIT2
#define		BUZZER			BIT3
#define		UART_TXD		BIT4
#define		UART_RXD		BIT5
#define		EN				BIT6
#define		TEST			BIT7
// *****************************************************************************/
#endif
