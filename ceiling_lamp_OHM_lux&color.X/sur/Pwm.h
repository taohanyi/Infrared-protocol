#include "define.h"

#define PWM1   1
#define PWM2   2
#define PWM3   3
#define PWM4   4
//void WritePWM(unsigned char AN, unsigned int Duty);
void WritePwmPercentage(u8 AN, u16 Percentage);

//#define IrCom  PORTAbits.RA2