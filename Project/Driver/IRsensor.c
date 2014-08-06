#include <msp430.h>
#include <math.h>
#include "IRsensor.h"

extern unsigned int TimerValue;
extern unsigned int RateDisplayFlag;
extern unsigned int time_num;
unsigned int OverflowTime;
extern unsigned int Rate ;
extern unsigned int RefreshTime;
extern unsigned char   DisplayDigtalClockFlag;
extern unsigned int DisplayMode;
extern unsigned char DisplayModeChargetimes;
extern unsigned char DisplayModeChargeFlag;
extern unsigned char Battery_Display_Flag;
extern unsigned char Display_Blink_Flag;   //����ֵ��˸��־
extern unsigned Key_Set_Mode;  //��������ģʽ
extern unsigned char KeyPressFlag;   //������־
extern unsigned int keyPressLongNum;  //������ʱ����
extern unsigned char keyPressLongFlag;  //������ʾ

extern unsigned int TotalDrop;  //�ܵ�Һ

unsigned int Alarm_timer_Num = 0;  //�������ʱ�����
unsigned char Display_Blink_TimeOut;  //����ֵ���ó�ʱ

unsigned char Display_All_TimeOut;  //��ʾ��ʱ


extern unsigned char RFDataRecFlag;
unsigned char Rf_Interval_Num = 0;  //RF�򿪼��ʱ��
unsigned char Rf_Interval_Flag = 0;  //RF�򿪼�ϱ�־

extern unsigned char AlarmUpperValue;  //��������ֵ
extern unsigned char AlarmLowerValue; //��������ֵ

unsigned char TestModeChargeRatetimes;   //����ģʽ�µ��ٶȸ��±�־
extern unsigned int VoiceDelayNum;
unsigned int OverflowTime_Time0;        //for CC1101 ��ʱ by cai
unsigned char Calflag;                  //by cai ʱ��У׼��־

void IRSensorPortInit(void)
{
  IR_CTL_DIR |= IR_CTL;
  IR_CTL_OUT |= IR_CTL;
  
  IR_RE_DIR &=~ IR_RE;
}

unsigned char IRReceiveStatus()
{
  unsigned char ret;  
  if(IR_RE_IN &  IR_RE)
  {
    ret = Open;
  }
  else
    ret = Close;
  
  return ret;
}

void IRRecCaptureInit()
{
  IR_RE_SEL |= IR_RE;
  TA1CTL = TACLR;
  TA1CTL = TASSEL_1 + MC_2 + TAIE;  //ACLK  Continuous mode
  TA1CCTL0 = CM_1+CCIS_1+SCS +CAP+CCIE;    // 
}

void IRRecCaptureDisable()
{
    TA1CTL = 0;
    TA1CCTL0 = 0;
}
void TA0Init()
{
  TA0CTL = TACLR;
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCTL1 = CCIE;                             // CCR1 interrupt enabled
  TA0CCR0 = 328;                                 //��ʱʱ��Ϊ10ms   32768/100
  TA0CCR1 = 0x8000;                               //��ʱ��ʱ��Ϊ1000ms
  TA0CTL = TASSEL_1 + MC_2+ TAIE;     //Continuous mode
}

void  IRSensorInit()
{
  IRSensorPortInit();  //�����ʼ��
  IRRecCaptureInit();
}

  
unsigned int GetRate()   //��β���ȡƽ��
{
  static unsigned int buffer[8];
  static unsigned char flag;
  unsigned int retdata = 0;
  static signed char i;
  double data = 0;
  double Valuefabs;
  if(RateDisplayFlag == 1)                  //����Ҫ���ж�  ���������
  {
    data = 32768*OverflowTime;
    data += TimerValue;
  }
  else
  {
    data = 0;
  }
  if(data)  
  {
      data = 60*32768 / data;
  }
  else
  {
    data =0;
  }
  OverflowTime = 0;
  TimerValue = 0;
  if((buffer[7] == 0) &&(buffer[0] == 0))  //��һ��
  {
    for(i=0;i<8;i++)
    {
      buffer[i] = (unsigned int)data;
    }
//    if(data >= 40)         //ûʲôҪ����40?
//    {
//      if(RateDisplayFlag)  //����Ҫ���ж�
//      {
//        TotalDrop = 0;//���¼���
//       // Play_Drop_Start();
//        Alarm_timer_Num = 20;  //��ʼ��Ϊ����ʱ���������
//      }
//    }
  }
  else  //��������
  {
    for(i=7;i>0;i--)
    {
      buffer[i] = buffer[i-1]; 
    }
    buffer[0] = (unsigned int)data;
  }
    Valuefabs = fabs((double)buffer[1] - (double)buffer[0]);
    if(Valuefabs> 20 )  //���λ�ȡ�����ٶȲ��̫��    ֱ���滻�µ�
    {
      for(i=0;i<8;i++)
      {
        buffer[i] = (unsigned int)data;
      }
    }
  
  for(i=0;i<8;i++)
  {
    retdata+=buffer[i];
  }
  retdata>>=3;
  if(RateDisplayFlag)  //����Ҫ���ж�
  {
    if(Alarm_timer_Num >= 10)  //10sһ��
    {
      if(retdata >= AlarmUpperValue)
      {
        flag++;
        if(flag>=5)
        {
            Play_OverQuick();
        }
        Alarm_timer_Num = 0;
      }
      else if(retdata <= AlarmLowerValue)
      {
        flag++;
        if(flag>=5)
        {
            Play_TooSlow();
        }
        
        Alarm_timer_Num = 0;
      }
      else
      {
        flag = 0;
      }
    }
  }
//  if((buffer[7] >= 15) &&(retdata <=15))  //�ٶȱ���  ֱ��ֹͣ
//  {
//   // Play_Drop_Stop();
//  }


  return retdata;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{ 
  
  
  static unsigned int IRTimerValue[8] = {0,0,0,0,0,0,0,0};
  static unsigned int OverflowValue[8] = {0,0,0,0,0,0,0,0};
  static signed char i =0;
  static double Valuefabs;
  TA1CCTL0 &=~ COV;
  Display_All_TimeOut = 0;   //����ɼ��ж�, �����ʾ��ʱ����
  if((IRTimerValue[0] == 0)&&(IRTimerValue[7] == 0))
  {
    for(i=7;i>=0;i--)
    {
      IRTimerValue[i] = TA1CCR0;
      OverflowValue[i] = OverflowTime;
    }
  }
  else
  {
    for(i=7;i>0;i--)
    {
      IRTimerValue[i] = IRTimerValue[i-1];
      OverflowValue[i] = OverflowValue[i-1];
    }
    IRTimerValue[0] = TA1CCR0;
    OverflowValue[0] = OverflowTime;
  }
  if(OverflowValue[0]>3)   //���ʱ��̫��  6s
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = 0;
    OverflowValue[i] = 0;
    }
    OverflowTime = 0;
    goto step2;
  }
  if((OverflowValue[0]==0)  && (IRTimerValue[0]<=3280) )    //   100ms 328 = 10ms  ��ε��������ϴεļ��ʱ��̫��,����  
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = 0;
    OverflowValue[i] = 0;
    }
    goto step2;
  }
  Valuefabs = fabs((double)IRTimerValue[1] - (double)IRTimerValue[0]);
  if(Valuefabs> 20000 )  //���λ�ȡ�����ٶȲ��̫�� ��ˢ������
  {
    for(i=7;i>=0;i--)
    {
    IRTimerValue[i] = TA1CCR0;
    OverflowValue[i] = OverflowTime;
    }
    goto step2;
  }
        TimerValue = IRTimerValue[0];
        OverflowTime = OverflowValue[0];
        RateDisplayFlag =1;    //�����жϽ���    ��ʾ��־
        TotalDrop++;   //�ܵ��ټ�һ
step2:  

  IRRecCaptureInit();
  LPM2_EXIT;
  
 
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
  TA1CCTL3 &=~ COV;
  switch( TA1IV )
  {
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case  6: 
    case  8:  break;                        // CCR2 not used
    case 10:  
      {
        if(!RateDisplayFlag)
        {
         OverflowTime++;   //û���ж� ���뽫ÿ�ε����ֵ�ۼ�
        }
        break;
      }
  }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_0 (void)
{

  static unsigned int i;
  TA0CCR0 += 328;  
  i++;

/**********************************************************************************/

/*********************************����RF���ض�ʱ��*****************************/         
        if(RFDataRecFlag == 0)
        {
          Rf_Interval_Num++;
          if(Rf_Interval_Num >= 50)     //��ʾ�ź���ʱʱ��
          {
          	Rf_Interval_Num = 0;
            DisplayRFRSSI(0);
            RFDataRecFlag = 2;
          }
 //         if(Rf_Interval_Num >= RFRECEIVEINTERVAL)  //8��
 //         {
 //           Rf_Interval_Num = 0;
  //          Rf_Interval_Flag = 1;
  //          DisplayRFRSSI(0);
 //         }
        }
  //      else if(RFDataRecFlag == 1)
   //     {
  //        Rf_Interval_Num = 0;
  //      }


/************************��������ʱ������**************************************/ 
  if(keyPressLongFlag != NONE)
  {
    keyPressLongNum++;
    if(keyPressLongNum>=40)   //400������ʱ
    {
      if(keyPressLongNum%10 == 0) // 100����һ�μ�
      {
        Display_Blink_Flag = 1;  //��ʾ
        if(keyPressLongFlag == KeyvolumeUp)
        {
          KeyPressFlag = KeyvolumeUp;       
        }
        else if(keyPressLongFlag == KeyvolumeDown)
        {
          KeyPressFlag = KeyvolumeDown;
        }
      }
    }
  }
/*******************************************************************************/

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A0_1 (void)
{

  static unsigned int i;
    switch( TA0IV )
  {
    case  2:  
      {
        TA0CCR1 += 0x4000;      //500ms
        i++;
		
        if(i%1000)  //500s    �����ʾ����
        {
          Battery_Display_Flag = 1;
        }
/************************����ģʽ�µ��ٶȸ���**********************************/       
        if((DisplayMode & DisplayTestMode) == DisplayTestMode)
        {
          TestModeChargeRatetimes++;
          if(TestModeChargeRatetimes >= TESTMODERATEREFRESHTIME) //  //2s����һ���ٶ�
          {
            TestModeChargeRatetimes = 0;
            RateDisplayFlag = 1;
          }
        }
/**********************************************************************************/
	  if(!RateDisplayFlag)
	  {
	      RefreshTime++;
	      if((RefreshTime%CLEARDATATIMEOUT)==0)  //8���Ӹ���һ��
	      {
	          RefreshTime = 1;
	          RateDisplayFlag = 2;  //����ģʽ  �ٶ�Ϊ0
	      }
	   } 
	  else
	  {
	      RefreshTime = 1;
	  }
	  if(VoiceDelayNum>0)    //10msһ���ж�
	  {
	    VoiceDelayNum--;
	  }

/*******************************Һ����ʾ��ʱ����***************************************/        

	Display_All_TimeOut++;
	if(Display_All_TimeOut >=60000)
	{
		Display_All_TimeOut = 60000;
	}


/********************************����ʱ��������*******************************/
        Alarm_timer_Num++;  //   500msһ��
        if(Alarm_timer_Num >= 60000)   //�������Զ�����
        {
          Alarm_timer_Num = 60000;
        }
/*******************************************************************************/        
        
/*********************************������˸��ʱ��*********************************/
        if(Key_Set_Mode != CLOSE)  //����ģʽ��  ��ʾ��Ҫ��˸
        {
          if(i%2)    //500ms  ��˸
          {
            Display_Blink_Flag = 1;  //����ģʽ����ʾ����
          }
          else
          {
            Display_Blink_Flag = 2;     //����ģʽ�²���ʾ����
          }
          Display_Blink_TimeOut++;//����ֵ��˸״̬��ʱ
        }
        else
        {
          Display_Blink_Flag = 0;   //������ģʽ��  ����˸״̬
        }
/*******************************************************************************/        
        
        

/*******************************************************************************/ 
        
        
        break;                        // CCR1 not used
      }
    case  4:  break;                        // CCR2 not used
    case  6:  break;   
    case  8:  break;                       // CCR2 not used
    case 10:  
  {
      break;  
     
     } 
  }

}
