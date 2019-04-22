#include "define.h"
#include "pic12f1501.h"

//#define EEPROM

#ifdef EEPROM
void EE_Start(void);
void EE_Stop(void);
void EE_Ack(void);
void EE_NoAck(void);
u8 EE_TestAck(void);
void EE_WriteByte(u8 Data);
void EE_WriteData(u8 Addr,u8 WriteData);
u8 EE_ReadByte(void);
u8 EE_ReadData(u8 Addr);
void EE_Delay(u16 Time);
void WriteEE(u8 *wdat,u8 addr,u8 num);
void ReadEE(u8 *rdat,u8 addr,u8 num);


#define EECLK  PORTCbits.RC6
#define EEDAT  PORTCbits.RC7
#define EEDAT_Input   TRISC|=0x80;
#define EEDAT_Output  TRISC&=0x7f;

#define	WriteDeviceAddress 0xa0
#define	ReadDviceAddress 0xa1

#define WaitTime    5

#endif