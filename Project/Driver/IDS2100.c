#include "IDS2100.h"

unsigned char VoiceLevel = 5;  //…˘“Ùµ»º∂
unsigned char VoiceDelayNum = 0;



void ISD2100Init(void)
{
  ISD2100_MOSIDIR |= MOSI;
  ISD2100_SCLKDIR |= SCLK;
  ISD2100_SSBDIR |= SSB;  
  ISD2100_SOMIDIR &=~ SOMI;
  ISD2100_RDYDIR &=~ RDY;
  
  WriteVoiceLevel(VoiceLevel);
}

void ISD2100_send_data(unsigned char data_temp)
{
  unsigned char i;
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
    Delay_ms(1);
    SCLK_HI
    Delay_ms(1);
  }
  SCLK_HI
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
  Delay_ms(10);
}

void ISD2100_power_down()
{
  SSB_LO
  ISD2100_send_data(0x12);
  SSB_HI
  Delay_ms(10);
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
  Delay_ms(50);
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
  Delay_ms(50);
  SSB_LO
  ISD2100_send_data(0xB8);
  ISD2100_send_data(0x03);
  ISD2100_send_data(value);
  SSB_HI
 // ISD2100_power_down();
}

void WriteVoiceLevel(unsigned char level)
{
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
  Delay_ms(700);
  VoicePlay(29);
  Delay_ms(700);
}

void Play_Drop_Stop()
{
  VoicePlay(38);
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
  VoicePlay(39);
}
void Play_TooSlow()
{
  VoicePlay(40);
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



