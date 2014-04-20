#include "key.h"

extern unsigned char DataRecFlag;
extern unsigned char Rf_Interval_Num;  //RF�򿪼��ʱ��
extern unsigned char KeyPressFlag;   //������־
unsigned char keyPressLongFlag;  //������ʾ

extern unsigned char AlarmUpperValue;
extern unsigned char AlarmLowerValue;

unsigned Key_Set_Mode = CLOSE;  //��������ģʽ
extern unsigned char Display_Blink_Flag;   //����ֵ��˸��־
extern unsigned char VoiceLevel;  //�����ȼ�

extern unsigned char Display_Blink_TimeOut;  //����ֵ���ó�ʱ

extern unsigned char Display_All_TimeOut;  //��ʾ��ʱ
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
    WriteAlarmValue2Flash();  //��Ҫ�޸�  ����д��ôƵ��
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
    WriteAlarmValue2Flash(); 
    Display_Blink_TimeOut =0;
  }


}

#pragma vector = PORT1_VECTOR
__interrupt void port1_ISR (void)
{
  Display_All_TimeOut = 0;
    if(P1IFG & TI_CC_GDO2_PIN)
    {
      DataRecFlag = 1;   //���ݽ��ձ�־
      Rf_Interval_Num = 0;  //RF�򿪼��ʱ��
    }
    if(KeyIFG & KeyvolumeUp)
    {
      Delay_ms(50);     //����  
      if((KeyIES & KeyvolumeUp) == KeyvolumeUp)   //�ſ����������ж�
      {
        if(!(KeyIN&KeyvolumeUp))
        {
          keyPressLongFlag = 0;
        }
        KeyIES &=~ KeyvolumeUp;  //ȡ��
      }
      else  //���°��������ж�
      {
        if(KeyIN&KeyvolumeUp)
        {
          KeyPressFlag = KeyvolumeUp;
          keyPressLongFlag = 1;
        }
        KeyIES |= KeyvolumeUp;  //ȡ��
      }
    }
    else if(KeyIFG & KeyvolumeDown)
    {
      Delay_ms(50);     //����  
      if((KeyIES & KeyvolumeDown) == KeyvolumeDown)   //�ſ����������ж�
      {
        if(!(KeyIN&KeyvolumeDown))
        {
          keyPressLongFlag = 0;
        }
        KeyIES &=~ KeyvolumeDown;  //ȡ��
      }
      else  //���°��������ж�
      {
        if(KeyIN&KeyvolumeDown)
        {
          KeyPressFlag = KeyvolumeDown;
          keyPressLongFlag = 1;
        }
        KeyIES |= KeyvolumeDown;  //ȡ��
      }
      
    }
    else if(KeyIFG & KeyUpperLimit)
    {
       Delay_ms(50);
       if(!(KeyIN&KeyUpperLimit))
       {
          KeyPressFlag = KeyUpperLimit;
       }
    }
    else if(KeyIFG & KeyLowLimit)
    {
       Delay_ms(50);
       if(!(KeyIN&KeyLowLimit))
       {
          KeyPressFlag = KeyLowLimit;
       }
    }
    else
    {
      KeyPressFlag = 0;
    }
    
    P1IFG = 0;  
    LPM3_EXIT;
      
}