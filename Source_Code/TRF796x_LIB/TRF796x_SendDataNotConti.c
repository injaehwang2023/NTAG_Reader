/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... TRF796x_SendDataNotConti.c
* DATE CREATED.. 12/15/2006 10:26AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

/*******************************************************************************/
#include "msp430x12x2.h"
#include "TRF796x_SendDataNotConti.h"
#include "MCU_Port_Define.h"
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_Parallel
void TRF796x_SendDataNotConti(unsigned char Mode, unsigned char Add, unsigned char Data)
{
	if(!Mode)
	{
		// Send Add
		P1DIR |= 0x0F;
		P2DIR |= 0x0F;
		P2OUT &= 0xF0;
		
		P1OUT = Add;
		Add >>= 0x04;
		
		P2OUT |= Add;
		
		P2OUT |= CLK;
		asm("nop");
		P2OUT &= ~CLK;
		
		// Send data
		P2OUT &= 0xF0;
		
		P1OUT = Data;
		Data >>= 0x04;
		
		P2OUT |= Data;
		
		P2OUT |= CLK;
		P2OUT &= ~CLK;
	}
	else
		{
			// Send CMD
			P1DIR |= 0x0F;
			P2DIR |= 0x0F;
			P2OUT &= 0xF0;
			
			P1OUT = Add;
			Add >>= 0x04;
			
			P2OUT |= Add;
			
			P2OUT |= CLK;
			P2OUT &= ~CLK;
		}
    
    P1OUT &= 0xF0;
    P2OUT &= 0xF0;
}
#endif
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_WithOut_SS
void TRF796x_SendDataNotConti(unsigned char Mode, unsigned char Add, unsigned char Data)
{
	unsigned char i;
	
	if(!Mode)
	{
		// Send Add
		for(i = 0x00; i < 0x08; i++)
		{
			if(Add & 0x80) P2OUT |= IO7;
				else P2OUT &= ~IO7;
			P2OUT |= CLK;
			Add <<= 0x01;
			P2OUT &= ~CLK;	
		}
		
		for(i = 0x00; i < 0x08; i++)
		{
			if(Data & 0x80) P2OUT |= IO7;
				else P2OUT &= ~IO7;
			P2OUT |= CLK;
			Data <<= 0x01;	
			P2OUT &= ~CLK;
		}
	}
	else
		{
			// Send CMD
			for(i = 0x00; i < 0x08; i++)
			{
				if(Add & 0x80) P2OUT |= IO7;
					else P2OUT &= ~IO7;
				P2OUT |= CLK;
				Add <<= 0x01;
				P2OUT &= ~CLK;	
			}
		}	
}
#endif
/*******************************************************************************/
