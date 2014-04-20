#include <msp430.h>
#include "include.h"

unsigned int    TimerValue;
unsigned int    RateDisplayFlag;
unsigned int    time_num = 999;   //开机显示0
unsigned int    Rate =0 ;
unsigned int    RefreshTime;

unsigned char BatteyLevel;
unsigned char Battery_Display_Flag;
unsigned char   DisplayDigtalClockFlag;

unsigned char KeyPressFlag;   //按键标志

unsigned int DisplayMode = DisplayRateMode;
unsigned char DisplayModeChargetimes=0;
unsigned char DisplayModeChargeFlag=0;

unsigned char CurrentSpeed =70;
unsigned int TotalDrip =0;
unsigned char TerminalPowerPrecent =100;
extern unsigned char Display_All_TimeOut;  //显示超时

extern unsigned char DataRecFlag;
extern unsigned char Rf_Interval_Flag;  //RF打开间断标志

extern unsigned char Display_Blink_Flag;   //报警值闪烁标志

extern unsigned char Display_Blink_TimeOut;  //报警值设置超时
extern unsigned Key_Set_Mode;  //键盘设置模式

unsigned char WorkingStatus = 0 ;

void ClockInit()
{
  volatile unsigned int i;                  // Use volatile to prevent removal
#ifdef HiXT1
  FLL_CTL0 = XTS_FLL ;                     // XT1  高频模式 Configure load caps
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

void VddIOEnable()
{
  VDD_IO_EN_SET;
  VDD_IO_ENABLE;
}

void VddIODisable()
{
  VDD_IO_EN_SET;
  VDD_IO_DISABLE;
}


void DriverInit()
{
  ClockInit();
  
  VddIOEnable();  //外部模块供电使能
  
  ReadAlarmValue4Flash();
  
  LEDInit(); 
  LCDInit();
  KeyInit();
 // UartInit();
  RadioInit();  //cc1101 初始化
  IRSensorInit();
 // DigtalClockInit(); //数字时钟初始化
  
  TA0Init();   //定时器初始化
  
  LEDOn(YELLOWLED);   //显示标志
  Delay_ms(500);
  LEDOff(YELLOWLED);
  
  LoginTransmit();   //登陆命令 
  
  GetBatteyLevel();
  
  ISD2100Init();
  
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  DriverInit();
  __bis_SR_register(GIE);   //全局中断使能
  while(1)
  {
    if(DisplayMode == DisplayRateMode)
    {
      if(RateDisplayFlag)
      {
        Rate = GetRate();
        RateDisplayFlag =0;
        DisplayRate(Rate); 
      //  DisplayTotalDrop();
        RefreshTime=1;
      }
      if(DataRecFlag == 1)  //数据接收标志
      {
        LEDOn(BLUELED);
        ReceiveData();
        RecDataCheck();   //接收数据检查
        DataRecFlag = 0;
      }
      if(Rf_Interval_Flag == 1)  //RF打开间断标志
      {
        Rf_Interval_Flag = 0;
        ReceiveOn();
      }
      if(Battery_Display_Flag == 1)  //电池显示
      {
        Battery_Display_Flag = 0;
        BatteyLevel = GetBatteyLevel();
        DisplayBattery( BatteyLevel);
      }
      if(KeyPressFlag)   //按键处理
      {
        KeyFunction();
        KeyPressFlag = 0;
        
      }
      if(Display_Blink_Flag)
      {
        if(Display_Blink_TimeOut >= 20)  //10s超时
        {
          Display_Blink_TimeOut =0;
          Key_Set_Mode &=~ (UPPERSETMODE+LOWERSETMODE);
        }
        RefreshAlarmValue();
        Display_Blink_Flag = 0;  //判断以后再清除
      }
    }
    else if(DisplayMode == DisplaySleepMode)
    {
      __bis_SR_register(GIE+LPM2);
      DisplayMode = DisplayRateMode;
      LCDInit();
    }
    if(Display_All_TimeOut >= 120)  //一分钟
    {
      Display_All_TimeOut = 0;
      DisplayNone();
      DisplayMode = DisplaySleepMode;
    }
#if 0
 //   else if(DisplayMode == DisplayClockMode)
    {
      if(DisplayDigtalClockFlag)
      {
        DisplayDigtalClockFlag = 0;
        DigtalClockRun();
        DisplayDigtalClock();
    //    ReceiveOn();
        TotalDrip++;  //测试  液滴自动加一
//        WorkingStateMsgTransmit();

      }
    }
#endif
    
  }
}





