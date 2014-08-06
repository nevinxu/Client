#include "include.h"

unsigned char RFDataRecFlag;
extern unsigned char Rf_Interval_Num;  //RF打开间断时间
extern unsigned char KeyPressFlag;   //按键标志

unsigned char keyPressLongFlag;  //长按表示
unsigned int keyPressLongNum;  //长按定时计数

extern unsigned char AlarmUpperValue;
extern unsigned char AlarmLowerValue;

unsigned Key_Set_Mode = CLOSE;  //键盘设置模式
extern unsigned char Display_Blink_Flag;   //报警值闪烁标志
extern unsigned char VoiceLevel;  //声音等级

extern unsigned char Display_Blink_TimeOut;  //报警值设置超时

extern unsigned char Display_All_TimeOut;  //显示超时
extern unsigned int DisplayMode;

void KeyInit()
{
  KeyDIR &=~ (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
  KeyIES |= (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
  KeyIFG &=~ (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
  KeyIE |= (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
}

void KeyFunction()
{
  if(KeyPressFlag == KeyUpperLimit)   //报警值上限按键按下
  {
    if((Key_Set_Mode & UPPERSETMODE) == CLOSE)    //设置与不设置模式切换
    {
      Key_Set_Mode |= UPPERSETMODE;
    }
    else if((Key_Set_Mode & UPPERSETMODE) == UPPERSETMODE)
    {
      Key_Set_Mode &=~ UPPERSETMODE;
      Display_Blink_Flag = 0;
    }
    if((Key_Set_Mode & LOWERSETMODE) == LOWERSETMODE)
    {
      Display_Blink_Flag = 0;
    }
    RefreshAlarmValue() ;
  if((Key_Set_Mode & LOWERSETMODE) == LOWERSETMODE)
  {
      if(Display_Blink_Flag == 0)
      {
        Key_Set_Mode &=~ LOWERSETMODE;
      }
  }
  }
  else if(KeyPressFlag == KeyLowLimit)  //报警值下限按键按下
  {
    if((Key_Set_Mode & LOWERSETMODE) == CLOSE)      //设置与不设置模式切换
    {
      Key_Set_Mode |= LOWERSETMODE;
    }
    else if((Key_Set_Mode & LOWERSETMODE) == LOWERSETMODE)
    {
      Key_Set_Mode &=~ LOWERSETMODE;
      Display_Blink_Flag = 0;
    }
    if((Key_Set_Mode & UPPERSETMODE) == UPPERSETMODE)
    {
      Display_Blink_Flag = 0;
    }
    RefreshAlarmValue() ;
  if((Key_Set_Mode & UPPERSETMODE) == UPPERSETMODE)
  {
      if(Display_Blink_Flag == 0)
      {
        Key_Set_Mode &=~ UPPERSETMODE;
      }
  }
  }
  else if(KeyPressFlag == KeyvolumeUp)  //增按键按下
  {
    if(Key_Set_Mode == UPPERSETMODE)   //设置模式下  报警上限设置
    {
      AlarmUpperValue++;
      if(AlarmUpperValue>=200)  //上限设置大于200
      {
        AlarmUpperValue = AlarmLowerValue + 20; //下限与上限的差距为20
      }
   //   WriteAlarmValue2Flash();  //需要修改  不能写这么频繁
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue++;
      if(AlarmLowerValue >= (AlarmUpperValue - 20))  //下限与上限的差距小于20
      {
        AlarmLowerValue = 30;
      }
   //   WriteAlarmValue2Flash();  //需要修改  不能写这么频繁
    }
#ifdef VOICE   //声音功能
    else if(Key_Set_Mode == CLOSE)
    {
      VoiceLevel++;
      if(VoiceLevel >= 6)
      {
        VoiceLevel = 0;
      }
      DisplayVoice(VoiceLevel);
      if(VoiceLevel >= VOICELEVELMID)
      {
        DisplayMoon(OFF);
        DisplaySun(ON);
      }
      else
      {
        DisplayMoon(ON);
        DisplaySun(OFF);
      }
      WriteVoiceLevel(VoiceLevel);
   //   VoicePlay(14);
    }
#endif
 //   WriteAlarmValue2Flash();  //需要修改  不能写这么频繁
    Display_Blink_TimeOut =0;
  }
  else if(KeyPressFlag == KeyvolumeDown) //减按键按下
  {
    if(Key_Set_Mode == UPPERSETMODE)   //设置模式下  报警下限设置
    {
      AlarmUpperValue--;
      if(AlarmUpperValue <= AlarmLowerValue + 20)
      {
        AlarmUpperValue = 199;
      }
    //  WriteAlarmValue2Flash();  //需要修改  不能写这么频繁
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue--;
      if(AlarmLowerValue <= 30)
      {
        AlarmLowerValue = AlarmUpperValue - 20;
      }
   //   WriteAlarmValue2Flash();  //需要修改  不能写这么频繁
    }
#ifdef VOICE
    else if(Key_Set_Mode == CLOSE)
    {
      VoiceLevel--;
      if(VoiceLevel >= 6)
      {
        VoiceLevel = 5;
      }
      DisplayVoice(VoiceLevel);
      if(VoiceLevel >= VOICELEVELMID)
      {
        DisplayMoon(OFF);
        DisplaySun(ON);
      }
      else
      {
        DisplayMoon(ON);
        DisplaySun(OFF);
      }
      WriteVoiceLevel(VoiceLevel);
    //  VoicePlay(14);
    }
#endif
  //  WriteAlarmValue2Flash(); 
    Display_Blink_TimeOut =0;
  }


}

#pragma vector = PORT1_VECTOR
__interrupt void port1_ISR (void)
{
  static unsigned int i;
  Display_All_TimeOut = 0;
  static unsigned char TestModeNum = 0;  //进入测试模式的方式计数
/****************************cc1101接收中断************************************/    
    if(P1IFG & TI_CC_GDO2_PIN)
    {
   //  TI_CC_GDO0_PxIE &=~   (TI_CC_GDO2_PIN);   // Disable the interrupt   
     RFDataRecFlag = 1;   //数据接收标志 
//      RecDataCheck();   //接收数据检查
      i++;
    }
/******************************************************************************/ 
    
/***************************向上按键中断***************************************/    
    if(KeyIFG & KeyvolumeUp)
    {
      Delay_ms(5);     //消抖 
      keyPressLongNum = 0;
      if((KeyIES & KeyvolumeUp) == KeyvolumeUp)   //放开按键产生中断
      {
        if(!(KeyIN&KeyvolumeUp))
        {
          KeyPressFlag = KeyvolumeUp;
          keyPressLongFlag = KeyvolumeUp;
          
/******************************************************************************/
          if(TestModeNum == 0)
          {
            TestModeNum = 1;
          }
          else if(TestModeNum == 7)
          {
            TestModeNum = 6;   //退出测试模式
            DisplayMode &=~ DisplayTestMode;
            DisplayMode |= DisplayRateStartMode;
            KeyPressFlag = 0;
            Key_Set_Mode = 0;
          }
          else
          {
            TestModeNum = 1;
          }
/******************************************************************************/        
        }
        KeyIES &=~ KeyvolumeUp;  //取反
      }
      else  //按下按键产生中断
      {
        if(KeyIN&KeyvolumeUp)
        {
          keyPressLongFlag = NONE;
        }
        KeyIES |= KeyvolumeUp;  //取反
      }
    }
/******************************向下按键中断*************************************/ 
    else if(KeyIFG & KeyvolumeDown)
    {
      Delay_ms(5);     //消抖  
      keyPressLongNum = 0;
      if((KeyIES & KeyvolumeDown) == KeyvolumeDown)   //放开按键产生中断
      {
        if(!(KeyIN&KeyvolumeDown))
        {
          KeyPressFlag = KeyvolumeDown;
          keyPressLongFlag = KeyvolumeDown;
/******************************************************************************/
          if(TestModeNum == 1)
          {
            TestModeNum = 2;
          }
          else if(TestModeNum == 8)
          {
            TestModeNum = 7;
          }
          else
          {
            TestModeNum = 0;
          }
/******************************************************************************/
        }
        KeyIES &=~ KeyvolumeDown;  //取反
      }
      else  //按下按键产生中断
      {
        if(KeyIN&KeyvolumeDown)
        {
          keyPressLongFlag = NONE;
        }
        KeyIES |= KeyvolumeDown;  //取反
      }
      
    }
/****************************上限值按键中断************************************/ 
    else if(KeyIFG & KeyUpperLimit)
    {
       Delay_ms(5);
       if(!(KeyIN&KeyUpperLimit))
       {
          KeyPressFlag = KeyUpperLimit;
/******************************************************************************/
          if(TestModeNum == 2)
          {
            TestModeNum = 3;
          }
          else if(TestModeNum == 9)
          {
            TestModeNum = 8;
          }
          else
          {
            TestModeNum = 0;
          }
/******************************************************************************/
       }
    }
/****************************下限值按键中断************************************/ 
    else if(KeyIFG & KeyLowLimit)
    {
       Delay_ms(5);
       if(!(KeyIN&KeyLowLimit))
       {
          KeyPressFlag = KeyLowLimit;
/******************************************************************************/
          if(TestModeNum == 3)
          {
            TestModeNum = 10;    //进入测试模式
            DisplayMode |= DisplayTestStartMode;
            KeyPressFlag = 0;
            Key_Set_Mode = 0;
          }
          else if(TestModeNum == 10)
          {
            TestModeNum = 9;
          }
          else
          {
            TestModeNum = 9;   //
          }
/******************************************************************************/
       }
    }
    else
    {
      KeyPressFlag = 0;   //清除全部按键中断
    }
    
    P1IFG = 0;  
//    LPM3_EXIT;
      
}