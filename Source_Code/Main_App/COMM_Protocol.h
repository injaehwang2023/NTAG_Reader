/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... COMM_Protocol.h
* DATE CREATED.. 12/30/2006 11:57AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__COMM_Protocol_
#define		__COMM_Protocol_

#define		HostSyncCode					0xAA
#define		LocalSyncCode					0x55
#define		UART_TxMaxBufferSize			0x14

#define		HostMode						0x01
#define		LocalMode						0x00

#define		SOF								0x01
#define		PacketLength					0x44
#define		Device_ID						0x02

#define		ISO14443A_Mode					0x01
	#define		ISO14443A_UID_Read				0x01
	
#define		ISO14443B_Mode					0x02
	#define		ATQA_Read						0x01
	
#define		ISO15693_Mode					0x03
	#define		ISO15693_UID_Read_1Slot			0x01
	#define		ISO15693_UID_Read_16Slot		0x02
	#define		ISO15693_Memory_Write			0x03
	#define		ISO15693_Memory_Read			0x04
	#define		ISO15693_KillTag				0x06		// 2007-01-06 2:01PM TI HF-I Pro Only
    #define     ISO15693_Block_Lock             0x07

#define		SystemConfig_Mode				0x10
	#define		BuzzerOn						0x01
	#define		RF_On							0x02
	#define		Full_PowerMode					0x03
	#define		System_Reset					0x04
	#define		AGC_On							0x05
	#define		THUBAN_Bootloader				0x10	

extern unsigned char RunMode;
#endif



/*
SOF					0x01
Packet Length		0xXX
Device ID			0x02		// Thuban TRF796x Module
CMD1				0x01		// ISO Command
CMD2				0xXX
Data1
Data2
Data3
Data4
Data5
BCC
~BCC
*/

/*

// ISO15693 Test Command
UID read 1slot mode
01 0C 02 03 01 00 00 00 00 00 0D F2

UID read 16slot mode
01 0C 02 03 02 00 00 00 00 00 0E F1 

Block write mode
01 0C 02 03 03 00 12 34 56 78 07 F8

Block read mode
01 0C 02 03 04 00 00 00 00 00 08 F7

Blcok Lock mode
01 0C 02 03 07 00 00 00 00 00 0B F4

// ISO14443A Test Command
UID read mode
01 0C 02 01 01 00 00 00 00 00 0F F0

// ISO14443B Test Command
ATQB read mode
01 0C 02 02 01 00 00 00 00 00 0C F3


*/
