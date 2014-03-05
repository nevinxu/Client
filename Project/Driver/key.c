#include "key.h"

void InitKey()
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
        char len=64;                              //接收最大数据长度
        ReceiveData(len);
    }
    if(KeyIFG & KeyvolumeUp)
    {
char len=64;
    }
    if(KeyIFG & KeyvolumeDown)
    {
char len=64;
    }
    if(KeyIFG & KeyMaxRate)
    {
char len=64;
    }
    if(KeyIFG & KeyMinRate)
    {
char len=64;
    }
    
    P1IFG = 0;  
      
}