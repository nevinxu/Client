#include "CC1101.h"
#include "HT1621.h"
#include "IDS2100.h"
#include "IRsensor.h"
#include "Uart.h"
#include "key.h"
#include "DigtalClock.h"
#include "DataSendRec.h"
#include "HAL_FLASH.h"

#define  TestMode  1                    
//#define         HiXT1   //高频晶振
#define         LoXT1   //低频晶振


#define VDD_IO_EN_DIR   P7DIR
#define VDD_IO_EN_OUT   P7OUT
#define VDD_IO_EN_PIN       BIT6

#define VDD_IO_EN_SET      VDD_IO_EN_DIR |=  VDD_IO_EN_PIN  
#define VDD_IO_ENABLE      VDD_IO_EN_OUT &=~  VDD_IO_EN_PIN 
#define VDD_IO_DISABLE      VDD_IO_EN_OUT |=  VDD_IO_EN_PIN 

#define MCLK_C      (8)//定义主时钟频率，供延时使用
#define Delay_us(n) __delay_cycles(n*MCLK_C)
#define Delay_ms(n) __delay_cycles(n*(1000L)*MCLK_C)
