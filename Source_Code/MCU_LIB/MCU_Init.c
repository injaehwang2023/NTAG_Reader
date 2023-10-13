/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... MCU_Init.c
* DATE CREATED.. 05/15 2006
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

#include "msp430x12x2.h"
#include "MCU_Init.h"
#include "TRF796x_LIB.h"
#include "MCU_Port_Define.h"

/*******************************************************************************/
#ifdef	TRF796xMode_Parallel
void MCU_Port_Init(void)
{
	
	P1OUT = 0x00;
	P2OUT = 0x00;
	P3OUT = 0x00;
	
	P1DIR = IO0 + IO1 + IO2 + IO3 + P1_NC0 + P1_NC1 + P1_NC2 + P1_NC3;
	P2DIR = IO4 + IO5 + IO6 + IO7 + CLK + P2_NC0 + P2_NC1;
	P3DIR = LED + BUZZER + UART_TXD + EN + TEST;
}
#endif
/*******************************************************************************/

/*******************************************************************************/
#ifdef	TRF796xMode_WithOut_SS
void MCU_Port_Init(void)
{
	
	P1OUT = IO2;
	P2OUT = 0x00;
	P3OUT = 0x00;
	
	P1DIR = IO0 + IO1 + IO2 + IO3 + P1_NC0 + P1_NC1 + P1_NC2 + P1_NC3;
	P2DIR = IO4 + IO7 + CLK + P2_NC0 + P2_NC1;
	P3DIR = LED + BUZZER + UART_TXD + EN + TEST;
}
#endif
/*******************************************************************************/


/*******************************************************************************/
void MCU_Clock_Init(void)
{
	unsigned int i;
	unsigned char LoopCnt;
	
	BCSCTL1 |= XTS;
	
	LoopCnt = 100;
	
	do
	{
		IFG1 &= ~OFIFG;
		for(i = 0x0000; i < 0x00FF; i++);
		
		if(!(IFG1 & OFIFG))
			break;
			
	}while(--LoopCnt);
	
	if(!LoopCnt)
	{
		for(i = 0x0000; i < 50000; i++);
		P3OUT ^= LED;							// High frequecy not stable or no suppply clock from TRF79xRHB
	}
    
    BCSCTL2 |= SELM_3;
}
/*******************************************************************************/

/*******************************************************************************/
void MCU_UART_Init(void)
{
    P3SEL = UART_TXD + UART_RXD;
  	ME2 |= UTXE0 + URXE0;                    // Enabled USART0 TXD/RXD
  	UCTL0 |= CHAR;                           // 8-bit character
  	UTCTL0 |= SSEL0;                         // UCLK = ACLK
  	UBR00 = 0xC2;                            // 9600BPS
  	UBR10 = 0x02;                            // 9600BPS
  	UMCTL0 = 0x00;                           // no modulation
  	UCTL0 &= ~SWRST;                         // Initalize USART state machine
}
/*******************************************************************************/

/*******************************************************************************/
void MCU_TimerA0_Init(void)
{
	// 100uS base
	TACCR0 = 678 - 1;
	TACCTL0 = 0x00;
	TACTL = TASSEL_1 + MC_0 + TACLR;
}
/*******************************************************************************/

/*******************************************************************************/
void MCU_TimerA0_Start(unsigned int TACCR0_Value)
{
	TACCTL0 = 0x0000;
	TACCR0 = TACCR0_Value;
	TACTL = TASSEL_1 + MC_1 + TACLR;
	
	while(!(TACCTL0 & CCIFG));
	
	TACCTL0 = 0x0000;
	TACTL = 0x0000;
}
/*******************************************************************************/

/*******************************************************************************/
void MCU_WDT_Init(void)
{
	WDTCTL = WDTPW + WDTTMSEL + WDTSSEL + WDTIS0;
	IE1 |= WDTIE;
}
/*******************************************************************************/
