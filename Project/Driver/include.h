#include "CC1101.h"
#include "HT1621.h"
#include "IDS2100.h"
#include "IRsensor.h"
#include "Uart.h"
#include "key.h"
#include "DigtalClock.h"
#include "DataSendRec.h"

#define  TestMode  1                    
//#define         HiXT1   //��Ƶ����
#define         LoXT1   //��Ƶ����


#define MCLK_C      (8)//������ʱ��Ƶ�ʣ�����ʱʹ��
#define Delay_us(n) __delay_cycles(n*MCLK_C)
#define Delay_ms(n) __delay_cycles(n*(1000L)*MCLK_C)
