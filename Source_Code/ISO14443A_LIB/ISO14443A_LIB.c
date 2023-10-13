/********************************************************************************
*           Copyright (C) 2006 THUBAN Co.,LTD.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* FILENAME...... ISO14443A_LIB.c
* DATE CREATED.. 12/23/2006 3:37PM
* AUTHOR........ Jake Hwang
*------------------------------------------------------------------------------
* HISTORY:


********************************************************************************/
#include "ISO14443A_LIB.h"
#include "MCU_Port_Define.h"
#include "TRF796x_LIB.h"
#include "System_Control.h"
#include "COMM_Protocol.h"

unsigned char ISO14443A_Handle(void)
{ 
	unsigned char AntiCollision_LoopCnt;
	unsigned char i;
    char Result;
			
	unsigned char ISO14443A_TagCnt;
	
    ISO14443A_TagCnt = 0x00;
	
    // ISO14443A Collision bit clear
    ISO14443A_AntiCol_Config.Collision = 0x00;
    
    i = 0x0A;
 	   
    do
    {
	    AntiCollision_LoopCnt = 32;
	    
	    if(ISO14443A_TagCnt == 0x05)
	    	break;
	    		    
	    if(NonError == ISO14443A_REQA_or_WAKEUP(REQA, &ISO14443A_ATQA[0]))
	    {
	        do
	        {
	        	if(NonError == ISO14443A_AntiCol(&ISO14443A_UID[ISO14443A_TagCnt][0]))
	        	{
	        		if(!ISO14443A_AntiCol_Config.Collision)
	        		{
	        			if(NonError == ISO14443A_SEL(&ISO14443A_UID[ISO14443A_TagCnt][0]))
	        			{
	        				if(NonError == ISO14443A_HALT())
	        				    ISO14443A_TagCnt++;
	        			}
                        else break;
	        		}    		
	        	}
	        	
	        }while(--AntiCollision_LoopCnt && ISO14443A_AntiCol_Config.Collision);
	        
	        // RF Hardware not matched...
	        if(!AntiCollision_LoopCnt) 
	        	break;
	    }
	    else
	    	break;
    }while(--i);
       
    if(!ISO14443A_TagCnt)
    {
        if(HostMode == RunMode)
        {
            Result = TAGNoResponse;
            SendToHost(&Result,0x01);
        }
        return 0x01;
    }
    else
    {
        if(HostMode == RunMode)
        {
            for(i = 0x00; i < ISO14443A_TagCnt; i++)
                SendToHost(&ISO14443A_UID[i][0],ISO14443A_UID_Size);
        }
        return 0x00;
    }
}
