/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... ISO14443A_AntiCol.h
* DATE CREATED.. 12/23/2006 6:05PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__ISO14443A_AntiCol_
#define		__ISO14443A_AntiCol_

// *****************************************************************************/
#define		ISO14443A_UID_Buffer_Cnt		0x05
#define		ISO14443A_UID_Size				0x05

typedef struct __ISO14443A_AntiCol_Config__
{
	unsigned char SendByte;
	unsigned char Collision;
	unsigned char NVB;
	char *pTempBuffer;
	
}ISO14443A_AntiCol_Config__;

extern ISO14443A_AntiCol_Config__ ISO14443A_AntiCol_Config;

extern unsigned char ISO14443A_AntiCol(char *pISO14443A_UID);

extern char ISO14443A_UID[ISO14443A_UID_Buffer_Cnt][ISO14443A_UID_Size];
// *****************************************************************************/
#endif
