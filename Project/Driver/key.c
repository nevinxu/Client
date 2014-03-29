#include "key.h"

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
      len=0;                      //接收最大数据长度
      ReceiveData(len);
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