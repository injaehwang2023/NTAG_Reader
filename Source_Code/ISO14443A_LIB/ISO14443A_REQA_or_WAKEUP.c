/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... REQA.c
* DATE CREATED.. 12/15/2006 3:47PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

#include "msp430x12x2.h"
#include "TRF796x_Define.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "ISO14443A_REQA_or_WAKEUP.h"
#include "TRF796x_LIB.h"
#include "MCU_ISR.h"
#include "MCU_Init.h"

char ISO14443A_ATQA[2];

unsigned char ISO14443A_REQA_or_WAKEUP(unsigned char Mode, char *pISO14443A_ATQA)
{	
	unsigned char REGStatus, TimeOutCnt;
		
	TRF796xRHB_Buffer.pTxBuffer = TRF796xRHB_TxBuffer;
	
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x00;
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x0F;
	if(Mode == REQA)
		*TRF796xRHB_Buffer.pTxBuffer = REQA;
	else 
		*TRF796xRHB_Buffer.pTxBuffer = WAKEUP;
		
	// TRF796xRHB SOF
	TRF796x_SOF();
	// TRF796xRHB Setup ISO14443A Type
	TRF796x_SendDataNotConti(ADD, ISOControl, 0x88);
	// TRF796xRHB
	TRF796x_SendDataNotConti(ADD, CollisionPosition_IntMask, 0x1F);
	// TRF796xRHB FIFO & Interrupt Flag Clear...
	TRF796x_SendDataNotConti(CMD, Reset + CMD, 0x00);
	// TRF796xRHB Transmitter without CRC
	TRF796x_SendDataNotConti(CMD, TransWithoutCRC + CMD, 0x00);
	// TRF796xRHB FIFO Write
	TRF796x_SendDataConti(TXLengthByte_1 + Conti, TRF796xRHB_TxBuffer, 0x03);
	TRF796x_EOFConti();
    
    REGStatus = 0x00;
    TimeOutCnt = 0x32;
    
    do
    {
    	MCU_TimerA0_Start(67);					//10uS Delay
    	if((P2IN & IRQ) == IRQ)
    	{
    		// Tx Finished interrupt
            TRF796x_SOF();
    		REGStatus = TRF796x_ReadDataNotConti(Read + IRQStatus);
    		TRF796x_EOFNotConti();
    		asm("nop");
    		break;
    	}
    }while(--TimeOutCnt);
    
    if(REGStatus)
    	REGStatus = 0x00;
    else
    	// TRF796xRHB Hardware error
    	return HWError;		

    TimeOutCnt = 0x32; 
    do
    {
    	MCU_TimerA0_Start(67);					//10uS Delay
    	if((P2IN & IRQ) == IRQ)
    	{
    		// Rx or the other interrupt
            TRF796x_SOF();
    		REGStatus = TRF796x_ReadDataNotConti(Read + IRQStatus);
    		TRF796x_EOFNotConti();
    		asm("nop");
    		break;
    	}
    }while(--TimeOutCnt);

    if(!REGStatus)
    	// PICC no response
    	return TAGNoResponse;        

	if((0x40 & REGStatus) == 0x40)
	{
		if(0x1F & REGStatus)
			// Error Found
			return IRQError;
	}
    else
        return TAGNoResponse;
	
	TRF796x_SOF();
    REGStatus = TRF796x_ReadDataNotConti(Read + FIFO_Status);
    TRF796x_EOFNotConti();
    REGStatus &= 0x0F;
    asm("nop");
    
    if(0x02 != ++REGStatus)
    	// ATQA is always 2byte.
    	return RxCntError;
    
    TRF796x_SOF();
    TRF796x_ReadDataConti(FIFO_IO_Resister + Read + Conti, pISO14443A_ATQA, REGStatus);
    TRF796x_EOFConti();
    asm("nop");
    
    return NonError;
}
