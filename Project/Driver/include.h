#include "CC1101.h"
#include "IDS2100.h"
#include "IRsensor.h"
#include "Uart.h"
#include "key.h"
#include "DigtalClock.h"
#include "DataSendRec.h"
#include "HAL_FLASH.h"
#include "Display.h"
#include "Battery.h"

//#define  TestMode  0                    
//#define         HiXT1   //��Ƶ����
#define         LoXT1   //��Ƶ����

#define VOICE  1


#define     TESTMODERATEREFRESHTIME        4   //  ����ģʽ�µ��ٶȸ���ʱ��  2s
#define     RFRECEIVEINTERVAL              16   //cc1101  �򿪼��ʱ��    8s
//#define     RFCHANNAL                      0x03   //cc1101  Ƶ������
#define     RFCHANNAL                      0x02   //cc1101  Ƶ������


#define SLEEPTIMEOUT                   2*600     //10���ӹ���
//#define CLEARDATATIMEOUT		2*7+1       // 7.5��δ��������    ���ٹ���
#define CLEARDATATIMEOUT		15


#define VDD_IO_EN_DIR   P7DIR
#define VDD_IO_EN_OUT   P7OUT
#define VDD_IO_EN_PIN       BIT6

#define VDD_IO_EN_SET      VDD_IO_EN_DIR |=  VDD_IO_EN_PIN  
#define VDD_IO_ENABLE      VDD_IO_EN_OUT &=~  VDD_IO_EN_PIN 
#define VDD_IO_DISABLE      VDD_IO_EN_OUT |=  VDD_IO_EN_PIN 




#define MCLK_C      (8)//������ʱ��Ƶ�ʣ�����ʱʹ��
#define Delay_us(n) __delay_cycles(n*MCLK_C)
#define Delay_ms(n) __delay_cycles(n*(1000L)*MCLK_C)
