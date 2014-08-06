#include <msp430.h>
#include <math.h>
#include "IRsensor.h"

extern unsigned int TimerValue;
extern unsigned int RateDisplayFlag;
extern unsigned int time_num;
unsigned int OverflowTime;
extern unsigned int Rate ;
extern unsigned int RefreshTime;
extern unsigned char   DisplayDigtalClockFlag;
extern unsigned int DisplayMode;
extern unsigned char DisplayModeChargetimes;
extern unsigned char DisplayModeChargeFlag;
extern unsigned char Battery_Display_Flag;
extern unsigned char Display_Blink_Flag;   //报警值闪烁标志
extern unsigned Key_Set_Mode;  //键盘设置模式
extern unsigned char KeyPressFlag;   //按键标志
extern unsigned int keyPressLongNum;  //长按定时计数
extern unsigned char keyPressLongFlag;  //长按表示

extern unsigned int TotalDrop;  //总滴液

unsigned int Alarm_timer_Num = 0;  //报警间断时间计数
unsigned char Display_Blink_TimeOut;  //报警值设置超时

unsigned char Display_All_TimeOut;  //显示超时


extern unsigned char RFDataRecFlag;
unsigned char Rf_Interval_Num = 0;  //RF打开间断时间
unsigned char Rf_Interval_Flag = 0;  //RF打开间断标志

extern unsigned char AlarmUpperValue;  //报警上限值
extern unsigned char AlarmLowerValue; //报警下限值

unsigned char TestModeChargeRatetimes;   //测试模式下的速度更新标志
extern unsigned int VoiceDelayNum;
unsigned int OverflowTime_Time0;        //for CC1101 定时 by cai
unsigned char Calflag;                  //by cai 时钟校准标志

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

void IRRecCaptureDisable()
{
    TA1CTL = 0;
    TA1CCTL0 = 0;
}
void TA0Init()
{
  TA0CTL = TACLR;
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCTL1 = CCIE;                             // CCR1 interrupt enabled
  TA0CCR0 = 328;                                 //定时时间为10ms   32768/100
  TA0CCR1 = 0x8000;                               //定时器时间为1000ms
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
  static unsigned char flag;
  unsigned int retdata = 0;
  static signed char i;
  double data = 0;
  double Valuefabs;
  if(RateDisplayFlag == 1)                  //必须要有中断  可能是溢出
  {
    data = 32768*OverflowTime;
    data += TimerValue;
  }
  else
  {
    data = 0;
  }
  if(data)  
  {
      data = 60*32768 / data;
  }
  else
  {
    data =0;
  }
  OverflowTime = 0;
  TimerValue = 0;
  if((buffer[7] == 0) &&(buffer[0] == 0))  //第一次
  {
    for(i=0;i<8;i++)
    {
      buffer[i] = (unsigned int)data;
    }
//    if(data >= 40)         //没什么要大于40?
//    {
//      if(RateDisplayFlag)  //必须要有中断
//      {
//        TotalDrop = 0;//重新计数
//       // Play_Drop_Start();
//        Alarm_timer_Num = 20;  //初始化为报警时间计数够了
//      }
//    }
  }
  else  //有数据了
  {
    for(i=7;i>0;i--)
    {
      buffer[i] = buffer[i-1]; 
    }
    buffer[0] = (unsigned int)data;
  }
    Valuefabs = fabs((double)buffer[1] - (double)buffer[0]);
    if(Valuefabs> 20 )  //两次获取到的速度差距太大    直接替换新的
    {
      for(i=0;i<8;i++)
      {
        buffer[i] = (unsigned int)data;
      }
    }
  
  for(i=0;i<8;i++)
  {
    retdata+=buffer[i];
  }
  retdata>>=3;
  if(RateDisplayFlag)  //必须要有中断
  {
    if(Alarm_timer_Num >= 10)  //10s一次
    {
      if(retdata >= AlarmUpperValue)
      {
        flag++;
        if(flag>=5)
        {
            Play_OverQuick();
        }
        Alarm_timer_Num = 0;
      }
      else if(retdata <= AlarmLowerValue)
      {
        flag++;
        if(flag>=5)
        {
            Play_TooSlow();
        }
        
        Alarm_timer_Num = 0;
      }
      else
      {
        flag = 0;
      }
    }
  }
//  if((buffer[7] >= 15) &&(retdata <=15))  //速度变慢  直到停止
//  {
//   // Play_Drop_Stop();
//  }


  return retdata;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{ 
  
  
  static unsigned int IRTimerValue[8] = {0,0,0,0,0,0,0,0};
  static unsigned int OverflowValue[8] = {0,0,0,0,0,0,0,0};
  static signed char i =0;
  static double Valuefabs;
  TA1CCTL0 &=~ COV;
  Display_All_TimeOut = 0;   //进入采集中断, 清除显示超时计数
  if((IRTimerValue[0] == 0)&&(IRTimerValue[7] == 0))
  {
    for(i=7;i>=0;i--)
    {
      IRTimerValue[i] = TA1CCR0;
      OverflowValue[i] = OverflowTime;
    }
  }
  else
  {
    for(i=7;i>0;i--)
    {
      IRTimerValue[i] = IRTimerValue[i-1];
      OverflowValue[i] = OverflowValue[i-1];
    }
    IRTimerValue[0] = TA1CCR0;
    OverflowValue[0] = OverflowTime;
  }
  if(OverflowValue[0]>3)   //间断时间太长  6s
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = 0;
    OverflowValue[i] = 0;
    }
    OverflowTime = 0;
    goto step2;
  }
  if((OverflowValue[0]==0)  && (IRTimerValue[0]<=3280) )    //   100ms 328 = 10ms  这次的数据与上次的间隔时间太短,消抖  
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = 0;
    OverflowValue[i] = 0;
    }
    goto step2;
  }
  Valuefabs = fabs((double)IRTimerValue[1] - (double)IRTimerValue[0]);
  if(Valuefabs> 20000 )  //两次获取到的速度差距太大 不刷新数据
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = TA1CCR0;
    OverflowValue[i] = OverflowTime;
    }
    goto step2;
  }
        TimerValue = IRTimerValue[0];
        OverflowTime = OverflowValue[0];
        RateDisplayFlag =1;    //数据中断接收    显示标志
        TotalDrop++;   //总滴速加一
step2:  

  IRRecCaptureInit();
  LPM2_EXIT;
  
 
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
  TA1CCTL3 &=~ COV;
  switch( TA1IV )
  {
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case  6: 
    case  8:  break;                        // CCR2 not used
    case 10:  
      {
        if(!RateDisplayFlag)
        {
         OverflowTime++;   //没有中断 必须将每次的溢出值累加
        }
        break;
      }
  }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_0 (void)
{

  static unsigned int i;
  TA0CCR0 += 328;  
  i++;

/**********************************************************************************/

/*********************************接收RF开关定时器*****************************/         
        if(RFDataRecFlag == 0)
        {
          Rf_Interval_Num++;
          if(Rf_Interval_Num >= 50)     //显示信号延时时间
          {
          	Rf_Interval_Num = 0;
            DisplayRFRSSI(0);
            RFDataRecFlag = 2;
          }
 //         if(Rf_Interval_Num >= RFRECEIVEINTERVAL)  //8秒
 //         {
 //           Rf_Interval_Num = 0;
  //          Rf_Interval_Flag = 1;
  //          DisplayRFRSSI(0);
 //         }
        }
  //      else if(RFDataRecFlag == 1)
   //     {
  //        Rf_Interval_Num = 0;
  //      }


/************************长按键定时处理函数**************************************/ 
  if(keyPressLongFlag != NONE)
  {
    keyPressLongNum++;
    if(keyPressLongNum>=40)   //400毫秒延时
    {
      if(keyPressLongNum%10 == 0) // 100毫秒一次加
      {
        Display_Blink_Flag = 1;  //显示
        if(keyPressLongFlag == KeyvolumeUp)
        {
          KeyPressFlag = KeyvolumeUp;       
        }
        else if(keyPressLongFlag == KeyvolumeDown)
        {
          KeyPressFlag = KeyvolumeDown;
        }
      }
    }
  }
/*******************************************************************************/

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A0_1 (void)
{

  static unsigned int i;
    switch( TA0IV )
  {
    case  2:  
      {
        TA0CCR1 += 0x4000;      //500ms
        i++;
		
        if(i%1000)  //500s    电池显示更新
        {
          Battery_Display_Flag = 1;
        }
/************************测试模式下的速度更新**********************************/       
        if((DisplayMode & DisplayTestMode) == DisplayTestMode)
        {
          TestModeChargeRatetimes++;
          if(TestModeChargeRatetimes >= TESTMODERATEREFRESHTIME) //  //2s更新一次速度
          {
            TestModeChargeRatetimes = 0;
            RateDisplayFlag = 1;
          }
        }
/**********************************************************************************/
	  if(!RateDisplayFlag)
	  {
	      RefreshTime++;
	      if((RefreshTime%CLEARDATATIMEOUT)==0)  //8秒钟更新一下
	      {
	          RefreshTime = 1;
	          RateDisplayFlag = 2;  //特殊模式  速度为0
	      }
	   } 
	  else
	  {
	      RefreshTime = 1;
	  }
	  if(VoiceDelayNum>0)    //10ms一次中断
	  {
	    VoiceDelayNum--;
	  }

/*******************************液晶显示超时计数***************************************/        

	Display_All_TimeOut++;
	if(Display_All_TimeOut >=60000)
	{
		Display_All_TimeOut = 60000;
	}


/********************************报警时间间隔计数*******************************/
        Alarm_timer_Num++;  //   500ms一次
        if(Alarm_timer_Num >= 60000)   //计数不自动清零
        {
          Alarm_timer_Num = 60000;
        }
/*******************************************************************************/        
        
/*********************************按键闪烁定时器*********************************/
        if(Key_Set_Mode != CLOSE)  //设置模式下  显示需要闪烁
        {
          if(i%2)    //500ms  闪烁
          {
            Display_Blink_Flag = 1;  //设置模式下显示数字
          }
          else
          {
            Display_Blink_Flag = 2;     //设置模式下不显示数字
          }
          Display_Blink_TimeOut++;//报警值闪烁状态超时
        }
        else
        {
          Display_Blink_Flag = 0;   //不设置模式下  不闪烁状态
        }
/*******************************************************************************/        
        
        

/*******************************************************************************/ 
        
        
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
