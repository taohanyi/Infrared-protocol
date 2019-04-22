#include "define.h"

void IrRcv(void);
bool CheckIr(void);//u16 ID;

#ifndef  IrVariable
#define  IrVariable

#define IrCom  PORTCbits.RC2
#define LightCode 0xF5
#define CustomerCode 0xAE48

struct {
u8      High;
u8	Low;
}Pcount;

volatile union {
    u32 Long;
    struct {
        u8 Buf1;
        u8 Buf2;
        u8 Buf3;
        u8 Buf4;
           };
} IrData;

u8 RecvN;
#endif