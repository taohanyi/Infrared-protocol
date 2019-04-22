#include "IrReceive.h"
//#ifndef Function
//#define Function
//u8 IrDataBuf=0;
bit RcvFlg=0,PlowFlg=0,LeadFlg=0;

bool CheckIr(void)//u16 ID
{
    if(RcvFlg==1)
    {
        if(IrData.Buf3==LightCode && (IrData.Buf1+IrData.Buf2)==0xff)
        {
//            IrDataBuf=IrData.Buf2;
            RcvFlg=0;
            return(1);
        }
        RcvFlg=0;
    }
    return(0);
}

void IrRcv(void)
{
    if(RcvFlg==1) return;
    if(IrCom==1)     //High
      {
          Pcount.High+=1;
          PlowFlg=1;
      }
    else
      {
          Pcount.Low+=1;    //Low
          if(PlowFlg)
             {
                 if(LeadFlg==0)  //Header
                    {
                         if((Pcount.Low>=28)&&(Pcount.Low<=40))    //3.5ms±0.3ms
                             {
                                 if((Pcount.High>=14)&&(Pcount.High<=20))     //1.7ms±0.3ms
                                    {
                                       LeadFlg=1;
				       IrData.Long=0;
                                       RecvN=0;        //48 bit
                                    }
                                 else LeadFlg=0;
                             }
                          else LeadFlg=0;
                    }
                 else if(LeadFlg==1)             //Date
                    {
                          if((Pcount.Low>=3)&&(Pcount.Low<=6))            //430us±270us
                              {
                                 if((Pcount.High>=10)&&(Pcount.High<=15))    //1.3ms±300us,1
                                     {
					IrData.Long<<=1;
					IrData.Long|=0x00000001;
                                        RecvN+=1;
                                     }
                                 else if((Pcount.High>=3)&&(Pcount.High<=6))   //430us±270us,0
                                     {
                                         IrData.Long<<=1;
                                         IrData.Long&=0xfffffffe;
                                         RecvN+=1;
                                     }
                                 else LeadFlg=0;
                                 if(RecvN==16 && IrData.Long!=CustomerCode) LeadFlg=0;
                                 else if(RecvN==48)
                                 {RcvFlg=1;LeadFlg=0;}
                              }
                          else LeadFlg=0;
                    }
                 PlowFlg=0;
                 Pcount.High=0;
                 Pcount.Low=0;
             }
       }
}
