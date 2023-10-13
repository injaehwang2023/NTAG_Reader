/******************************************************************************\
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... TRF796x_Define.h
* DATE CREATED.. 12/15/2006 11:23AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


\******************************************************************************/

#ifndef		__TRF796x_Define_H_
#define		__TRF796x_Define_H_

// Main Control Registers
#define		ChipStatusControl				0x00
#define		ISOControl						0x01

// Protocol Sub-Setting Resisters
#define		IOS14443BTxOption				0x02
#define		IOS1444AHighBitRateOption		0x03
#define		EPCTxTimerSetting_MSB			0x04
#define		EPCTxTimerSetting_LSB			0x05
#define		TxPulseLengthControl			0x06
#define		RxNoResponseWait				0x07
#define		RxWaitTime						0x08
#define		ModulationControl				0x09
#define		RxSpecialSetting				0x0A
#define		RegulatorControl				0x0B
#define		EPCHashValue					0x15

//Status Registers
#define		IRQStatus						0x0C		// "Read only"
#define		CollisionPosition_IntMask   	0x0D		// "Read only"
#define		CollisionPosition				0x0E		// "Read only"
#define		RSSILevelsAndOSCStatus			0x0F		// "Read only"

// RAM(Read / Write)
#define		RAM0							0x10
#define		RAM1							0x11
#define		RAM2							0x12
#define		RAM3							0x13
#define		RAM4							0x14

// Test(Read / Write)
#define		TestSetting_1					0x1A
#define		TestSetting_2					0x1B

//FIFO Registers(Read / Write)
#define		FIFO_Status						0x1C		// "Read only"
#define		TXLengthByte_1					0x1D
#define		TXLengthByte_2					0x1E
#define		FIFO_IO_Resister				0x1F

// Command Codes
#define		Idle							0x00
#define		SoftInit						0x03
#define		Reset							0x0F
#define		TransWithoutCRC					0x10
#define		TransWithCRC					0x11
#define		DelayedTransWithoutCRC			0x12
#define		DelayedTransWithCRC				0x13
#define		TransmitNextSlot				0x14
#define		CloseSlotSequence				0x15
#define		BlockRX							0x16
#define		EnableRX						0x17
#define		CheckInternalRF					0x18
#define		CheckExternalRF					0x19
#define		ADJGain							0x1A

// Address/Command Word Bit Distribution
#define     CMD                         	0x80
#define     ADD                         	0x00
#define     Read                        	0x40
#define     Write                       	0x00
#define     Conti                       	0x20
#define     NotConti                   		0x00

// Error Code
#define		NonError						0x00
#define		HWError							0x01
#define		TAGNoResponse					0x02
#define		IRQError						0x03
#define		RxCntError						0x04
#define		ChecksumError					0x05
#define     ISO15693_Tag_BlcokWrite_Err     0x06
#define     ISO15693_Tag_Lock_Err           0x07
#endif
