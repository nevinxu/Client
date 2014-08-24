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
//#define         HiXT1   //高频晶振
#define         LoXT1   //低频晶振

#define VOICE  1


#define     TESTMODERATEREFRESHTIME        4   //  测试模式下的速度更新时间  2s
#define     RFRECEIVEINTERVAL              16   //cc1101  打开间断时间    8s
#define     RFCHANNAL                      0x01   //cc1101  频率设置

#define     RFSYNC1                      0Xd3
//#define     RFSYNC0                      0x09
//#define     RFSYNC0                      0x24
//#define     RFSYNC0                      0x36
//#define     RFSYNC0                      0x2d
//#define     RFSYNC0                      0x12
#define     RFSYNC0                      0x1b


#define SLEEPTIMEOUT                   2*600     //10分钟关屏
//#define CLEARDATATIMEOUT		2*7+1       // 7.5秒未接收数据    滴速归零
#define CLEARDATATIMEOUT		15


#define VDD_IO_EN_DIR   P7DIR
#define VDD_IO_EN_OUT   P7OUT
#define VDD_IO_EN_PIN       BIT6

#define VDD_IO_EN_SET      VDD_IO_EN_DIR |=  VDD_IO_EN_PIN  
#define VDD_IO_ENABLE      VDD_IO_EN_OUT &=~  VDD_IO_EN_PIN 
#define VDD_IO_DISABLE      VDD_IO_EN_OUT |=  VDD_IO_EN_PIN 




#define MCLK_C      (8)//定义主时钟频率，供延时使用
#define Delay_us(n) __delay_cycles(n*MCLK_C)
#define Delay_ms(n) __delay_cycles(n*(1000L)*MCLK_C)
