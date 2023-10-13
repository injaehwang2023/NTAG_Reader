/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... MCU_ISR.c
* DATE CREATED.. 12/24/2006 9:07AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/
/*******************************************************************************/
#include "COMM_Protocol.h"
#include "msp430x12x2.h"
#include "MCU_Init.h"
#include "MCU_Port_Define.h"
#include "MCU_ISR.h"
/*******************************************************************************/

/*******************************************************************************/
unsigned char LEDOnTime;
unsigned int ActiveModeCnt;
unsigned int FreeCnt;
unsigned char TagFound;
unsigned char TagFoundLEDOnTime;
unsigned char UART_TxBuffer_Temp;
unsigned char UART_Rx_Byte;
unsigned char UART_Rx_Finish;
char UART_RxBuffer[PacketLength];
char *pUART_RxBuffer;
/*******************************************************************************/

/*******************************************************************************/
// UART0 interrupt service routine
#pragma location="INFO"	
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_ISR(void)
{
	if(U0RCTL & RXERR) 
        UART_TxBuffer_Temp = RXBUF0;		// RXERR clear from RXBUF0 read.
	else									// Rx data correct!
	{
		if(!UART_Rx_Byte)
		{
			UART_Rx_Byte++;
			pUART_RxBuffer = UART_RxBuffer;
			*pUART_RxBuffer++ = RXBUF0;
			__low_power_mode_off_on_exit();
            return;
		}
		UART_Rx_Byte++;
		*pUART_RxBuffer++ = RXBUF0;
		
		if(UART_Rx_Byte == 0x0C)
		{
			UART_Rx_Finish = 0x01;
			UART_Rx_Byte = 0x00;
		}
	}
}
/*******************************************************************************/

/*******************************************************************************/
// WDT interrupt service routine
#pragma location="INFO"
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	FreeCnt++;
	if(!TagFound)
	{
		if(++ActiveModeCnt >= 1000)
		{
			P3OUT |= LED;
			if(++LEDOnTime >= 40)
			{
				ActiveModeCnt = 0x0000;
				LEDOnTime = 0x00;
				P3OUT &= ~LED;
			}
		}
	}
	else
		{
			P3OUT |= LED;
			P3OUT ^= BUZZER;
			if(++TagFoundLEDOnTime >= 100)
			{
				TagFound = 0x00;
				TagFoundLEDOnTime = 0x00;
				ActiveModeCnt = 0x0000;
				LEDOnTime = 0x00;
				P3OUT &= ~(BUZZER + LED);
			}
		}
}
/*******************************************************************************/
