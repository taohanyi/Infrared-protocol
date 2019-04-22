/*********************************************************************
* FileName:	   main.c
* Dependencies:    See INCLUDES section below
* Processor:       PIC12F1501
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

#include <pic.h>
#include "define.h"
#include "IrReceive.h"
#include "Pwm.h"
#include "flash_routines.h"

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT controlled by the SWDTEN bit in the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = ON          // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = ON    // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = BOOT       // Flash Memory Self-Write Protection (000h to 1FFh write protected, 200h to 1FFFh may be modified by EECON control)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = ON       // Low-Power Brown Out Reset (Low-Power BOR is enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

u8  DimmingNum=0;
u16 TimingNum=0;
u16 TimingCnt=0;
u8  IrAn=0;

void main()
{
 	Initialize();
        ReadDataFromFlash();
        NightL=0;
        WritePwmPercentage(DimCom,DimmingNum*5);
	while (1)
	{
//            CLRWDT();
            TimeBase();
            if(CheckIr())
            {
                IrDeal();
            }
	}
}

u8 GetIrAN(void)
{
    static u8 cnt1=0,cnt2=0;
    u8 AN=0;
    if(K1==1)
    {
       cnt2=0;
       if(cnt1++>10) AN=Ir_CH1;
    }
    else
    {
       cnt1=0;
       if(cnt2++>10) AN=Ir_CH2;
    }
    return AN;
}

u8 DimmingNumBackup=0;
void SaveDataToFlash(void)
{
    u8 AddrTemp;
    AddrTemp=flash_memory_read(SaveDataRoom);
    if(DimmingNumBackup!=DimmingNum)
    {
        if(AddrTemp>=15)
        {       
            AddrTemp=1;
        }
        else
        {
            AddrTemp+=1;
        }
        flash_memory_erase(7);
        flash_memory_write(SaveDataRoom, &AddrTemp,1);
        DimmingNumBackup=DimmingNum;
        AddrTemp=AddrTemp+SaveDataRoom;
        flash_memory_write(AddrTemp, &DimmingNum,1);
    }
}

void ReadDataFromFlash(void)
{
    u8 AddrTemp;
    AddrTemp=flash_memory_read(SaveDataRoom);
    AddrTemp=AddrTemp+SaveDataRoom;
    DimmingNumBackup=DimmingNum=flash_memory_read(AddrTemp);
}

void IrDeal(void)
{
//    u8 Irdata=IrData.Buf2;
    static bit Night=0;

    if(IrAn!=IrData.Buf4)  //channel
        return;

    switch(IrData.Buf2)
    {
        case Ir_Timer30:    //auto OFF timer
        {
            if(TimingNum==0){TimingNum=1800; BzSet(1,BzShort);}
            else
            {
                TimingNum=0;
                TimingCnt=0;
                BzSet(2,BzShort);
            }
            break;
        }
        case Ir_Timer60:    //auto OFF timer
        {
            if(TimingNum==0){TimingNum=3600; BzSet(1,BzShort);}
            else
            {
                TimingNum=0;
                TimingCnt=0;
                BzSet(2,BzShort);
            }
            break;
        }
        case Ir_3300:
        {
//            TimingNum=0;/
            DimmingNum=56;
            Night=0;
            SaveDataToFlash();
            BzSet(1,BzShort);
            break;
        }
        case Ir_2700:
        {
//            TimingNum=0;
            DimmingNum=43;
            Night=0;
            SaveDataToFlash();
            BzSet(1,BzShort);
            break;
        }
        case Ir_2200:
        {
//            TimingNum=0;
            DimmingNum=35;
            Night=0;
            SaveDataToFlash();
            BzSet(1,BzShort);
            break;
        }
        case Ir_50:
        {
//            TimingNum=0;
            DimmingNum=28;
            Night=0;
            SaveDataToFlash();
            BzSet(1,BzShort);
            break;
        }
        case Ir_Night:
        {
//            TimingNum=0;
            if(Night!=ON)
            {
                DimmingNum=0;
                Night=1;
                BzSet(1,BzShort);
            }
            else
            {
                DimmingNum=0;
                Night=0;
                BzSet(1,BzShort);
            }
            break;
        }
        case Ir_Power:
        {
//            TimingNum=0;
            if(DimmingNum==OFF)
            {
//                TimingNum=0;
                Night=0;
                ReadDataFromFlash();
                BzSet(1,BzShort);
            }
            else
            {
                DimmingNum=0;
                Night=0;
                BzSet(1,BzShort);
            }
            break;
        }
        default: return;
    }
    NightL=Night;
    WritePwmPercentage(DimCom,DimmingNum*5);
}

bit Ms10flag=0;
u8 S1Cnt=0;
void TimeBase(void)
{
    if(Ms10flag==1)
    {
        Ms10flag=0;    
        S1Cnt+=1;
        IrAn=GetIrAN();
        if(S1Cnt>=100)
        {
            S1Cnt=0;
            if(TimingNum!=0)//Timer Off
            {
                TimingCnt+=1;
                if(TimingCnt>=TimingNum)
                {
                    TimingNum=0;
                    TimingCnt=0;
                    DimmingNum=0;
                    NightL=0;
                    WritePwmPercentage(DimCom,0);
                }
            }
        }
    }
}

void Initialize()
{
	OSCCON = 0x7a;          //16MHz internal oscillator
        WDTCON = 0x0e;          //watchdog timer 128ms 0x10
	ANSELA = 0;             //Set as digital port

//Port direction setting
	TRISA = 0b00010100;  //RA2 & RA4 : input
	WPUA  = 0b00000100;  //RA2 Pull-Up
	PORTA = 0b00100000;  //100000

//Timer0
        OPTION_REGbits.TMR0CS = 0; //Fosc/4
        OPTION_REGbits.PS = 0;     //1:2
        OPTION_REGbits.PSA = 0;

        TMR0 = TMR0SETVALUE;
        INTCONbits.TMR0IE = 1;
        INTCONbits.GIE = 1;	//enable global interrupt

//PWM setting
        PR2 = PWMDuty - 1;
        T2CON = 0b00000111;  //1:64

//        PWM4DCL = 0x00;
//        PWM4DCH = 0x00;
        PWM4CON = 0xd0;//11010000
}

void BzSet(u8 Times,u16 Long)
{
    Bz.LongCnt=0;
    Bz.Long=Long;
    Bz.TimesCnt=0;
    Bz.Times=Times;
    Bz.BzSwitchFlg=ON;
}

//***************************************
//	Interrupt ServiceRoutine
//***************************************
u8 Ms10Cnt=0;
void interrupt ISR(void)
{
    if(INTCONbits.TMR0IF == 1)   //100us
    {
        //Handle the Timer0 interrupt
        TMR0=TMR0SETVALUE;   //
        if(Bz.BzSwitchFlg==ON)
        {
	    if(Bz.LongCnt++<=Bz.Long)
		BzCom=1;
	    else
            {
		BzCom=0;
            }
            if(Bz.LongCnt>=(Bz.Long+BzInterval))
            {
               Bz.LongCnt=0;
               Bz.TimesCnt+=1;
               if(Bz.TimesCnt>=Bz.Times)
               {
                  Bz.BzSwitchFlg=OFF;
                  BzCom=0;
               }
            }
        }
        IrRcv();
        Ms10Cnt+=1;
        if(Ms10Cnt>=100)
        {
           Ms10Cnt=0;
           Ms10flag=1;
        }
        INTCONbits.TMR0IF = 0;
    }
}
