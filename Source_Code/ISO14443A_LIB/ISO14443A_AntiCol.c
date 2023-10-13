/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... ISO14443A_AntiCol.c
* DATE CREATED.. 12/15/2006 3:47PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/

#include "msp430x12x2.h"
#include "TRF796x_Define.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "ISO14443A_LIB.h"
#include "TRF796x_LIB.h"
#include "MCU_ISR.h"
#include "MCU_Init.h"
#include "string.h"

ISO14443A_AntiCol_Config__ ISO14443A_AntiCol_Config;
char ISO14443A_UID[ISO14443A_UID_Buffer_Cnt][ISO14443A_UID_Size];
char TempBuffer[5];

unsigned char ISO14443A_AntiCol(char *pISO14443A_UID)
{	
	unsigned char REGStatus,TimeOutCnt;
	unsigned char i;
	unsigned char Temp,Temp1;
	
	REGStatus = 0x00;
	
	ISO14443A_AntiCol_Config.pTempBuffer = TempBuffer;	
	TRF796xRHB_Buffer.pTxBuffer = TRF796xRHB_TxBuffer;
	
	if(!ISO14443A_AntiCol_Config.Collision)
	{
		// 1 card
		#if 1
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x00;
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x20;
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x93;
		*TRF796xRHB_Buffer.pTxBuffer = 0x20;
		ISO14443A_AntiCol_Config.SendByte = 0x04;
		#endif
		
		// Test only
		#if 0
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x00;
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x2B;
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x93;
		*TRF796xRHB_Buffer.pTxBuffer++ = 0x25;
		*TRF796xRHB_Buffer.pTxBuffer = 0x16;
		ISO14443A_AntiCol_Config.SendByte = 0x05;
		#endif
	}
	else
		{
			*TRF796xRHB_Buffer.pTxBuffer++ = 0x00;
			
			Temp = Temp1 = ISO14443A_AntiCol_Config.NVB;
			Temp &= 0x0F; Temp <<= 0x01; Temp |= 0x01;
			Temp1 &= 0xF0;
			
			*TRF796xRHB_Buffer.pTxBuffer++ =  Temp1 + Temp;
			*TRF796xRHB_Buffer.pTxBuffer++ = 0x93;
			*TRF796xRHB_Buffer.pTxBuffer++ = ISO14443A_AntiCol_Config.NVB;

			Temp1 -= 0x20;
			ISO14443A_AntiCol_Config.SendByte = 0x04;
			
			// 2007-01-07 8:13PM
			// Invalide Handle...
			if(Temp1 > 0x40)
			    return RxCntError;
            
            Temp1 >>= 0x04; Temp1++;
            
			for(i = 0x00; i < Temp1; i++)
			{
				*TRF796xRHB_Buffer.pTxBuffer++ = *ISO14443A_AntiCol_Config.pTempBuffer++;
				ISO14443A_AntiCol_Config.SendByte++;
			}
		}
	// TRF796xRHB SOF
	TRF796x_SOF();
	// TRF796xRHB Setup ISO14443A Type and no RX CRC
	TRF796x_SendDataNotConti(ADD, ISOControl, 0x88);
	// TRF796xRHB Interrupt mask enable
	TRF796x_SendDataNotConti(ADD, CollisionPosition_IntMask, 0x1E);
	
	// TRF796xRHB FIFO & Interrupt Flag Clear...
	TRF796x_SendDataNotConti(CMD, Reset + CMD, 0x00);
	// TRF796xRHB Transmitter without CRC
	TRF796x_SendDataNotConti(CMD, TransWithoutCRC + CMD, 0x00);
	// TRF796xRHB FIFO Write
	TRF796x_SendDataConti(TXLengthByte_1 + Conti, TRF796xRHB_TxBuffer, ISO14443A_AntiCol_Config.SendByte);
	TRF796x_EOFConti();
    
    REGStatus = 0x00;
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
		{
			// IRQ Error Check
			if(0x1D & REGStatus)
				return IRQError;
			// Collision Error Check
			if(0x02 & REGStatus)
			{
				ISO14443A_AntiCol_Config.pTempBuffer = TempBuffer;
                
                TRF796x_SOF();
				REGStatus = TRF796x_ReadDataNotConti(Read + CollisionPosition_IntMask);
                
				ISO14443A_AntiCol_Config.NVB = TRF796x_ReadDataNotConti(Read + CollisionPosition);
                
				REGStatus = (0x0F & TRF796x_ReadDataNotConti(Read + FIFO_Status));
                
    			TRF796x_ReadDataConti(FIFO_IO_Resister + Read + Conti, ISO14443A_AntiCol_Config.pTempBuffer, ++REGStatus);
    			TRF796x_EOFConti();
    			                
    			Temp = ++ISO14443A_AntiCol_Config.NVB;
                
    			Temp &= 0x07;
                
                ISO14443A_AntiCol_Config.NVB &= 0xF0;
                ISO14443A_AntiCol_Config.NVB += Temp;
                
    			Temp1 = 0x00;
                
    			for(i = 0x00; i < Temp; i++)
    			{
					Temp1 <<= 0x01; Temp1 |= 0x01;
    			}
    			
                ISO14443A_AntiCol_Config.pTempBuffer = TempBuffer;
    			*(ISO14443A_AntiCol_Config.pTempBuffer + REGStatus - 1) &= Temp1;
                
                ISO14443A_AntiCol_Config.pTempBuffer = TempBuffer;
                
                if(ISO14443A_AntiCol_Config.Collision)
                {
                    for(i = 0x00; i < 0x05; i++)
                        *ISO14443A_AntiCol_Config.pTempBuffer++ |= pISO14443A_UID[i];
                }
                memcpy(pISO14443A_UID,TempBuffer,sizeof TempBuffer);
                
    			ISO14443A_AntiCol_Config.Collision = 0x01;
    			
    			//500uS Delay
    			MCU_TimerA0_Start(3350);					
    			
    			if((P2IN & IRQ) == IRQ)
    			{
	    			TRF796x_SOF();
	    			REGStatus = TRF796x_ReadDataNotConti(Read + IRQStatus);
	    			TRF796x_EOFNotConti();
    			}
    			return NonError;
			}
			else 
				// Error Found
				return IRQError;
		}
	}
    else 
        return TAGNoResponse;
	
	TRF796x_SOF();
    REGStatus = TRF796x_ReadDataNotConti(Read + FIFO_Status);
    TRF796x_EOFNotConti();
    REGStatus &= 0x0F;
    
    if(!ISO14443A_AntiCol_Config.Collision)
    {
	    if(0x05 != ++REGStatus)
	    	// UID is always 5byte(UID 4Byte + XOR Checksum 1Byte).
	    	return RxCntError;
    }
    
    for(i = 0x00; i < 0x05; i++)
        pISO14443A_UID[i] = 0x00;
    
    if(ISO14443A_AntiCol_Config.Collision)
    {
        ISO14443A_AntiCol_Config.NVB &= 0xF0;
        ISO14443A_AntiCol_Config.NVB >>= 0x04;
        ISO14443A_AntiCol_Config.NVB -= 0x02;
    }
    else
        ISO14443A_AntiCol_Config.NVB = 0x00;
    
    TRF796x_SOF();
    TRF796x_ReadDataConti(FIFO_IO_Resister + Read + Conti, (pISO14443A_UID + ISO14443A_AntiCol_Config.NVB), ++REGStatus);
    TRF796x_EOFConti();
    
    if(ISO14443A_AntiCol_Config.Collision)
    {
        ISO14443A_AntiCol_Config.SendByte -= 0x04;
        for(i = 0x00; i < ISO14443A_AntiCol_Config.SendByte; i++)
            pISO14443A_UID[i] |= TempBuffer[i];
    }
    
    Temp = 0x00;
    
    for(i = 0x00; i < 4; i++)
    	Temp ^= pISO14443A_UID[i];
    	
    if(pISO14443A_UID[i] != Temp)
        return ChecksumError;
    else
    {
    	if(ISO14443A_AntiCol_Config.Collision)
            ISO14443A_AntiCol_Config.Collision = 0x00;
        
        return NonError;
    }
}
