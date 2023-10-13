/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... THUBAN_Bootloader.c
* DATE CREATED.. 12/30/2006 8:17PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

#include "msp430x12x2.h"
#include "MCU_ISR.h"
#include "THUBAN_Bootloader.h"

#pragma location="THUBAN_Bootloader"	
void _THUBAN_Bootloader(unsigned char Mode)
{
	unsigned char i;
	unsigned char Size;
	unsigned char CheckSum;
	unsigned int FlashAddress;
	char *Flash_Pointer;
	
	// "TempBuffer[3]" -> TempBuffer[0] = Flash Address MSB, TempBuffer[1] = Flash Address LSB, TempBuffer[2] = Program Size
	char TempBuffer[3];
	
	// Global Interrupt Disable
	__disable_interrupt();
	
	pUART_RxBuffer = UART_RxBuffer;
	
	// WDT Interrupt Disable & Stop
	IE1 &= ~WDTIE;
	WDTCTL = WDTPW + WDTHOLD;
	
	// TimerA0 Stop
	TACTL = 0x0000;
	TACCTL0 = 0x0000;
    
    if(THUBAN_Bootloader != Mode)
    {
        // Invalid Function call..
        WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;
			// To be system reset
		for(;;);
    }
	
	//ACLK, 356.8KHz, Flash controller clock divider = 0x13
	FCTL2 = FWKEY + FSSEL_0 + 0x13;

	FlashAddress = 0xE200;
	
	// Flash Memory Erase(MSP430F1232 only!!)
	for(i = 0x00; i < 15; i++)
	{
		Flash_Pointer = (char *)FlashAddress;
			
		FCTL1 = FWKEY + ERASE;
		FCTL3 = FWKEY;     
		*Flash_Pointer = 0;             
		
		FCTL1 = FWKEY;                 
		FCTL3 = FWKEY + LOCK;  	
		
		FlashAddress += 0x200;
	}
	
	while (!(IFG2 & UTXIFG0));
	TXBUF0 = FlashEraseOkay;
	
	for(;;)
	{
		for(i = 0x00; i < 3; i++)
		{
			while(!(URXIFG0 & IFG2));
			TempBuffer[i] = RXBUF0;
		}
		
		Size = TempBuffer[2];
		
		do
		{
			while(!(URXIFG0 & IFG2));
			*pUART_RxBuffer++ = RXBUF0;
		}while(--Size);
		
		Size = TempBuffer[2];
		
		CheckSum = 0x00;
		
		pUART_RxBuffer = UART_RxBuffer;
		
		// Data CheckSum Verify
		for(i = 0x00; i < Size - 1; i++)
			CheckSum ^= *pUART_RxBuffer++;
		
		if(CheckSum != *pUART_RxBuffer)
		{
			// CheckSum Error
			while (!(IFG2 & UTXIFG0));
			TXBUF0 = CheckSumError;
			continue;
		}
		
		// Flash Program
		FlashAddress = TempBuffer[0];
		FlashAddress <<= 0x08;
		FlashAddress |= TempBuffer[1];
		
		pUART_RxBuffer = UART_RxBuffer;
		Flash_Pointer = (char *)FlashAddress;
		
		FCTL3 = FWKEY;     
		FCTL1 = FWKEY + WRT;           
		
		for(i = 0x00; i < Size - 1; i++)
			*Flash_Pointer++ = *pUART_RxBuffer++;
		
		FCTL1 = FWKEY;                 
		FCTL3 = FWKEY + LOCK;  
		
		while (!(IFG2 & UTXIFG0));
		TXBUF0 = ProgramOkay;       
		
		// Flash Program Finish..
		if(FlashAddress == 0xFFE0)
		{
			WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;
			// To be system reset
			for(;;);
		}
		else continue;
	}
}
