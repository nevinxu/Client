#include <msp430.h>
#include <stdlib.h>  //����� ����   ͷ�ļ�
#include "include.h"

unsigned int    TimerValue;
unsigned int    RateDisplayFlag;
unsigned int    time_num = 999;   //������ʾ0
unsigned int    RefreshTime;

unsigned char BatteyLevel;
unsigned char Battery_Display_Flag;
unsigned char   DisplayDigtalClockFlag;

unsigned char KeyPressFlag;   //������־

unsigned int DisplayMode = 0;
unsigned char DisplayModeChargetimes=0;
unsigned char DisplayModeChargeFlag=0;

unsigned char CurrentSpeed =0;
extern unsigned int TotalDrop;
unsigned char TerminalPowerPrecent =100;
extern unsigned char Display_All_TimeOut;  //��ʾ��ʱ

extern unsigned char RFDataRecFlag;
extern unsigned char Rf_Interval_Flag;  //RF�򿪼�ϱ�־

extern unsigned char Display_Blink_Flag;   //����ֵ��˸��־

extern unsigned char Display_Blink_TimeOut;  //����ֵ���ó�ʱ
extern unsigned Key_Set_Mode;  //��������ģʽ

extern unsigned int ModelAddress;  //��λ��

unsigned char LogInFlag = 0;  //��½��־

unsigned char WorkingStatus = 0 ;

extern signed char RSSIValue;

extern unsigned char UartStatusFlag;
extern unsigned char Calflag;                  //by cai ʱ��У׼��־




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


void DriverInit()
{
  ClockInit();
  
  VddIOEnable();  //�ⲿģ�鹩��ʹ��
  
  ReadAlarmValue4Flash();

  LEDInit(); 
  LCDInit();
  KeyInit();
//UartInit();

  RadioInit();  //cc1101 ��ʼ��
  IRSensorInit();
  
  
  TA0Init();   //��ʱ����ʼ��
  
  LEDOn(YELLOWLED);
  LEDOn(BLUELED); 
  Delay_ms(500);
  LEDOff(BLUELED);
  LEDOff(YELLOWLED);
  
  GetBatteyLevel();
  
#ifdef   VOICE
//  ISD2100Init();
#endif
  
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  srand(1);
  DriverInit();
  DisplayMode = DisplayInitMode + DisplayRateStartMode;
  __bis_SR_register(GIE);   //ȫ���ж�ʹ��


  while(1)
  {
/***************************��ʾ�ٶ�ģʽ***************************************/
      if(RateDisplayFlag  == 1)  //���ݸ��±�־
      {
        if((DisplayMode & DisplayTestMode) == DisplayTestMode)
        {
            CurrentSpeed = rand()%100 +40;
            TotalDrop++;
        }
        else if((DisplayMode & DisplayRateMode) == DisplayRateMode)
        {
          CurrentSpeed = GetRate();
        }
        RateDisplayFlag =0;
        DisplayRate(CurrentSpeed); 
        RefreshTime=1;
      }
      else if(RateDisplayFlag  == 2)
      {
        RateDisplayFlag = 3;
        CurrentSpeed = 0;
        DisplayRate(CurrentSpeed); 
      }
      if(RFDataRecFlag == 1)  //���ݽ��ձ�־
      {
        ReceiveData();    //RF���ݽ���   
        RecDataCheck();   //�������ݼ��
        RFDataRecFlag = 0;
        DisplayRFRSSILevel(RSSIValue);  //
        RSSIValue = -120;
      }
      
/***************************ģʽ***************************************/      
      if(Battery_Display_Flag == 1)  //�����ʾ
      {
        Battery_Display_Flag = 0;
        BatteyLevel = GetBatteyLevel();
        DisplayBattery( BatteyLevel);
      }
      if(KeyPressFlag)   //��������
      {
        if((CurrentSpeed == 0) &&(TotalDrop!=0))//��������ʱ ��ǰ�ٶ�Ϊ����ܵ��ٲ�Ϊ��   �ܵ�������
        {
            IRRecCaptureInit();
            TotalDrop = 0;
        }
        else
        {
        KeyFunction();
//}
       // ReceiveOn();
        KeyPressFlag = 0;

       // LogoutAckTransmit();
        }
      }
      if(Display_Blink_Flag)    //����ģʽ�µ�������˸ģʽ
      {
        if(Display_Blink_TimeOut >= 20)  //10s��ʱ
        {
          Display_Blink_TimeOut =0;
          Key_Set_Mode &=~ (UPPERSETMODE+LOWERSETMODE);
        }
        RefreshAlarmValue();
        Display_Blink_Flag = 0;  //�ж��Ժ������
      }
      if((DisplayMode & DisplayTestStartMode) == DisplayTestStartMode)      //��ʼģ�����ģʽ״̬
      {
        ModelAddress = 188;
        LCDInit();
        DisplayMode &=~ DisplayTestStartMode;
        DisplayMode &=~ DisplayRateMode;
        DisplayMode |= DisplayTestMode;
      }
      if((DisplayMode & DisplayRateStartMode) == DisplayRateStartMode)    //��ʼ��������ģʽ״̬
      {
        ModelAddress = RFCHANNAL;
        LCDInit();
        DisplayMode &=~ DisplayRateStartMode;
        DisplayMode &=~ DisplayTestMode;
        DisplayMode |= DisplayRateMode;
      } 


/*********************************������������************************************/	
      VoiceFunction();



	
/******************************************************************************/
    
/*******************************˯��ģʽ**************************************/
    if(DisplayMode == DisplaySleepMode)
    {
      __bis_SR_register(GIE+LPM3);
      DisplayMode = DisplayRateMode;
      LCDInit();
    }
    if(Display_All_TimeOut >= SLEEPTIMEOUT)  //
    {
      Display_All_TimeOut = 0;
      DisplayNone();
      DisplayMode = DisplaySleepMode;
    }
/*******************************���ڴ�����***************************************/
  if(UartStatusFlag == 1)
  {
    UartStatusFlag = 0;
    UartCommandstatus();
  }
    
 
  }
}





