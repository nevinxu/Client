#include "CC1101.h"
#include "HT1621.h"
#include "IDS2100.h"
#include "IRsensor.h"
#include "Uart.h"
#include "key.h"
#include "DigtalClock.h"
#include "DataSendRec.h"

#define  TestMode  1                    
//#define         HiXT1   //高频晶振
#define         LoXT1   //低频晶振


#define MCLK_C      (8)//定义主时钟频率，供延时使用
#define Delay_us(n) __delay_cycles(n*MCLK_C)
#define Delay_ms(n) __delay_cycles(n*(1000L)*MCLK_C)
