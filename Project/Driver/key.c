#include "key.h"

extern unsigned char DataRecFlag;
extern unsigned char Rf_Interval_Num;  //RF打开间断时间
extern unsigned char KeyPressFlag;   //按键标志
unsigned char keyPressLongFlag;  //长按表示

extern unsigned char AlarmUpperValue;
extern unsigned char AlarmLowerValue;

unsigned Key_Set_Mode = CLOSE;  //键盘设置模式
extern unsigned char Display_Blink_Flag;   //报警值闪烁标志
extern unsigned char VoiceLevel;  //声音等级

extern unsigned char Display_Blink_TimeOut;  //报警值设置超时

void KeyInit()
{
  KeyDIR &=~ (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
  KeyIES &=~ (KeyvolumeUp+KeyvolumeDown+KeyUpperLimit+KeyLowLimit);
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
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue++;
      if(AlarmLowerValue >= (AlarmUpperValue - 20))  //下限与上限的差距小于20
      {
        AlarmLowerValue = 30;
      }
    }
    else if(Key_Set_Mode == CLOSE)
    {
      VoiceLevel++;
      if(VoiceLevel >= 6)
      {
        VoiceLevel = 0;
      }
      DisplayVoice(VoiceLevel);
      WriteVoiceLevel(VoiceLevel);
      VoicePlay(14);
    }
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
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue--;
      if(AlarmLowerValue <= 30)
      {
        AlarmLowerValue = AlarmUpperValue - 20;
      }
    }
    else if(Key_Set_Mode == CLOSE)
    {
      VoiceLevel--;
      if(VoiceLevel >= 6)
      {
        VoiceLevel = 5;
      }
      DisplayVoice(VoiceLevel);
      WriteVoiceLevel(VoiceLevel);
      VoicePlay(14);
    }
    Display_Blink_TimeOut =0;
  }


}

#pragma vector = PORT1_VECTOR
__interrupt void port1_ISR (void)
{
    if(P1IFG & TI_CC_GDO2_PIN)
    {
      DataRecFlag = 1;   //数据接收标志
      Rf_Interval_Num = 0;  //RF打开间断时间
    }
    if(KeyIFG & KeyvolumeUp)
    {
       Delay_ms(50);     //消抖  
       if(KeyIN&KeyvolumeUp)
       {
          KeyPressFlag = KeyvolumeUp;
       }
    }
    else if(KeyIFG & KeyvolumeDown)
    {
       Delay_ms(50);
       if(KeyIN&KeyvolumeDown)
       {
          KeyPressFlag = KeyvolumeDown;
       }
    }
    else if(KeyIFG & KeyUpperLimit)
    {
       Delay_ms(50);
       if(KeyIN&KeyUpperLimit)
       {
          KeyPressFlag = KeyUpperLimit;
       }
    }
    else if(KeyIFG & KeyLowLimit)
    {
       Delay_ms(50);
       if(KeyIN&KeyLowLimit)
       {
          KeyPressFlag = KeyLowLimit;
       }
    }
    else
    {
      KeyPressFlag = 0;
    }
    
    P1IFG = 0;  
      
}