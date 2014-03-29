#include <msp430.h>
#include <math.h>
#include "IRsensor.h"

extern unsigned int TimerValue;
extern unsigned int RateDisplayFlag;
extern unsigned int time_num;
extern unsigned int Rate ;
extern unsigned int RefreshTime;
extern unsigned char   DisplayDigtalClockFlag;
extern unsigned int DisplayMode;
extern unsigned char DisplayModeChargetimes;
extern unsigned char DisplayModeChargeFlag;



void IRSensorPortInit(void)
{
  IR_CTL_DIR |= IR_CTL;
  IR_CTL_OUT |= IR_CTL;
  
  IR_RE_DIR &=~ IR_RE;
}

unsigned char IRReceiveStatus()
{
  unsigned char ret;  
  if(IR_RE_IN &  IR_RE)
  {
    ret = Open;
  }
  else
    ret = Close;
  
  return ret;
}

void LEDInit(void)
{
  LEDDIR |= LED1+LED2;
  LEDOUT &=~ (LED1+LED2);
  
#ifdef TestMode
  TestLEDIR |= TestLed;
  TestLEDOUT &=~ TestLed;
#endif
}

void  LEDOn(unsigned char num)
{
  if(num == LED1)
  {
    LEDOUT |= LED1;
  }
  else if(num == LED2)
  {
    LEDOUT |= LED2;
  }
}

void  LEDOff(unsigned char num)
{
  if(num == LED1)
  {
    LEDOUT &=~ LED1;
  }
  else if(num == LED2)
  {
    LEDOUT &=~ LED2;
  }
}

void IRRecCaptureInit()
{
  IR_RE_SEL |= IR_RE;
  TA1CTL = TACLR;
  TA1CTL = TASSEL_1 + MC_2 + TAIE;  //ACLK  Continuous mode
  TA1CCTL3 = CM_1 +SCS +CAP+CCIE;    // 
}
void TA0Init()
{
  TA0CTL = TACLR;
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCTL1 = CCIE;                             // CCR1 interrupt enabled
  TA0CCR0 = 328;                                 //定时时间为10ms   32768/100
  TA0CCR1 = 0x8000;
  TA0CTL = TASSEL_1 + MC_2+ TAIE;     //Continuous mode
}

void  IRSensorInit()
{
  IRSensorPortInit();  //红外初始化
  IRRecCaptureInit();
}


unsigned int GetRate()
{
  static unsigned int buffer[8];
 // static unsigned char 
  /*
  for(unsigned char i =7;i>0;i--)
  {
    buffer[i] = buffer[i-1];
  }
  */
  double data;
  data = 32768*time_num;
  data += TimerValue;
  data = 60*32768 / data;
  buffer[0] = (unsigned int)data;
  /*
  for(unsigned char i =1;i<8;i++)
  {
    buffer[0]+=buffer[i];
  }
  */
//  buffer[0];
  return buffer[0];
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
  static unsigned int OverflowTime=0,StoreTimes=1;
  static unsigned int IRTimerValue[10] = {0,0,0,0};
  static unsigned int OverflowValue[10] = {0,0,0,0};
  static double Valuefabs;
  TA1CCTL3 &=~ COV;
  switch( TA1IV )
  {
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case  6: 
      {
        IRTimerValue[StoreTimes] = TA1CCR3; 
        OverflowValue[StoreTimes] = OverflowTime;
        OverflowTime =0;
        if(StoreTimes>=9)
        {
          IRTimerValue[0] = IRTimerValue[StoreTimes-1];
          IRTimerValue[1] = IRTimerValue[StoreTimes];
          StoreTimes=1;     
        }
        if(OverflowValue[StoreTimes]>3)   //间断时间太长  6s
        {
          StoreTimes=0;
          IRTimerValue[0] =0;
          OverflowValue[0] =0;
          goto step2;
        }
        if((StoreTimes == 1) && (IRTimerValue[0] == 0 )&& (OverflowValue[0] == 0))  //第一次采集  或者  上次数据为0
        {
          goto step1;
        }
        else if((OverflowValue[StoreTimes]==0)  && (IRTimerValue[StoreTimes]<=3280) )    //   100ms 328 = 10ms  这次的数据与上次的间隔时间太短,消抖  
        {
          StoreTimes--;   //去掉这个值
          goto step2;
        }
        Valuefabs = fabs((double)IRTimerValue[StoreTimes] - (double)IRTimerValue[StoreTimes-1]);
        if(Valuefabs> 20000 )  //两次获取到的速度差距太大 不刷新数据
        {
          goto step2;
        }
step1:  TimerValue = IRTimerValue[StoreTimes];
        time_num = OverflowValue[StoreTimes];
        DisplayMode = DisplayRateMode;
        DisplayModeChargetimes=0;
        RateDisplayFlag =1;
step2:  StoreTimes++;
        break; 
      }
        // CCR2 not used
    case  8:  break;                        // CCR2 not used
    case 10:  
      {
        OverflowTime++;                // overflow
        if(!RateDisplayFlag)
        {
         time_num++;   //没有中断 必须将每次的溢出值累加
        }
        
      break;
      }
  }
  IRRecCaptureInit();
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_0 (void)
{
  TA0CCR0 += 328;  
  if(IRReceiveStatus() == 1)
  {
     LEDOn(LED1);
#ifdef TestMode  
    TestLEDOUT |= TestLed;
#endif
  }
  else
  {
    LEDOff(LED1);
#ifdef TestMode  
    TestLEDOUT &=~ TestLed;
#endif
  }
  if(!RateDisplayFlag)
  {
      RefreshTime++;
      if((RefreshTime%200)==0)  //两秒钟更新一下
      {
        RefreshTime = 1;
        RateDisplayFlag = 1;
      }
   } 
  else
  {
      RefreshTime = 1;
  }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A0_1 (void)
{
    switch( TA0IV )
  {
    case  2:  
      {
        TA0CCR1 += 0x8000;   
        DisplayDigtalClockFlag = 1;
        DisplayModeChargetimes++;
        if(DisplayModeChargetimes==5)
        {
          LEDOff(LED1);
        }
        else if(DisplayModeChargetimes>=10)
        {
          DisplayModeChargetimes=0;
          DisplayMode = DisplayClockMode;
        }
        break;                        // CCR1 not used
      }
    case  4:  break;                        // CCR2 not used
    case  6:  break;   
    case  8:  break;                       // CCR2 not used
    case 10:  
  {
      break;   
  }
  }
}
