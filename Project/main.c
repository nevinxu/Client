#include <msp430.h>
#include "include.h"

unsigned int    TimerValue;
unsigned int    RateDisplayFlag;
unsigned int    time_num = 999;   //������ʾ0
unsigned int    Rate =0 ;
unsigned int    RefreshTime;
unsigned char   DisplayDigtalClockFlag;

unsigned int DisplayMode = DisplayClockMode;
unsigned char DisplayModeChargetimes=0;
unsigned char DisplayModeChargeFlag=0;

unsigned char CurrentSpeed =70;
unsigned int TotalDrip =0;
unsigned char TerminalPowerPrecent =100;

unsigned char WorkingStatus ;

void ClockInit()
{
  volatile unsigned int i;                  // Use volatile to prevent removal
#ifdef HiXT1
  FLL_CTL0 = XTS_FLL ;                     // XT1  ��Ƶģʽ Configure load caps
  FLL_CTL1 = SMCLKOFF  + SELM1 ;
  //  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (int i = 0x47FF; i > 0; i--);           // Time for flag to set
  }
//  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
#endif

#ifdef LoXT1
  SCFI0 |= FN_4;                            // x2 DCO freq, 8MHz nominal DCO
  SCFQCTL = 121;                            // (121+1) x 32768 x 2 = 7.99 Mhz
  FLL_CTL0 |= DCOPLUS + XCAP11PF;           // DCO+ set so freq= xtal x D x N+1
/*  
    FLL_CTL0 |= XCAP11PF;                     // Configure load caps
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
                                            // stabilize
  do
  {
    IFG1 &= ~OFIFG;                         // Clear osc fault flag
    for (i = 0; i < 1000; i++);             // Delay for osc to stabilize
  } while(IFG1 & OFIFG);                    // Check to see if osc flag is set
  */
#endif
}

int main(void)
{
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  ClockInit();
  LEDInit(); 
  LCDInit();
  KeyInit();
  UartInit();
  RadioInit();  //cc1101 ��ʼ��
  IRSensorInit();
  DigtalClockInit(); //����ʱ�ӳ�ʼ��
  
  TA0Init();   //��ʱ����ʼ��
  
  LEDOn(LED1);   //��ʾ��־
  Delay_ms(1000);
  LEDOff(LED1);
  
  


  /*
  InitISD2100();
  unsigned char j;
  for(unsigned char i =0; i<10;i++)
  {
    for(unsigned char j=12;j<25;j++)
    {
    VoicePlay(j);
//    Delay_ms(500);
    }
 
  }
*/
  __bis_SR_register(GIE);   //ȫ���ж�ʹ��
  while(1)
  {
    if(DisplayMode == DisplayRateMode)
    {
      if(RateDisplayFlag)
      {
        Rate = GetRate();
        RateDisplayFlag =0;
        DisplayRate(Rate); 
   //     TransmitRate(Rate);
        RefreshTime=1;
      }
    }
    else if(DisplayMode == DisplayClockMode)
    {
      if(DisplayDigtalClockFlag)
      {
        DisplayDigtalClockFlag = 0;
        DigtalClockRun();
        DisplayDigtalClock();
    //    ReceiveOn();
        TotalDrip++;  //����  Һ���Զ�����
        WorkingStateMsgTransmit();
        LEDOn(LED1);
      }
    }

  }
}





