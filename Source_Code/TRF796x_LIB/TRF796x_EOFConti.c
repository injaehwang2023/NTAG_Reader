/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... TRF796x_EOFConti.c
* DATE CREATED.. 12/15/2006 10:26AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

/*******************************************************************************/
#include "msp430x12x2.h"
#include "TRF796x_EOFConti.h"
#include "MCU_Port_Define.h"
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_Parallel
void TRF796x_EOFConti(void)
{
	P2DIR |= IO7;
    P2OUT |= IO7;
	
	P2OUT &= ~IO7;
}
#endif
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_WithOut_SS
void TRF796x_EOFConti(void)
{
    P2OUT |= IO7;
	
	P2OUT |= CLK;
	
	P2OUT &= ~IO7;
	
	P2OUT &= ~CLK;
}
#endif
/*******************************************************************************/
