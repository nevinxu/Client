#include "include.h"

unsigned char VoiceLevel = 5;  //声音等级
extern unsigned int TotalDrop;
extern unsigned int RateDisplayFlag;
unsigned int VoiceDelayNum = 0;

void ISD2100Init(void)
{
  ISD2100_MOSIDIR |= MOSI;
  ISD2100_SCLKDIR |= SCLK;
  ISD2100_SSBDIR |= SSB;  
  ISD2100_SOMIDIR &=~ SOMI;
  ISD2100_RDYDIR &=~ RDY;
  
  WriteVoiceLevel(VoiceLevel);
  DisplayVoice(VoiceLevel);

}

void ISD2100_send_data(unsigned char data_temp)
{
  unsigned char i;
  if(VoiceDelayNum == 0)
  {
      for(i=0;i<8;i++)
      { 
        SCLK_LO
        if((data_temp&0x80)==0)
        {
          MOSI_LO
        }
        else
        {
          MOSI_HI
        }
        data_temp=(data_temp<<1);
        Delay_ms(5);
        SCLK_HI
        Delay_ms(5);
      }
      SCLK_HI
  }
}

unsigned char ISD2100_receive_data()
{
  unsigned char data;
  unsigned char i;
  for(i=0;i<8;i++)
  { 
    SCLK_LO
    Delay_ms(1);
    SCLK_HI
    data<<=1;  
    if(SOMIIN&SOMI)
    {
      data |=0x01;
    }
    Delay_ms(1);
  }
  SCLK_HI
    return data;

}

void ISD2100_power_up()
{
  SSB_LO
  ISD2100_send_data(0x10);
  SSB_HI
  Delay_ms(1);
}

void ISD2100_power_down()
{
  SSB_LO
  ISD2100_send_data(0x12);
  SSB_HI
  Delay_ms(1);
}

void VoicePlay(unsigned int num)
{
  WriteVoiceLevel(VoiceLevel);
  while(!(RDYIN&RDY));
  ISD2100_power_up();
  SSB_LO
  ISD2100_send_data(0xB0);
  ISD2100_send_data((num&0xff00)>>8);
  ISD2100_send_data(num&0xff);
  SSB_HI
 // ISD2100_power_down();
}

unsigned char ReadVoice()
{
  unsigned char value;
  ISD2100_power_up();
  Delay_ms(1);
  SSB_LO
  ISD2100_send_data(0xBA);
  ISD2100_send_data(0x03);
  //Delay_us(50);
  //while((RDYIN&RDY));
  value = ISD2100_receive_data();
  SSB_HI
//  ISD2100_power_down();
  return value;
}
                     
void WriteVoice(unsigned char value)
{
  while(!(RDYIN&RDY));
  ISD2100_power_up();
  Delay_ms(10);
  SSB_LO
  ISD2100_send_data(0xB8);
  ISD2100_send_data(0x03);
  ISD2100_send_data(value);
  SSB_HI
 // ISD2100_power_down();
}

void WriteVoiceLevel(unsigned char level)
{
 // VoiceDelayNum = 0;
  if(level == 5)
  {
    WriteVoice(0);
  }
  else if(level == 4)
  {
    WriteVoice(0x3);
  }
  else if(level == 3)
  {
    WriteVoice(0x10);
  }
  else if(level == 2)
  {
    WriteVoice(0x20);
  }
  else if(level == 1)
  {
    WriteVoice(0x30);
  }
  else if(level == 0)
  {
    WriteVoice(0x7f);
  }
}

void Play_Drop_Start()
{
  VoicePlay(28);
}

void Play_Drop_Stop()
{
  static unsigned char flag = 0;
  if((flag == 0)&&(VoiceDelayNum == 0))
  {
    flag = 1;
    VoicePlay(34);
    VoiceDelayNum = 70; 
  }
  else if((flag == 1) &&(VoiceDelayNum == 0))
  {
    flag = 2;
    VoicePlay(34);
    VoiceDelayNum = 70; 
  }
   else if((flag == 2) &&(VoiceDelayNum == 0)) 
   {
    VoicePlay(32); 
    RateDisplayFlag = 0;
    VoiceDelayNum = 200; 
    flag = 0;
    TotalDrop = 0;
   }

}
void VoiceUp()
{
  unsigned char value;
  value = ReadVoice();
  if(!(value&0xff))
  {     
    value = 0;
  }
  else
  {
    value>>=1;
  }
  WriteVoice(value);
}

void Play_OverQuick()
{
  VoicePlay(30);
}
void Play_TooSlow()
{
  VoicePlay(31);
}

void VoiceDown()
{
  unsigned char value;
  value = ReadVoice();
  if(!(value&0x80))
  {     
    value = 0x80;
  }
  else
  {
    value<<=1;
  }
  WriteVoice(value);
}

void Play_LowBattery()
{
  VoicePlay(29);
}



void VoiceFunction()
{
  if(TotalDrop == 3)
  {
    Play_Drop_Start();
    VoiceDelayNum = 50;  //100ms
  }
  if((RateDisplayFlag == 3) &&(TotalDrop>0))
  {
    Play_Drop_Stop();
    RadioInit();  //cc1101 初始化
  }
//  if(BatteyLevel == 0)
//  {
//    Play_LowBattery();
//  }
}



