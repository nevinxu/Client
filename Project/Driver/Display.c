#include "include.h"

const unsigned char BedNumBuf[] = {0xf5,0x60,0xd3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};
const unsigned char RateNumBuf[] = {0xaf,0x06,0xcb,0x4f,0x66,0x6d,0xed,0x07,0xef,0x6f};
const unsigned char AlarmNumBuf[] = {0xaf,0x06,0xcb,0x4f,0x66,0x6d,0xed,0x07,0xef,0x6f};

unsigned char AlarmUpperValue = 120;  //报警上限值
unsigned char AlarmLowerValue = 40; //报警下限值

unsigned int TotalDrop = 0;  //总滴液

extern unsigned char VoiceLevel;;  //声音等级

unsigned char Display_Blink_Flag = 0;   //报警值闪烁标志
extern unsigned Key_Set_Mode;  //键盘设置模式

char digit[11] = {
0xD7,                    // displays "0"
0x06,                    // displays "1"
0xE3,                    // displays "2"
0xA7,                    // displays "3"
0x36,                    // displays "4"
0xB5,                    // displays "5"
0xF5,                    // displays "6"
0x07,                    // displays "7"
0xF7,                    // displays "8"
0xB7,                    // displays "9"
0x00                     // displays " "
};

void LEDInit(void)
{
  LEDDIR |= YELLOWLED+BLUELED;
  LEDOUT |= YELLOWLED+BLUELED;
}

void  LEDOff(unsigned char num)
{
  if(num == YELLOWLED)
  {
    LEDOUT |= YELLOWLED;
  }
  else if(num == BLUELED)
  {
    LEDOUT |= BLUELED;
  }
}

void  LEDOn(unsigned char num)
{
  if(num == YELLOWLED)
  {
    LEDOUT &=~ YELLOWLED;
  }
  else if(num == BLUELED)
  {
    LEDOUT &=~ BLUELED;
  }
}

void LCDInit(void)
{
  int i;
  
  P5SEL = 0xFE;
  P4SEL = 0xFF;
  P2SEL = 0xFF;
  P3SEL = 0xFF;
  // Configure LCD_A
  LCDACTL = LCDFREQ_128 | LCDMX1 | LCDMX0 | LCDSON | LCDON;//
  LCDAPCTL0 = LCDS0 | LCDS4 | LCDS8 | LCDS12 | LCDS16 | LCDS20;
  LCDAPCTL1 = 0;
  LCDAVCTL0 = LCDCPEN;
  LCDAVCTL1 = VLCD_3_02;
  
  for (i=0; i<26; i++)                 //清屏
  LCDMEM[i]=0x0;
  DisplayLine(1);   //显示框架
  DisplayBattery(3);
  DisplayMoon(ON);
  DisplaySun(ON);
  DisplayBedNum(188);
 // DisplayTotalDrop();
  DisplayRate(0);
  DisplayVoice(VoiceLevel);
  DisplayUpAlarm(AlarmUpperValue);
  DisplayDownAlarm(AlarmLowerValue);

}

void  DisplayNone()
{
  int i;
  for (i=0; i<26; i++)                 //清屏
  LCDMEM[i]=0x0;
}

void RefreshAlarmValue()   //刷新报警值
{

  if((Key_Set_Mode & UPPERSETMODE) == UPPERSETMODE)
  {
    if(Display_Blink_Flag != 2)   //显示数字   
    {
      DisplayUpAlarm(AlarmUpperValue);
    }
    else if(Display_Blink_Flag == 2)  //不显示数字
    {
      DisplayUpAlarm(0xff);
    }
  }
  if((Key_Set_Mode & LOWERSETMODE) == LOWERSETMODE)
  {
    
    if(Display_Blink_Flag != 2)   //显示数字   
    {
      DisplayDownAlarm(AlarmLowerValue);
    }
    else if(Display_Blink_Flag == 2)  //不显示数字
    {
      DisplayDownAlarm(0xff);
    }
  }
  if((Key_Set_Mode & (UPPERSETMODE+LOWERSETMODE)) == CLOSE)
  {
    DisplayUpAlarm(AlarmUpperValue);
    DisplayDownAlarm(AlarmLowerValue);
  }
}

void DisplayLine(unsigned char status)   //显示框线
{
  if(status == ON)
  {
  LCDMEM[0] |= BIT0;
  }
  else
  {
  LCDMEM[0] &=~ BIT0;
  }
}

void DisplayBattery(unsigned int level)
{
  LCDMEM[9] |= BIT7;
    if(level == 3)
    {
        LCDMEM[5] |= BIT4;
        LCDMEM[6] |= BIT4;
        LCDMEM[7] |= BIT4;
    }
    else if(level == 2)
    {
        LCDMEM[5] |= BIT4;
        LCDMEM[6] |= BIT4;
        LCDMEM[7] &=~ BIT4;
    }
    else if(level == 1)
    {
        LCDMEM[5] |= BIT4;
        LCDMEM[6] &=~ BIT4;
        LCDMEM[7] &=~ BIT4;
    }
    else if(level == 0)
    {
        LCDMEM[5] &=~ BIT4;
        LCDMEM[6] &=~ BIT4;
        LCDMEM[7] &=~ BIT4;
    }
      
}


void DisplayVoice(unsigned char level)
{
  if(level >= 5)
  {
    LCDMEM[3] |= BIT3;
  }
  else
  {
    LCDMEM[3] &=~ BIT3;
  }
  if(level >= 4)
  {
    LCDMEM[3] |= BIT2;
  }
  else
  {
    LCDMEM[3] &=~ BIT2;
  }
  if(level >= 3)
  {
    LCDMEM[3] |= BIT1;
  }
  else
  {
    LCDMEM[3] &=~ BIT1;
  }
  if(level >= 2)
  {
    LCDMEM[3] |= BIT0;
  }
  else
  {
    LCDMEM[3] &=~ BIT0;
  }
  if(level >= 1)
  {
    LCDMEM[2] |= BIT4;
  }
  else
  {
    LCDMEM[2] &=~ BIT4;
  }        
}

void DisplayRate(unsigned int rate)
{
  unsigned char h,t,m;
  h = rate/100;
  t = rate%100/10;
  m = rate%10;
  LCDMEM[5] &=~ 0xe0;
  LCDMEM[5] |= (RateNumBuf[h]&0xe0);
  LCDMEM[6] &=~ 0xef;
  LCDMEM[6] |= (RateNumBuf[h]&0x0f)+(RateNumBuf[t]&0xe0);
  LCDMEM[7] &=~ 0xef;
  LCDMEM[7] |= (RateNumBuf[t]&0x0f)+(RateNumBuf[m]&0xe0);
  LCDMEM[8] &=~ 0x0f;
  LCDMEM[8] |= (RateNumBuf[m]&0x0f);
}
#if  1
void DisplayBedNum(unsigned int num)
{
  unsigned char h,t,m; 
  if(num >=200)
  {
    num = 199;
  }
  h = num/100;
  t = num%100/10;
  m = num%10;
  if(num != 0xff)  //正常值
  {
    if(h == 1)
    {
      LCDMEM[8] |= BIT7;
    }
    else
    {
      LCDMEM[8] &=~ BIT7;
    }
    LCDMEM[8] &=~ 0x70;
    LCDMEM[8] |= ((BedNumBuf[t]&07)<<4);
    LCDMEM[9] &=~ 0x7f;
    LCDMEM[9] |= ((BedNumBuf[t]&0xF0)>>4)+((BedNumBuf[m]&0x07)<<4);
    LCDMEM[10] &=~ 0x0f;
    LCDMEM[10] |= ((BedNumBuf[m]&0xF0)>>4);
  }
  else   //不显示
  {
    LCDMEM[5] &=~ 0x0f;
    LCDMEM[4] &=~ 0xfe;
    LCDMEM[3] &=~ 0xf0;
    
  }
}
#endif

void DisplayUpAlarm(unsigned int alarmvalue)
{
  unsigned char h,t,m;
  h = alarmvalue/100;
  t = alarmvalue%100/10;
  m = alarmvalue%10;
  LCDMEM[4] |= BIT0;  //'上'  显示
  if(alarmvalue != 0xff)  //正常值
  {
    if(h == 1)
    {
      LCDMEM[5] |= BIT0;
    }
    else
    {
      LCDMEM[5] &=~ BIT0;
    }
    LCDMEM[5] &=~ 0x0e;
    LCDMEM[5] |= ((AlarmNumBuf[t]&0xe0)>>4);
    LCDMEM[4] &=~ 0xfe;
    LCDMEM[4] |= ((AlarmNumBuf[t]&0x0f)<<4)+((AlarmNumBuf[m]&0xe0)>>4);
    LCDMEM[3] &=~ 0xf0;
    LCDMEM[3] |= ((AlarmNumBuf[m]&0x0f)<<4);
  }
  else   //不显示
  {
    LCDMEM[5] &=~ 0x0f;
    LCDMEM[4] &=~ 0xfe;
    LCDMEM[3] &=~ 0xf0;
    
  }
}

void DisplayDownAlarm(unsigned int alarmvalue)
{
  unsigned char h,t,m;
  h = alarmvalue/100;
  t = alarmvalue%100/10;
  m = alarmvalue%10;
  LCDMEM[1] |= BIT0;
  if(alarmvalue != 0xff)  //正常值
  {
    if(h == 1)
    {
      LCDMEM[2] |= BIT0;
    }
    else
    {
      LCDMEM[2] &=~ BIT0;
    }
    LCDMEM[2] &=~ 0x0e;
    LCDMEM[2] |= ((AlarmNumBuf[t]&0xe0)>>4);
    LCDMEM[1] &=~ 0xfe;
    LCDMEM[1] |= ((AlarmNumBuf[t]&0x0f)<<4)+((AlarmNumBuf[m]&0xe0)>>4);
    LCDMEM[0] &=~ 0xf0;
    LCDMEM[0] |= ((AlarmNumBuf[m]&0x0f)<<4);
  }
  else
  {
    LCDMEM[2] &=~ 0x0f;
    LCDMEM[1] &=~ 0xfe;
    LCDMEM[0] &=~ 0xf0;
  }
}

void DisplayMoon(unsigned char status)
{
  if(status == ON)
  {
  LCDMEM[2] |= BIT6;
  }
  else
  {
  LCDMEM[2] &=~ BIT6;
  }
}

void DisplaySun(unsigned char status)
{
  if(status == ON)
  {
  LCDMEM[2] |= BIT5;
  }
  else
  {
  LCDMEM[2] &=~ BIT5;
  }
}

void DisplayRFRSSI(unsigned char level)
{
  LCDMEM[2] |= BIT5;
}



