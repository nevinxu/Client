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
    char len=0;   
    if(P1IFG & TI_CC_GDO2_PIN)
    {
      DataRecFlag = 1;   //数据接收标志
    }
    if(KeyIFG & KeyvolumeUp)
    {
      len=64;
    }
    if(KeyIFG & KeyvolumeDown)
    {
      len=64;
    }
    if(KeyIFG & KeyMaxRate)
    {
      len=64;
    }
    if(KeyIFG & KeyMinRate)
    {
      len=64;
    }
    
    P1IFG = 0;  
      
}