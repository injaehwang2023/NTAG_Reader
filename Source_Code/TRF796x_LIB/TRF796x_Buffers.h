/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... TRF796x_Buffers.h
* DATE CREATED.. 12/15/2006 11:17AM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__TRF796x_Buffers_
#define		__TRF796x_Buffers_

// *****************************************************************************/

// *****************************************************************************/
typedef struct __TRF796xRHB_Buffer__
{
	char *pTxBuffer;
	unsigned char ChipStatus;
}TRF796xRHB_Buffer__;

extern char TRF796xRHB_TxBuffer[15];

extern TRF796xRHB_Buffer__ TRF796xRHB_Buffer;

#endif
