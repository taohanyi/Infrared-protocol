#include "EEPROM.h"

#ifdef EEPROM
void EE_Delay(u16 Time)
{
    if(Time==0) return;
    while(Time--)
    CLRWDT();
}

void EE_Start(void)
{
    EEDAT=1;
    EE_Delay(WaitTime);
    EECLK=1;
    EE_Delay(WaitTime);
    EEDAT=0;
    EE_Delay(WaitTime);
    EECLK=0;
    EE_Delay(WaitTime);
}

void EE_Stop(void)
{
    EECLK=0;
    EE_Delay(WaitTime);
    EEDAT=0;
    EE_Delay(WaitTime);
    EECLK=1;
    EE_Delay(WaitTime);
    EEDAT=1;
    EE_Delay(WaitTime);
}

void EE_Ack(void)
{
    EEDAT=0;
    EE_Delay(WaitTime);
    EECLK=1;
    EE_Delay(WaitTime);
    EECLK=0;
    EE_Delay(WaitTime);
    EEDAT=1;
    EE_Delay(WaitTime);
}

void EE_NoAck(void)
{
    EEDAT=1;
    EE_Delay(WaitTime);
    EECLK=1;
    EE_Delay(WaitTime);
    EECLK=0;
    EE_Delay(WaitTime);
}

u8 EE_TestAsk(void)
{
    u8 BitData;
    EECLK=1;
    EE_Delay(WaitTime);
    EEDAT_Input;
    EE_Delay(WaitTime);
    EEDAT=1;
    EE_Delay(WaitTime);

    BitData=EEDAT;

    EEDAT_Output;
    EE_Delay(WaitTime);
    EECLK=0;
    EE_Delay(WaitTime);
    return(BitData);
}

void EE_WriteByte(u8 WriteData)
{
    u8 Bit8;
    for(Bit8=0;Bit8<8;Bit8++)
    {
        if( (WriteData & 0x80) == 0x80 )
            EEDAT=1;
        else
            EEDAT=0;
        EE_Delay(WaitTime);
        EECLK=1;
        EE_Delay(WaitTime);
        EECLK=0;
        EE_Delay(WaitTime);
        WriteData<<=1;
    }
}

u8 EE_ReadByte(void)
{
    u8 Bit8,ReadData=0;
    EEDAT_Input;
    EE_Delay(WaitTime);
    for(Bit8=0;Bit8<8;Bit8++)
    {
        EECLK=1;
        EE_Delay(WaitTime);
	ReadData<<=1;
        if(EEDAT==1) ReadData|=0x01;
        EE_Delay(WaitTime);
        EECLK=0;
        EE_Delay(WaitTime);
    }
    EEDAT_Output;
    return(ReadData);
}
void WriteEE(u8 *wdat,u8 addr,u8 num)
{
    while(num--)
    {
       EE_WriteData(addr,*wdat);
       addr+=1;
       wdat+=1;
    }
    /*
    EE_Start();
    EE_WriteByte(WriteDeviceAddress);
    EE_TestAsk();
    EE_WriteByte(addr);
    EE_TestAsk();
    for(;num>1;num--)
    {
        EE_WriteByte(*wdat);
        EE_TestAsk();
        wdat++;
    }
    EE_WriteByte(*wdat);
    EE_TestAsk();
    EE_Stop();
    EE_Delay(1000);
    */
}

void ReadEE(u8 *rdat,u8 addr,u8 num)
{
    while(num--)
    {
        *rdat=EE_ReadData(addr);
        addr+=1;
        rdat+=1;
    }
    /*
    if(num==0) return;
    EE_Start();
    EE_WriteByte(WriteDeviceAddress);
    EE_TestAsk();
    EE_WriteByte(addr);
    EE_TestAsk();
    EE_Start();
    EE_WriteByte(ReadDviceAddress);
    EE_TestAsk();
    for(;num>1;num--)
    {
        *rdat=EE_ReadByte();
        EE_Ack();
        rdat++;
    }
    *rdat=EE_ReadByte();
    EE_NoAck();
    EE_Stop(); 
    EE_Delay(50);
     */
}

void EE_WriteData(u8 Addr,u8 Data)
{
    EE_Start();
    EE_WriteByte(WriteDeviceAddress);
    EE_TestAsk();
    EE_WriteByte(Addr);
    EE_TestAsk();
    EE_WriteByte(Data);
    EE_TestAsk();
    EE_Stop();
    Delay(1000);
}

u8 EE_ReadData(u8 Addr)
{
    u8 Data=0;
    EE_Start();
    EE_WriteByte(WriteDeviceAddress);
    EE_TestAsk();
    EE_WriteByte(Addr);
    EE_TestAsk();
    EE_Start();
    EE_WriteByte(ReadDviceAddress);
    EE_TestAsk();
    Data=EE_ReadByte();
    EE_NoAck();
    EE_Stop();
    Delay(50);
    return(Data);
}
#endif