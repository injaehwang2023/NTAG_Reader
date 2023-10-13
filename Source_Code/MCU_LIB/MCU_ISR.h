/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... MCU_ISR.h
* DATE CREATED.. 12/23/2006 12:52PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:

********************************************************************************/

#ifndef		__MCU_ISR_Define__
#define		__MCU_ISR_Define__
/*******************************************************************************/

#include "COMM_Protocol.h"
/*******************************************************************************/
extern unsigned char TagFound;
extern char UART_RxBuffer[PacketLength];
extern unsigned char UART_Rx_Byte;
extern unsigned char UART_Rx_Finish;
extern char *pUART_RxBuffer;
/*******************************************************************************/

#endif
