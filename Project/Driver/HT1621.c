#include "include.h"

//unsigned char   seg_tab[]={0xbe,0x06,0xd6,0x97,0x47,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};原版
unsigned char seg_tab1[10]={0xbe,0x06,0x7c,0x5e,0xc6,0xda,0xfa,0x0e,0xfe,0xde}; //下半部分
unsigned char seg_tab2[10]={0x7d,0x60,0x3e,0x7a,0x63,0x5b,0x5f,0x70,0x7f,0x7b}; //上半部分
void Ht1621Wr_Data(unsigned char Data,unsigned char cnt) 
{ 
  unsigned char i; 
  for (i=0;i<cnt;i++) 
  {   
    WR_LO
    Delay_us(2);  
    if(Data&0x80)
    {
      DATA_HI
    }
    else
    {
      DATA_LO
    }

    Delay_us(2);  
    WR_HI 
    Delay_us(2);  
    Data<<=1;  
  } 
} 
void Ht1621WrCmd(unsigned char Cmd) 
{ 
  CS_LO 
  Delay_us(2); 
  Ht1621Wr_Data(0x80,4); //D′è??üá?±ê??100 
  Ht1621Wr_Data(Cmd,8); //D′è??üá?êy?Y 
  CS_HI 
  Delay_us(2);   
} 

void Ht1621_Init(void) 
{ 
  CS_HI 
 Delay_us(2); 
  WR_HI 
  Delay_us(2);  
  DATA_HI 
  Delay_ms(100);      //?óê±ê1LCD1¤×÷μ??1?è?¨ 
  Ht1621WrCmd(BIAS); 
  Ht1621WrCmd(RC256);     //ê1ó??ú2???μ′?÷ 
  Ht1621WrCmd(SYSDIS); 
  Ht1621WrCmd(WDTDIS); 
  Ht1621WrCmd(SYSEN); 
  Ht1621WrCmd(LCD_ON); 
} 

void Ht1621WrHalfOneData(unsigned char Addr,unsigned char Data)    //高4位有效
{ 
  CS_LO 
  Ht1621Wr_Data(0xa0,3); //D′è?êy?Y±ê??101 
  Ht1621Wr_Data(Addr<<2,6); //D′è?μ??·êy?Y 
  Ht1621Wr_Data(Data,4); //D′è?êy?Y 
  CS_HI   
  Delay_us(1);   
} 

void Ht1621WrOneData(unsigned char Addr,unsigned char Data) 
{ 
  CS_LO 
  Ht1621Wr_Data(0xa0,3); //D′è?êy?Y±ê??101 
  Ht1621Wr_Data(Addr<<2,6); //D′è?μ??·êy?Y 
  Ht1621Wr_Data(Data,8); //D′è?êy?Y 
  CS_HI   
  Delay_us(1);   
} 


void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt) 
{ 
  unsigned char i; 
  CS_LO 
  Ht1621Wr_Data(0xa0,3); //D′è?êy?Y±ê??101 
  Ht1621Wr_Data(Addr<<2,6); //D′è?μ??·êy?Y 
  for (i=0;i<cnt;i++) 
  { 
   Ht1621Wr_Data(*p,8); //D′è?êy?Y 
p++; 
  } 
  CS_HI   
  Delay_us(1);  
} 

void Ht1621ClearDisplay() 
{ 
  unsigned char i; 
  CS_LO 
  Ht1621Wr_Data(0xa0,3); //D′è?êy?Y±ê??101 
  Ht1621Wr_Data(0,6); //D′è?μ??·êy?Y 
  for (i=0;i<16;i++) 
  { 
   Ht1621Wr_Data(0,8); //D′è?êy?Y  
  } 
  CS_HI   
  Delay_us(2); 
} 

void Ht1621WriteNum(unsigned char type,unsigned char addr,unsigned char num)
{
  if(type == UPLINE)
   Ht1621WrOneData(addr,seg_tab1[num]);
  else
    Ht1621WrOneData(addr,seg_tab2[num]);
}

void Display_Digit(unsigned char type, unsigned char add ,signed int Data )
{
  unsigned char S,H,T,A;
  S=Data/1000;
  H=Data%1000/100;
  T=Data%100/10;
  A=Data%10;
  if((Data >=0)  || (type & GIGTALCLOCKFLAG))
  {
    if(type & UPLINE)
    {
      Ht1621WrOneData(add,seg_tab2[A]);
    }
    else
    {
      Ht1621WrOneData(add,seg_tab1[A]);
    }
  }
  else
  {
    if(type & UPLINE)
    {
      Ht1621WrOneData(add,0);
    }
    else
    {
      Ht1621WrOneData(add,0);
    }
  }
  if((Data >=10)  || (type & GIGTALCLOCKFLAG))
  {
    if(type & UPLINE)
    {
      Ht1621WrOneData(add+2,seg_tab2[T]);
    }
    else
    {
      Ht1621WrOneData(add-2,seg_tab1[T]);
    }
  }
  else
  {
    if(type & UPLINE)
    {
      Ht1621WrOneData(add+2,0);
    }
    else
    {
      Ht1621WrOneData(add-2,0);
    }
  }
  if(type & RATEFLAG)
  {
    if((Data >=100))
    {
      if(type & UPLINE)
      {
        Ht1621WrOneData(add+4,seg_tab2[H]);
      }
      else
      {
        Ht1621WrOneData(add-4,seg_tab1[H]);
      }
    }
    else
    {
      if(Data & UPLINE)
      {
        Ht1621WrOneData(add+4,0);
      }
      else
      {
        Ht1621WrOneData(add-4,0);
      }
    }
    if((Data >=1000))
    {
      if(type & UPLINE)
      {
        Ht1621WrOneData(add+6,seg_tab2[S]);
      }
      else
      {
        Ht1621WrOneData(add-6,seg_tab1[S]);
      }
    }
    else
    {
      if(Data & UPLINE)
      {
        Ht1621WrOneData(add+6,0);
      }
      else
      {
        Ht1621WrOneData(add-6,0);
      }
    }
  }
}

void DisplayRate(unsigned int rate)
{
  Ht1621ClearDisplay();
#ifdef TestMode
  static unsigned int Display_Times=0;
  Display_Times++;
  Display_Digit(DOWNLINE+RATEFLAG, 11, Display_Times);
#endif
  if(rate<10)
  {
    rate =0;
  }
  Display_Digit(UPLINE+RATEFLAG, 23, rate);

}

