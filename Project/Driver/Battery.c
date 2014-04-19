//满格  3.7

//最后一档  3.2

#include "include.h"

unsigned int  GetBatteyValue()
{
  unsigned int adcValue;

  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V  + REFON + ADC10ON;
  ADC10CTL1 = INCH_7;
  ADC10AE0 |= BIT7;                         // P7.4 ADC option select

  ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
  while (!(ADC10IFG & ADC10CTL0));       // ADC10 Conversion Complete?
  adcValue = ADC10MEM;
  return adcValue;
}

unsigned char  GetBatteyLevel()
{
  static unsigned int adcValue[16] = {0};  
  unsigned int tmpValue = 0;
  unsigned char i;
  unsigned int GetAdcValue = GetBatteyValue();
  if(adcValue[0] == 0)         //刚开始采样
  {
    for(i = 0;i<16;i++)
    {
        adcValue[i] = GetAdcValue;
    }
  }
  else
  {
    for(i = 15; i>0;i--)
    {
      adcValue[i] = adcValue[i-1];
    }
  }
  adcValue[0] = GetAdcValue;
  for(i = 0;i<16;i++)
  {
      tmpValue += adcValue[i];
  }
  tmpValue >>= 4;
  if(tmpValue > BATTERYLEVEL3)
  {
    return 3;
  }
  else if(tmpValue > BATTERYLEVEL2)
  {
    return 2;
  }
  else if(tmpValue > BATTERYLEVEL1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
    
  
}