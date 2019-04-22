#include "Pwm.h"
//#include "define.h"

void WritePwmPercentage(u8 AN, u16 Percentage)
{
    if(Percentage>276)
       Percentage=276;

    if(AN == 1)
    {
        PWM1DCH  = Percentage>>2;
        PWM1DCL  = Percentage<<6;
    }
    else if(AN == 2)
    {
        PWM2DCH  = Percentage>>2;
        PWM2DCL  = Percentage<<6;
    }
    else if(AN == 3)
    {
        PWM3DCH  = Percentage>>2;
        PWM3DCL  = Percentage<<6;
    }
    else if(AN == 4)
    {
        PWM4DCH  = Percentage>>2;
        PWM4DCL  = Percentage<<6;
    }
}
