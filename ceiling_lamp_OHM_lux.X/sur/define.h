#define SAVERESTORE
//#define YAMAZEN

#ifndef _TYP
#define _TYP
#define u32  unsigned long
#define s32   signed long
#define u16  unsigned int
#define s16   signed int
#define u8   unsigned char
#define s8    signed char
#define bool  u8
#endif

#ifndef _DEFINE_H
#define _DEFINE_H
#include <pic.h>
void Initialize();
void IrDeal(void);
void SaveDataToFlash(void);
void ReadDataFromFlash(void);
u8 GetIrAN(void);
//void InitPowerOn(void);
void BzSet(u8 Times,u16 Long);
void TimeBase(void);

#define K1     PORTAbits.RA4
#define NightL PORTAbits.RA1
#define BzCom  PORTAbits.RA0

#define DimCom  PWM4

#define OFF    0
#define ON     1

//#define BzLong	     10000	//1s
#define BzShort	     1000	//0.1s
#define BzInterval   700

#define SaveDataRoom  112

#define FullOff           0
#define FullOn            1
#define NightLight        2
#define DimColor          3

#define Ir_Power	0x80
#define Ir_Night	0x40
#define Ir_Timer30	0x30
#define Ir_Timer60	0xD0
#define Ir_2200	        0x50
#define Ir_2700   	0x90
#define Ir_3300 	0x10
#define Ir_50 	        0x20

#define Ir_CH1		0x80
#define Ir_CH2		0xF0

#define TMR0SETVALUE	64
#define PWMDuty  	69

struct {
   u16 LongCnt;
   u16 Long;
   u8  TimesCnt;
   u8  Times;
   u8  BzSwitchFlg             : 1;
}Bz;

//typedef union {
//    u32 Uint32;
//    struct{
//    u8 byte0;
//    u8 byte1;
//    u8 byte2;
//    u8 byte3;
//	  };
//    u16 Uint16;
//}Union32;
//
//typedef union {
//    u16 Uint16;
//    struct{
//    u8 byte0;
//    u8 byte1;
//	  };
//}Union16;

#endif
