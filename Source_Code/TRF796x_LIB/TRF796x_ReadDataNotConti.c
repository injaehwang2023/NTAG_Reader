/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... TRF796x_ReadDataNotConti.c
* DATE CREATED.. 12/22/2006 4:53PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

/*******************************************************************************/
#include "msp430x12x2.h"
#include "TRF796x_ReadDataNotConti.h"
#include "MCU_Port_Define.h"
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_Parallel
unsigned char TRF796x_ReadDataNotConti(unsigned char Add)
{
	unsigned char i,Byte;
	
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
	
	// Read data
	P1DIR &= 0xF0;
	P2DIR &= 0xF0;
    asm("nop");
    asm("nop");
	
	P2OUT |= CLK;
	asm("nop");
	i = P2IN;
	Byte = i & 0x0F;
	Byte <<= 0x04;
	
	i = P1IN & 0x0F;
	Byte |= i;
	P2OUT &= ~CLK;
    
    P1OUT &= 0xF0;
    P2OUT &= 0xF0;
    P1DIR |= 0x0F;
	P2DIR |= 0x0F;
	
	return Byte;
}
#endif
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_WithOut_SS
unsigned char TRF796x_ReadDataNotConti(unsigned char Add)
{
	unsigned char i,Byte;
	
	// Send Add
	for(i = 0x00; i < 0x08; i++)
	{
		if(Add & 0x80) P2OUT |= IO7;
			else P2OUT &= ~IO7;
				
		P2OUT |= CLK;
		Add <<= 0x01;
		P2OUT &= ~CLK;	
	}
	
	Byte = 0x00;
	for(i = 0x00; i < 0x08; i++)
	{
		P2OUT |= CLK;
		
		Byte <<= 0x01;
		if((P2IN & IO6) == IO6) Byte |= 0x01;
		
		P2OUT &= ~CLK;	
	}
	return Byte;
}
#endif
/*******************************************************************************/

