#include <msp430.h>
#include <stdlib.h>  //随机数 产生   头文件
#include <stdio.h>
#include "include.h"

unsigned int    TimerValue;
unsigned int    RateDisplayFlag;
unsigned int    time_num = 999;   //开机显示0
unsigned int    RefreshTime;

unsigned char BatteyLevel;
unsigned char Battery_Display_Flag;
unsigned char   DisplayDigtalClockFlag;

unsigned char KeyPressFlag;   //按键标志

unsigned int DisplayMode = 0;
unsigned char DisplayModeChargetimes=0;
unsigned char DisplayModeChargeFlag=0;

unsigned char CurrentSpeed =0;
extern unsigned int TotalDrop;
unsigned char TerminalPowerPrecent =100;
extern unsigned char Display_All_TimeOut;  //显示超时

extern unsigned char RFDataRecFlag;
extern unsigned char Rf_Interval_Flag;  //RF打开间断标志

extern unsigned char Display_Blink_Flag;   //报警值闪烁标志

extern unsigned char Display_Blink_TimeOut;  //报警值设置超时
extern unsigned Key_Set_Mode;  //键盘设置模式

extern unsigned int ModelAddress;  //床位号

unsigned char LogInFlag = 0;  //登陆标志

unsigned char WorkingStatus = 0 ;

extern signed char RSSIValue;

extern unsigned char UartStatusFlag;
extern unsigned char Calflag;                  //by cai 时钟校准标志

extern unsigned int CC1101sysc;
extern unsigned int CC1101fre;

extern void  DataSet();

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
  
  DataSet();
  
  ReadAlarmValue4Flash();

  LEDInit(); 
  LCDInit();
  KeyInit();
  UartInit();

  RadioInit();  //cc1101 初始化
  IRSensorInit();
  
  
  TA0Init();   //定时器初始化
  
  LEDOn(YELLOWLED);
  LEDOn(BLUELED); 
  Delay_ms(500);
  LEDOff(BLUELED);
  LEDOff(YELLOWLED);
  
  GetBatteyLevel();
  
#ifdef   VOICE
  ISD2100Init();
#endif
  
}

#if 1
void  DataSet()
{
  unsigned char flashBuffer[32];
  const unsigned char HardWareVersion[] = "1.00";
  const unsigned char SoftWareVersion[] = "1.00";
  const unsigned char ProductID[] = "0123456789";
  ReadFlash(0x1000,flashBuffer,0x20);
  //CC1101sysc = (flashBuffer[0]<<8) + flashBuffer[1];
 // CC1101fre = (flashBuffer[2]<<8) + flashBuffer[3];
 // CC1101sysc = (0xd3<<8) + 0x09;
  CC1101sysc = (0xd3<<8) + 0x1b;
 // CC1101sysc = (0xd3<<8) + 0x12;
  CC1101fre = (0x2A<<8) + 0x2e;
  if(flashBuffer[4]<'0' ||(flashBuffer[4]>'Z'))
  {
    memcpy(flashBuffer+4,ProductID,10);
  }
  memcpy(flashBuffer+14,HardWareVersion,4);
  memcpy(flashBuffer+18,SoftWareVersion,4);
  ModelAddress = 0x03;
//  ModelAddress = flashBuffer[22];
  Flash_SegmentErase(0x1000);
  FlashWrite_8(flashBuffer,0x1000,0x20);
  ReadFlash(0x1000,flashBuffer,0x20);
//FlashWrite_8(flashBuffer,0x2000,0x20);
  ReadFlash(0x3000,flashBuffer,0x20);
}
#endif

int main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
 // srand(1);
  DriverInit();
  DisplayMode = DisplayInitMode + DisplayRateStartMode;
  __bis_SR_register(GIE);   //全局中断使能


  while(1)
  {
/***************************显示速度模式***************************************/
      if(RateDisplayFlag  == 1)  //数据更新标志
      {
        if((DisplayMode & DisplayTestMode) == DisplayTestMode)
        {
            CurrentSpeed = rand()%100 +40;
            TotalDrop++;
        }
        else if((DisplayMode & DisplayRateMode) == DisplayRateMode)
        {
          CurrentSpeed = GetRate();
        }
        RateDisplayFlag =0;
        DisplayRate(CurrentSpeed); 
        RefreshTime=1;
      }
      else if(RateDisplayFlag  == 2)
      {
        RateDisplayFlag = 3;
        CurrentSpeed = 0;
        DisplayRate(CurrentSpeed); 
      }
      if(RFDataRecFlag == 1)  //数据接收标志
      {
        ReceiveData();    //RF数据接收   
        RecDataCheck();   //接收数据检查
        RFDataRecFlag = 0;
        DisplayRFRSSILevel(RSSIValue);  //
        RSSIValue = -120;
      }
      
/***************************模式***************************************/      
      if(Battery_Display_Flag == 1)  //电池显示
      {
        Battery_Display_Flag = 0;
        BatteyLevel = GetBatteyLevel();
        DisplayBattery( BatteyLevel);
      }
      if(KeyPressFlag)   //按键处理
      {
        if((CurrentSpeed == 0) &&(TotalDrop!=0))//按键按下时 当前速度为零而总滴速不为零   总滴速清零
        {
            IRRecCaptureInit();
            TotalDrop = 0;
        }
        else
        {
        KeyFunction();

        KeyPressFlag = 0;

        }
      }
      if(Display_Blink_Flag)    //设置模式下的数字闪烁模式
      {
        if(Display_Blink_TimeOut >= 20)  //10s超时
        {
          Display_Blink_TimeOut =0;
          Key_Set_Mode &=~ (UPPERSETMODE+LOWERSETMODE);
        }
        RefreshAlarmValue();
        Display_Blink_Flag = 0;  //判断以后再清除
      }
      if((DisplayMode & DisplayTestStartMode) == DisplayTestStartMode)      //开始模拟测试模式状态
      {
        ModelAddress = 188;
        LCDInit();
        DisplayMode &=~ DisplayTestStartMode;
        DisplayMode &=~ DisplayRateMode;
        DisplayMode |= DisplayTestMode;
      }
      if((DisplayMode & DisplayRateStartMode) == DisplayRateStartMode)    //开始正常测试模式状态
      {
       // ModelAddress = RFCHANNAL;
        LCDInit();
        DisplayMode &=~ DisplayRateStartMode;
        DisplayMode &=~ DisplayTestMode;
        DisplayMode |= DisplayRateMode;
      } 


/*********************************语音报警函数************************************/	
      VoiceFunction();



	
/******************************************************************************/
    
/*******************************睡眠模式**************************************/
    if(DisplayMode == DisplaySleepMode)
    {
      __bis_SR_register(GIE+LPM3);
      DisplayMode = DisplayRateMode;
      LCDInit();
    }
    if(Display_All_TimeOut >= SLEEPTIMEOUT)  //
    {
      Display_All_TimeOut = 0;
      DisplayNone();
      DisplayMode = DisplaySleepMode;
    }
/*******************************串口处理函数***************************************/
  if(UartStatusFlag == 1)
  {
    UartStatusFlag = 0;
    UartCommandstatus();
  }
    
 
  }
}





