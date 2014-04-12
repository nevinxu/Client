#include "key.h"

extern unsigned char DataRecFlag;

void KeyInit()
{
  KeyDIR &=~ (KeyvolumeUp+KeyvolumeDown+KeyMaxRate+KeyMinRate);
  KeyIES &=~ (KeyvolumeUp+KeyvolumeDown+KeyMaxRate+KeyMinRate);
  KeyIFG &=~ (KeyvolumeUp+KeyvolumeDown+KeyMaxRate+KeyMinRate);
  KeyIE |= (KeyvolumeUp+KeyvolumeDown+KeyMaxRate+KeyMinRate);
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_ISR (void)
{
    if(P1IFG & TI_CC_GDO2_PIN)
    {
      DataRecFlag = 1;   //数据接收标志
    }
    if(KeyIFG & KeyvolumeUp)
    {
      ;
    }
    if(KeyIFG & KeyvolumeDown)
    {
      ;
    }
    if(KeyIFG & KeyMaxRate)
    {
      ;
    }
    if(KeyIFG & KeyMinRate)
    {
      ;
    }
    
    P1IFG = 0;  
      
}