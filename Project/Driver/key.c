#include "include.h"

unsigned char RFDataRecFlag;
extern unsigned char Rf_Interval_Num;  //RF�򿪼��ʱ��
extern unsigned char KeyPressFlag;   //������־

unsigned char keyPressLongFlag;  //������ʾ
unsigned int keyPressLongNum;  //������ʱ����

extern unsigned char AlarmUpperValue;
extern unsigned char AlarmLowerValue;

unsigned Key_Set_Mode = CLOSE;  //��������ģʽ
extern unsigned char Display_Blink_Flag;   //����ֵ��˸��־
extern unsigned char VoiceLevel;  //�����ȼ�

extern unsigned char Display_Blink_TimeOut;  //����ֵ���ó�ʱ

extern unsigned char Display_All_TimeOut;  //��ʾ��ʱ
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
  if(KeyPressFlag == KeyUpperLimit)   //����ֵ���ް�������
  {
    if((Key_Set_Mode & UPPERSETMODE) == CLOSE)    //�����벻����ģʽ�л�
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
  else if(KeyPressFlag == KeyLowLimit)  //����ֵ���ް�������
  {
    if((Key_Set_Mode & LOWERSETMODE) == CLOSE)      //�����벻����ģʽ�л�
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
  else if(KeyPressFlag == KeyvolumeUp)  //����������
  {
    if(Key_Set_Mode == UPPERSETMODE)   //����ģʽ��  ������������
    {
      AlarmUpperValue++;
      if(AlarmUpperValue>=200)  //�������ô���200
      {
        AlarmUpperValue = AlarmLowerValue + 20; //���������޵Ĳ��Ϊ20
      }
   //   WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue++;
      if(AlarmLowerValue >= (AlarmUpperValue - 20))  //���������޵Ĳ��С��20
      {
        AlarmLowerValue = 30;
      }
   //   WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
    }
#ifdef VOICE   //��������
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
 //   WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
    Display_Blink_TimeOut =0;
  }
  else if(KeyPressFlag == KeyvolumeDown) //����������
  {
    if(Key_Set_Mode == UPPERSETMODE)   //����ģʽ��  ������������
    {
      AlarmUpperValue--;
      if(AlarmUpperValue <= AlarmLowerValue + 20)
      {
        AlarmUpperValue = 199;
      }
    //  WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
    }
    else if(Key_Set_Mode == LOWERSETMODE)
    {
      AlarmLowerValue--;
      if(AlarmLowerValue <= 30)
      {
        AlarmLowerValue = AlarmUpperValue - 20;
      }
   //   WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
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
  static unsigned char TestModeNum = 0;  //�������ģʽ�ķ�ʽ����
/****************************cc1101�����ж�************************************/    
    if(P1IFG & TI_CC_GDO2_PIN)
    {
   //  TI_CC_GDO0_PxIE &=~   (TI_CC_GDO2_PIN);   // Disable the interrupt   
     RFDataRecFlag = 1;   //���ݽ��ձ�־ 
//      RecDataCheck();   //�������ݼ��
      i++;
    }
/******************************************************************************/ 
    
/***************************���ϰ����ж�***************************************/    
    if(KeyIFG & KeyvolumeUp)
    {
      Delay_ms(5);     //���� 
      keyPressLongNum = 0;
      if((KeyIES & KeyvolumeUp) == KeyvolumeUp)   //�ſ����������ж�
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
            TestModeNum = 6;   //�˳�����ģʽ
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
        KeyIES &=~ KeyvolumeUp;  //ȡ��
      }
      else  //���°��������ж�
      {
        if(KeyIN&KeyvolumeUp)
        {
          keyPressLongFlag = NONE;
        }
        KeyIES |= KeyvolumeUp;  //ȡ��
      }
    }
/******************************���°����ж�*************************************/ 
    else if(KeyIFG & KeyvolumeDown)
    {
      Delay_ms(5);     //����  
      keyPressLongNum = 0;
      if((KeyIES & KeyvolumeDown) == KeyvolumeDown)   //�ſ����������ж�
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
        KeyIES &=~ KeyvolumeDown;  //ȡ��
      }
      else  //���°��������ж�
      {
        if(KeyIN&KeyvolumeDown)
        {
          keyPressLongFlag = NONE;
        }
        KeyIES |= KeyvolumeDown;  //ȡ��
      }
      
    }
/****************************����ֵ�����ж�************************************/ 
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
/****************************����ֵ�����ж�************************************/ 
    else if(KeyIFG & KeyLowLimit)
    {
       Delay_ms(5);
       if(!(KeyIN&KeyLowLimit))
       {
          KeyPressFlag = KeyLowLimit;
/******************************************************************************/
          if(TestModeNum == 3)
          {
            TestModeNum = 10;    //�������ģʽ
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
      KeyPressFlag = 0;   //���ȫ�������ж�
    }
    
    P1IFG = 0;  
//    LPM3_EXIT;
      
}