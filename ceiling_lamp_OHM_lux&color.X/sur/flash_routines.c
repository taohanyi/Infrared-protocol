/*********************************************************************
* FileName:        flash_routines.c
* Dependencies:    See INCLUDES section below
* Processor:       
* Compiler:        
* Company:         Microchip Technology, Inc.
*
* Software License Agreement:
*
* The software supplied herewith by Microchip Technology Incorporated
* (the "Company") for its PICmicro?Microcontroller is intended and
* supplied to you, the Company's customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*********************************************************************
* File Description:
*
* Change History:
* Author               
********************************************************************/
#include <htc.h>
#include "flash_routines.h"

//void Delay(unsigned long Time)
//{
//    while(Time--);
//}
//****************************************************************
//  FLASH MEMORY READ
//  needs 16 bit address pointer in address
//  returns 14 bit value from selected address
//  for PIC16F150x
//****************************************************************
unsigned char flash_memory_read (unsigned char address)
{
//    const unsigned char *PROM;
//    PROM=address;
//    return(*PROM);
        unsigned int AddrBuf;
        INTCONbits.GIE = 0;     //Disable interrupt
        AddrBuf=FlashAddr;
        AddrBuf+=address;
	PMADRL=((AddrBuf)&0xff);
	PMADRH=((AddrBuf)>>8);
	PMCON1bits.CFGS = 0;                // access FLASH program, not config
	PMCON1bits.RD = 1;
	#asm
		NOP;
		NOP;
		NOP;
		NOP;
	#endasm
        INTCONbits.GIE = 1;     //Disable interrupt
	return ((PMDATH)<<8 | (PMDATL));
}

//****************************************************************
//  FLASH MEMORY WRITE
//  needs 16 bit address pointer in address
// 14bit?Word?????????8bits??????
//  ?????????????32?
//****************************************************************
void flash_memory_write (unsigned char address, unsigned char *data, unsigned char datalen )
{
        unsigned char wdi=0;
        unsigned int AddrBuf;
        if(datalen==0)
            return;
        INTCONbits.GIE = 0;     //Disable interrupt
        AddrBuf=FlashAddr;
        AddrBuf+=address;
        PMADRL=((AddrBuf)&0xff);	// load address
        PMADRH=((AddrBuf)>>8);		// load address
        PMCON1bits.CFGS = 0;
        PMCON1bits.FREE = 0;
        PMCON1bits.LWLO = 1;
        PMCON1bits.WREN = 1;

        while(1)//for (wdi=0;wdi<datalen;wdi++)
        {
                PMDATH = 0x00;
                PMDATL = data[wdi];                 //???????8bit

//Unlock sequence
                PMCON2 = 0x55;				// required sequence
                PMCON2 = 0xAA;				// required sequence
                PMCON1bits.WR = 1;			// set WR to begin erase cycle
                #asm
                        NOP;
                        NOP;
		        NOP;
		        NOP;
                #endasm
                datalen-=1;
                if(datalen==0)
                   break;
                PMADRL++;            //increase the address
                wdi++;
        }

        PMCON1bits.LWLO = 0;					// this time start write

    //Unlock sequence
        PMCON2 = 0x55;				// required sequence
        PMCON2 = 0xAA;				// required sequence
        PMCON1bits.WR = 1;			// set WR to begin erase cycle
        #asm
                NOP;
                NOP;
                NOP;
		NOP;
        #endasm

        PMCON1bits.WREN = 0;		// disallow program/erase
	INTCONbits.GIE = 1;         //re-enable interrupt
}

//****************************************************************
//  FLASH MEMORY ERASE
//  Program memory can only be erased by rows.
//  A row consists of 32 words where the PMADRL<4:0> = 0000.
//
//****************************************************************
void flash_memory_erase (unsigned char Page)
{
        //address=(address*32)+0x0380;
        unsigned int AddrBuf;
        INTCONbits.GIE = 0;         //Disable interrupt
        AddrBuf=FlashAddr;
        AddrBuf+=(Page*16);
        PMADRL=((AddrBuf)&0xff);	// load address
        PMADRH=((AddrBuf)>>8);		// load address
        PMCON1bits.CFGS = 0;		// access FLASH program, not config
        PMCON1bits.WREN = 1;		// allow program/erase
        PMCON1bits.FREE = 1;		// perform an erase on next WR command, cleared by hardware
    //Unlock sequence
        PMCON2 = 0x55;				// required sequence
        PMCON2 = 0xAA;				// required sequence
        PMCON1bits.WR = 1;			// set WR to begin erase cycle
        #asm
                NOP;
                NOP;
                NOP;
		NOP;
        #endasm
        PMCON1bits.WREN = 0;		// disallow program/erase
        INTCONbits.GIE = 1;         //re-enable interrupt
}
