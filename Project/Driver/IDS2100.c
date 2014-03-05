#include "IDS2100.h"

void InitISD2100(void)
{
  ISD2100_MOSIDIR |= MOSI;
  ISD2100_SCLKDIR |= SCLK;
  ISD2100_SSBDIR |= SSB;  
  ISD2100_SOMIDIR &=~ SOMI;
  ISD2100_RDYDIR &=~ RDY;
  
  WriteVoice(0x01);
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
  ISD2100_power_up();
  SSB_LO
  ISD2100_send_data(0xB0);
  ISD2100_send_data((num&0xff00)>>8);
  ISD2100_send_data(num&0xff);
  SSB_HI
  ISD2100_power_down();
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
  ISD2100_power_up();
  Delay_ms(50);
  SSB_LO
  ISD2100_send_data(0xB8);
  ISD2100_send_data(0x03);
  ISD2100_send_data(value);
  SSB_HI
 // ISD2100_power_down();
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



