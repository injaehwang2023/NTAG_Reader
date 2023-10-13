/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... THUBAN_Bootloader.h
* DATE CREATED.. 2006-12-31 12:26PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__THUBAN_Bootloader__
#define		__THUBAN_Bootloader__

#define		CheckSumError		0x55
#define		FlashEraseOkay		0xA5
#define		ProgramOkay			0xAA

extern void _THUBAN_Bootloader(unsigned char Mode);

#endif
