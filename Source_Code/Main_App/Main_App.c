/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... Main_App.c
* DATE CREATED.. 12/15/2006 10:56AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/
#include "COMM_Protocol.h"
#include "msp430x12x2.h"
#include "MCU_Init.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "ISO14443A_LIB.h"
#include "ISO14443B_LIB.h"
#include "ISO15693_LIB.h"
#include "MCU_ISR.h"
#include "stdio.h"
#include "System_Control.h"
#include "THUBAN_Bootloader.h"
/*******************************************************************************/

/*******************************************************************************/
unsigned char RunMode;
/*******************************************************************************/

/*******************************************************************************/
void main(void)
{
    unsigned char i;
    unsigned char REGStatus;
    unsigned char TempBuffer;
    
    // MSP430F1232RHB I/O Port Init
	MCU_Port_Init();
    
    //TRF796x wake up from Low Power Mode
    P3OUT |= EN;					
	
	// TRF796x crystal stable Check
	i = 100;
    do
    {
    	TRF796x_SOF();
    	REGStatus = TRF796x_ReadDataNotConti(Read + RSSILevelsAndOSCStatus);
    	TRF796x_EOFNotConti();
    	
    	// Crystal stable ?
    	if((0x40 & REGStatus) == 0x40)
    		break;
    }while(--i);
        
	TRF796x_SOF();
	// TRF796x Clock setup 6.78MHz
    TRF796x_SendDataNotConti(ADD, ModulationControl, 0x29);
    // TRF796x Manual Regulator Setting(Maximum VDD_RF)
    TRF796x_SendDataNotConti(ADD, RegulatorControl, 0x07);
    // TRF796x Test Setting(Subcarrier out to MOD)
    TRF796x_SendDataNotConti(ADD, TestSetting_1, 0x40);
    TRF796x_EOFNotConti();
    
    // MSP430F1232RHB Switch DCO to High Frequecny
	MCU_Clock_Init();
	// MSP430F1232RHB UART Init(9600BPS, 8Bit, 1Stop Bit, Non Parity)
	MCU_UART_Init();
	// MSP430F1232RHB TimerA0 Init(Up mode, TimerA0 Stop)
	MCU_TimerA0_Init();
	// MSP430F1232RHB WDT Init(Interval Timer Mode 1.208mS Base)
	MCU_WDT_Init();
	
	// First Run mode is Local mode...
	RunMode = LocalMode;
    	
	TRF796xRHB_Buffer.ChipStatus = 0x21;
		
	__enable_interrupt();
	
	for(;;)
	{
		if(LocalMode == RunMode)
		{
			if((IFG2 & URXIFG0) == URXIFG0)
            {
                if(HostSyncCode == RXBUF0)
                {
                    IE2 |= URXIE0;
                    while (!(IFG2 & UTXIFG0));
                    TXBUF0 =  LocalSyncCode;
                    RunMode = HostMode;
                    // Host Mode Detected...
                    TagFound = 0x01;
                }
                continue;
            }
			else
				{
					//Run Mode is Local...
					// TRF796x RF On
					TRF796x_SOF();
				    TRF796x_SendDataNotConti(ADD, ChipStatusControl, TRF796xRHB_Buffer.ChipStatus);
				    TRF796x_EOFNotConti();
                    
				    MCU_TimerA0_Start(0xFFFF);                     // 9.6mS Delay
				    
                    TRF796x_SOF();
    		        TempBuffer = TRF796x_ReadDataNotConti(Read + IRQStatus);
    		        TRF796x_EOFNotConti();
            
				    TempBuffer = TAGNoResponse;
				    
				    // ISO14443A type card check
				    if(!ISO14443A_Handle())
				    	TempBuffer = 0x00;
				    
				    // ISO15693 type card check
				    if(!ISO15693_Handle(ISO15693_UID_Read_16Slot))
				    	TempBuffer = 0x00;
				    
				    // ISO14443B type card check
				    if(!ISO14443B_Handle())
				    	TempBuffer = 0x00;
				    
				    // Any Card Found ?
				    if(!TempBuffer)
						TagFound = 0x01;
						
					// TRF796x RF Off
					TRF796x_SOF();
				    TRF796x_SendDataNotConti(ADD,ChipStatusControl, 0x01);
				    TRF796x_EOFNotConti();
				    
				    //200mS Delay
				    for(i = 0x00; i < 200; i++)
				    	MCU_TimerA0_Start(6780);			// 1mS Delay	
				    continue;
				}
		}
		
		// MSP430F1232RHB enter the low power mode 0
		__low_power_mode_0();
		
        // Run Mode is host...
		// UART Rx Data timeout Check...		
		i = 50;
		do
		{
			MCU_TimerA0_Start(6780);			// 1mS Delay
			if(UART_Rx_Finish)
			{
				UART_Rx_Byte = 0x00;
                UART_Rx_Finish = 0x00;
				break;
			}
		}while(--i);
		
		if(!i)
		{
			// UART Rx Time out...
			UART_Rx_Byte = 0x00;
			UART_Rx_Finish = 0x00;
			continue;
		}
		
		TempBuffer = 0x00;
		pUART_RxBuffer = UART_RxBuffer;
		
		for(i = 0x00; i < 0x0C - 2; i++)
			TempBuffer ^= *pUART_RxBuffer++;
			
		//	Verify Checksum
		if((TempBuffer != *pUART_RxBuffer++) || ((TempBuffer = ~TempBuffer) != *pUART_RxBuffer))
			continue;
		
		pUART_RxBuffer = UART_RxBuffer;
		
		// UART Rx data structure check
		if((*pUART_RxBuffer++ != SOF) || (*pUART_RxBuffer++ != 0x0C) || (*pUART_RxBuffer++ != Device_ID))
			continue;
		
		TempBuffer = TAGNoResponse;
		
		// TRF796x RF On
		TRF796x_SOF();
	    TRF796x_SendDataNotConti(ADD, ChipStatusControl, TRF796xRHB_Buffer.ChipStatus);
	    TRF796x_EOFNotConti();
	    
	    MCU_TimerA0_Start(0xFFFF);                     // 9.6mS Delay
	                        
		switch(*pUART_RxBuffer++)
		{
			case ISO14443A_Mode:
				TempBuffer = ISO14443A_Handle();
			break;
			
			case ISO14443B_Mode:
				TempBuffer = ISO14443B_Handle();
			break;
			
			case ISO15693_Mode:
				TempBuffer = ISO15693_Handle(*pUART_RxBuffer);
			break;
			
			case SystemConfig_Mode:
				System_Control(pUART_RxBuffer);
			break;
		}
		
		// TRF796x RF Off
        i = TRF796xRHB_Buffer.ChipStatus;
		i &= ~0x20;
		TRF796x_SOF();
	    TRF796x_SendDataNotConti(ADD,ChipStatusControl, i);
        i = TRF796x_ReadDataNotConti(Read + IRQStatus);
	    TRF796x_EOFNotConti();
	    
		if(!TempBuffer)
			TagFound = 0x01;   
	}
}
/*******************************************************************************/
