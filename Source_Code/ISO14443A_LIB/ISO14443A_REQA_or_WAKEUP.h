/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... ISO14443A_REQA_or_WAKEUP.h
* DATE CREATED.. 12/22/2006 3:42PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		ISO14443A_REQA_or_WAKEUP_
#define		ISO14443A_REQA_or_WAKEUP_

#define		REQA		0x26
#define		WAKEUP		0x52

// *****************************************************************************/
extern unsigned char ISO14443A_REQA_or_WAKEUP(unsigned char Mode, char *pISO14443A_ATQA);
extern char ISO14443A_ATQA[2];
// *****************************************************************************/
#endif
