/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... ISO14443A_SEL.c
* DATE CREATED.. 12/29/2006 11:50AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

#include "msp430x12x2.h"
#include "TRF796x_Define.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "ISO14443A_SEL.h"
#include "TRF796x_LIB.h"
#include "MCU_ISR.h"
#include "MCU_Init.h"

unsigned char ISO14443A_SEL(char *pUID_Buffer)
{	
	unsigned char REGStatus, TimeOutCnt;
	char SAK;

	TRF796xRHB_Buffer.pTxBuffer = TRF796xRHB_TxBuffer;
	
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x00;
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x70;
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x93;
	*TRF796xRHB_Buffer.pTxBuffer++ = 0x70;
	*TRF796xRHB_Buffer.pTxBuffer++ = *pUID_Buffer++;
	*TRF796xRHB_Buffer.pTxBuffer++ = *pUID_Buffer++;
	*TRF796xRHB_Buffer.pTxBuffer++ = *pUID_Buffer++;
	*TRF796xRHB_Buffer.pTxBuffer++ = *pUID_Buffer++;
	*TRF796xRHB_Buffer.pTxBuffer = *pUID_Buffer;
	
    P3OUT |= TEST;    
	// TRF796xRHB SOF
	TRF796x_SOF();
	// TRF796xRHB Setup ISO14443A Type
	TRF796x_SendDataNotConti(ADD, ISOControl, 0x08);
	// TRF796xRHB FIFO & Interrupt Flag Clear...
	TRF796x_SendDataNotConti(CMD, Reset + CMD, 0x00);
	// TRF796xRHB CollisionPosition_IntMask Setup
	TRF796x_SendDataNotConti(ADD, CollisionPosition_IntMask, 0x1E);
	// TRF796xRHB TransWithCRC
	TRF796x_SendDataNotConti(CMD, TransWithCRC + CMD, 0x00);
	// TRF796xRHB FIFO Write
	TRF796x_SendDataConti(TXLengthByte_1 + Conti, TRF796xRHB_TxBuffer, 0x09);
	TRF796x_EOFConti();
    
    REGStatus = 0x00;
    TimeOutCnt = 0x50;
    
    do
    {
    	MCU_TimerA0_Start(67);					//10uS Delay
    	if((P2IN & IRQ) == IRQ)
    	{
    		// Tx Finished interrupt
            P3OUT &= ~TEST;
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

    TimeOutCnt = 0x50; 
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
    
    if(0x01 != ++REGStatus)
    	// SAK is always 1byte.
    	return RxCntError;
    
    TRF796x_SOF();
    TRF796x_ReadDataConti(FIFO_IO_Resister + Read + Conti, &SAK, REGStatus);
    TRF796x_EOFConti();
    asm("nop");
    
    return NonError;
}
