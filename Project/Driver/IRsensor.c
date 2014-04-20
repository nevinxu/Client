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
extern unsigned char Battery_Display_Flag;
extern unsigned char Display_Blink_Flag;   //报警值闪烁标志
extern unsigned Key_Set_Mode;  //键盘设置模式

extern unsigned int TotalDrop;  //总滴液

unsigned int Alarm_timer_Num = 0;  //报警间断时间计数
unsigned char Display_Blink_TimeOut;  //报警值设置超时

unsigned char Display_All_TimeOut;  //显示超时


extern unsigned char DataRecFlag;
unsigned char Rf_Interval_Num = 0;  //RF打开间断时间
unsigned char Rf_Interval_Flag = 0;  //RF打开间断标志

extern unsigned char AlarmUpperValue;  //报警上限值
extern unsigned char AlarmLowerValue; //报警下限值


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

void IRRecCaptureInit()
{
  IR_RE_SEL |= IR_RE;
  TA1CTL = TACLR;
  TA1CTL = TASSEL_1 + MC_2 + TAIE;  //ACLK  Continuous mode
  TA1CCTL0 = CM_1+CCIS_1+SCS +CAP+CCIE;    // 
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

  
unsigned int GetRate()   //多次采样取平均
{
  static unsigned int buffer[8];
  unsigned int retdata = 0;
  unsigned char i;
  double data = 0;
  if(RateDisplayFlag)  //必须要有中断  可能是溢出
  {
    data = 32768*time_num;
    data += TimerValue;
  }
  if(data)  
  {
      data = 60*32768 / data;
  }
  else
  {
    data =0;
  }
  time_num = 0;
  TimerValue = 0;
  if((buffer[7] == 0) &&(buffer[0] == 0))  //第一次
  {
    for(i=0;i<8;i++)
    {
      buffer[i] = (unsigned int)data;
    }
    if(data >= 40)
    {
      if(RateDisplayFlag)  //必须要有中断
      {
        TotalDrop = 0;//重新计数
        Play_Drop_Start();
      }
    }
  }
  else  //有数据了
  {
    for(i=7;i>0;i--)
    {
      buffer[i] = buffer[i-1];
    }
    buffer[0] = (unsigned int)data;
  }
  
  for(unsigned char i =0;i<8;i++)
  {
    retdata+=buffer[i];
  }
  retdata>>=3;
  if(RateDisplayFlag)  //必须要有中断
  {
    if(Alarm_timer_Num >= 20)  //10s一次
    {
      Alarm_timer_Num = 0;
      if(retdata >= AlarmUpperValue)
      {
        Play_OverQuick();
      }
      else if(retdata <= AlarmLowerValue)
      {
        Play_TooSlow();
      }
    }
  }
  if((buffer[7] >= 30) &&(retdata <=30))  //速度变慢  直到停止
  {
    Play_Drop_Stop();
  }


  return retdata;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
  static unsigned int OverflowTime=0,StoreTimes=1;
  static unsigned int IRTimerValue[10] = {0,0,0,0};
  static unsigned int OverflowValue[10] = {0,0,0,0};
  static double Valuefabs;
  TA1CCTL0 &=~ COV;
  Display_All_TimeOut = 0;
  IRTimerValue[StoreTimes] = TA1CCR0; 
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
        TotalDrop++;
step2:  StoreTimes++;

  IRRecCaptureInit();
  LPM3_EXIT;
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
          goto step4;
        }
        if((StoreTimes == 1) && (IRTimerValue[0] == 0 )&& (OverflowValue[0] == 0))  //第一次采集  或者  上次数据为0
        {
          goto step3;
        }
        else if((OverflowValue[StoreTimes]==0)  && (IRTimerValue[StoreTimes]<=3280) )    //   100ms 328 = 10ms  这次的数据与上次的间隔时间太短,消抖  
        {
          StoreTimes--;   //去掉这个值
          goto step4;
        }
        Valuefabs = fabs((double)IRTimerValue[StoreTimes] - (double)IRTimerValue[StoreTimes-1]);
        if(Valuefabs> 20000 )  //两次获取到的速度差距太大 不刷新数据
        {
          goto step4;
        }
step3:  TimerValue = IRTimerValue[StoreTimes];
        time_num = OverflowValue[StoreTimes];
 //       DisplayMode = DisplayRateMode;
        DisplayModeChargetimes=0;
        RateDisplayFlag =1;
step4:  StoreTimes++;
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
  //   LEDOn(LED1);
#ifdef TestMode  
    TestLEDOUT |= TestLed;
#endif
  }
  else
  {
 //   LEDOff(LED1);
#ifdef TestMode  
    TestLEDOUT &=~ TestLed;
#endif
  }
  if(!RateDisplayFlag)
  {
      RefreshTime++;
      if((RefreshTime%200)==0)  //两秒钟更新一下
      {
        if(GetRate()>=10)//速度大于10 更新  
        {
          RefreshTime = 1;
          RateDisplayFlag = 1;
        }
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
  static unsigned char i;
    switch( TA0IV )
  {
    case  2:  
      {
        TA0CCR1 += 0x4000;      //500ms
        i++;
        if(i%10)  //5s    电池显示更新
        {
          Battery_Display_Flag = 1;
        }
        Alarm_timer_Num++;
        if(Alarm_timer_Num >= 60000)   //计数不自动清零
        {
          Alarm_timer_Num = 60000;
        }
        if(Key_Set_Mode != CLOSE)  //设置模式下  显示需要闪烁
        {
          if(i%2)
          {
            Display_Blink_Flag = 1;  //设置模式下显示数字
          }
          else
          {
            Display_Blink_Flag = 2;     //设置模式下不显示数字
          }
          Display_Blink_TimeOut++;//报警值状态超时
        }
        else
        {
          Display_Blink_Flag = 0;
        }
        Display_All_TimeOut++;
        DisplayModeChargetimes++;
        if(DisplayModeChargetimes%2)
        {
          LEDOff(BLUELED);
        }
        else
        {
          DisplayDigtalClockFlag = 1;
        }
        if(DisplayModeChargetimes>=20)   //   10s
        {
          DisplayModeChargetimes=0;
//          DisplayMode = DisplayClockMode;
        }
        if(DataRecFlag == 0)
        {
          Rf_Interval_Num++;
          if(Rf_Interval_Num >= 16)  //8秒
          {
            Rf_Interval_Num = 0;
            Rf_Interval_Flag = 1;
          }
        }
        else
        {
          Rf_Interval_Num = 0;
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
