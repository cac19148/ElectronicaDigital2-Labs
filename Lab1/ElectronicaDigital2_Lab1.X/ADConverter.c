#include <xc.h>
#include "ADConverter.h"

unsigned char ADCVal (unsigned char canal)
{
    ADCON0bits.CHS = canal; //se cambia a canal de displays
    //variable = ADRESH;        //se actualizan los displays con valor de pot1
    
    return (ADRESH);
}