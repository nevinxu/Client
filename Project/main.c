#include <msp430.h>
#include "include.h"

unsigned int    TimerValue;
unsigned int    RateDisplayFlag;
unsigned int    time_num = 999;   //������ʾ0
unsigned int    Rate =0 ;
unsigned int    RefreshTime;

unsigned char BatteyLevel;
unsigned char Battery_Display_Flag;
unsigned char   DisplayDigtalClockFlag;

unsigned char KeyPressFlag;   //������־

unsigned int DisplayMode = DisplayClockMode;
unsigned char DisplayModeChargetimes=0;
unsigned char DisplayModeChargeFlag=0;

unsigned char CurrentSpeed =70;
unsigned int TotalDrip =0;
unsigned char TerminalPowerPrecent =100;

extern unsigned char DataRecFlag;
extern unsigned char Rf_Interval_Flag;  //RF�򿪼�ϱ�־

extern unsigned char Display_Blink_Flag;   //����ֵ��˸��־

extern unsigned char Display_Blink_TimeOut;  //����ֵ���ó�ʱ
extern unsigned Key_Set_Mode;  //��������ģʽ

unsigned char WorkingStatus = 0 ;

void ClockInit()
{
  volatile unsigned int i;                  // Use volatile to prevent removal
#ifdef HiXT1
  FLL_CTL0 = XTS_FLL ;                     // XT1  ��Ƶģʽ Configure load caps
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
/*  
    FLL_CTL0 |= XCAP11PF;                     // Configure load caps
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
                                            // stabilize
  do
  {
    IFG1 &= ~OFIFG;                         // Clear osc fault flag
    for (i = 0; i < 1000; i++);             // Delay for osc to stabilize
  } while(IFG1 & OFIFG);                    // Check to see if osc flag is set
  */
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

int main(void)
{
  unsigned char buf[] = {0x55,0x99,0x66};
  unsigned int addr = 0xf800;
  unsigned int a;
  
  unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  ClockInit();
  
  VddIOEnable();  //�ⲿģ�鹩��ʹ��
  
  LEDInit(); 
  LCDInit();
  KeyInit();
 // UartInit();
  RadioInit();  //cc1101 ��ʼ��
  IRSensorInit();
 // DigtalClockInit(); //����ʱ�ӳ�ʼ��
  
  TA0Init();   //��ʱ����ʼ��
  
  LEDOn(YELLOWLED);   //��ʾ��־
  Delay_ms(500);
  LEDOff(YELLOWLED);
  
  LoginTransmit();   //��½���� 
  
  Flash_SegmentErase(addr);
  FlashWrite_8(buf,addr,3);
  GetBatteyLevel();
  
  ISD2100Init();
#if 0
  WriteVoice(0x00);
    a = ReadVoice();
    a++;
    VoicePlay(14);
    Delay_ms(500);
  WriteVoice(0x3f);
    a = ReadVoice();
        a++;
    VoicePlay(14);
    Delay_ms(500);
  WriteVoice(0x7f);
    a = ReadVoice();
        a++;
    VoicePlay(14);
    Delay_ms(500);
  a = ReadVoice();
      a++;
  unsigned char j;
  for(unsigned char i =0; i<10;i++)
  {
    for(unsigned char j=12;j<25;j++)
    {
    VoicePlay(j);
    Delay_ms(500);
    }
 
  }
#endif
  
  __bis_SR_register(GIE);   //ȫ���ж�ʹ��
  while(1)
  {
    if(DisplayMode == DisplayRateMode)
    {
      if(RateDisplayFlag)
      {
        Rate = GetRate();
        RateDisplayFlag =0;
        DisplayRate(Rate); 
        DisplayTotalDrop();
        RefreshTime=1;
      }
    }
#if 0
 //   else if(DisplayMode == DisplayClockMode)
    {
      if(DisplayDigtalClockFlag)
      {
        DisplayDigtalClockFlag = 0;
        DigtalClockRun();
        DisplayDigtalClock();
    //    ReceiveOn();
        TotalDrip++;  //����  Һ���Զ���һ
//        WorkingStateMsgTransmit();

      }
    }
#endif
    
    if(DataRecFlag == 1)  //���ݽ��ձ�־
    {
      LEDOn(BLUELED);
      ReceiveData();
      RecDataCheck();   //�������ݼ��
      DataRecFlag = 0;
    }
    if(Rf_Interval_Flag == 1)  //RF�򿪼�ϱ�־
    {
      Rf_Interval_Flag = 0;
      ReceiveOn();
    }
    if(Battery_Display_Flag == 1)  //�����ʾ
    {
      Battery_Display_Flag = 0;
      BatteyLevel = GetBatteyLevel();
      DisplayBattery( BatteyLevel);
    }
    if(KeyPressFlag)   //��������
    {
      KeyFunction();
      KeyPressFlag = 0;
      
    }
    if(Display_Blink_Flag)
    {
      if(Display_Blink_TimeOut >= 20)  //10s��ʱ
      {
        Display_Blink_TimeOut =0;
        Key_Set_Mode &=~ (UPPERSETMODE+LOWERSETMODE);
      }
      RefreshAlarmValue();
      Display_Blink_Flag = 0;  //�ж��Ժ������
    }
    
  }
}





