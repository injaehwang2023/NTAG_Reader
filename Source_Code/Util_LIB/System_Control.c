/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... System_Control.c
* DATE CREATED.. 12/30/2006 8:17PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/
#include "COMM_Protocol.h"
#include "System_Control.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "MCU_ISR.h"
#include "THUBAN_Bootloader.h"

char UART_Tx_Buffer[UART_TxMaxBufferSize];
char *pUART_Tx_Buffer;

void System_Control(char *pCommand_Data)
{
	unsigned char REGStatus;
	
	switch(*pCommand_Data++)
	{
		case BuzzerOn:
			if(*pCommand_Data)
				P3DIR |= BUZZER;
			else 
				P3DIR &= ~BUZZER;
		break;
		
		case RF_On:
			TRF796x_SOF();
    		REGStatus = TRF796x_ReadDataNotConti(Read + ChipStatusControl);
    		
    		if(*pCommand_Data)
    			REGStatus |= 0x20;
    		else
    			REGStatus &= 0xDF;
    		
    		TRF796x_SendDataNotConti(ADD, ChipStatusControl, REGStatus);
    		TRF796x_EOFNotConti();
		break;
		
		case Full_PowerMode:
			TRF796x_SOF();
    		REGStatus = TRF796x_ReadDataNotConti(Read + ChipStatusControl);
    		TRF796x_EOFNotConti();
            
    		if(*pCommand_Data)
    			REGStatus &= 0xEF;
    		else
    			REGStatus |= 0x10;
    		
    		TRF796xRHB_Buffer.ChipStatus = REGStatus;
            
		break;
		
		case System_Reset:
			IE1 &= ~WDTIE;
			WDTCTL = WDTPW + WDTHOLD;
			
            // To be system reset
			WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;
		break;
		
		case AGC_On:
			TRF796x_SOF();
    		REGStatus = TRF796x_ReadDataNotConti(Read + ChipStatusControl);
    		TRF796x_EOFNotConti();
            asm("nop");
            
    		if(*pCommand_Data)
    			REGStatus |= 0x04;
    		else
    			REGStatus &= 0xFB;
            
            TRF796xRHB_Buffer.ChipStatus = REGStatus;

		break;
		
		case THUBAN_Bootloader:
			_THUBAN_Bootloader(THUBAN_Bootloader);
		break;
	}
}

void SendToHost(char *pData, unsigned char Length)
{
	unsigned char i;
	unsigned char CheckSum;
	
	pUART_Tx_Buffer = UART_Tx_Buffer;
	pUART_RxBuffer = &UART_RxBuffer[0x03];		// 2007-01-05 11:34 PM
	
	i = Length + 0x07;
	
	*pUART_Tx_Buffer++ = SOF;
	*pUART_Tx_Buffer++ = i;
	*pUART_Tx_Buffer++ = Device_ID;
	
	*pUART_Tx_Buffer++ = *pUART_RxBuffer++;
	*pUART_Tx_Buffer++ = *pUART_RxBuffer;
	
	for(i = 0x00; i < Length; i++)
		*pUART_Tx_Buffer++ = *pData++;
	    
	CheckSum = 0x00;
	pUART_Tx_Buffer = UART_Tx_Buffer;
	
	for(i = 0x00; i < Length + 0x05; i++)
		CheckSum ^= *pUART_Tx_Buffer++;

	*pUART_Tx_Buffer++ = CheckSum;
	*pUART_Tx_Buffer = (CheckSum = ~CheckSum);
	
	pUART_Tx_Buffer = UART_Tx_Buffer;
	
	for(i = 0x00; i < Length + 0x07; i++)
	{
		while (!(IFG2 & UTXIFG0));
		TXBUF0 = *pUART_Tx_Buffer++;
	}	
}
