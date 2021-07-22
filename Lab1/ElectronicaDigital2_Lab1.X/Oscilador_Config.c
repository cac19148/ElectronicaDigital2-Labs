#include <xc.h>
#include "Oscilador_Config.h"

void Osc_Config(unsigned char frec)
{
    switch(frec)
    {
        case 31:                        //31kHz
          OSCCONbits.IRCF0 = 0;  
          OSCCONbits.IRCF1 = 0;  
          OSCCONbits.IRCF2 = 0;
          break;
          
        case 125:                       //125kHz
          OSCCONbits.IRCF0 = 1;  
          OSCCONbits.IRCF1 = 0;  
          OSCCONbits.IRCF2 = 0;
          
        case 250:                       //250kHz
          OSCCONbits.IRCF0 = 0;  
          OSCCONbits.IRCF1 = 1;  
          OSCCONbits.IRCF2 = 0;
          
        case 50:                        //500kHz
          OSCCONbits.IRCF0 = 1;  
          OSCCONbits.IRCF1 = 1;  
          OSCCONbits.IRCF2 = 0;
          
        case 1:                         //1MHz
          OSCCONbits.IRCF0 = 0;  
          OSCCONbits.IRCF1 = 0;  
          OSCCONbits.IRCF2 = 1;
          
        case 2:                         //2MHz
          OSCCONbits.IRCF0 = 1;  
          OSCCONbits.IRCF1 = 0;  
          OSCCONbits.IRCF2 = 1;
          
        case 4:                         //4MHz
          OSCCONbits.IRCF0 = 0;  
          OSCCONbits.IRCF1 = 1;  
          OSCCONbits.IRCF2 = 1;
          
        case 8:                         //8MHz
          OSCCONbits.IRCF0 = 1;  
          OSCCONbits.IRCF1 = 1;  
          OSCCONbits.IRCF2 = 1;
    }
    OSCCONbits.SCS  = 1;
}