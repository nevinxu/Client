#ifndef _HT1621_
#define _HT1621_

#include <msp430.h>


#define WR      BIT5//时钟线
#define DATA    BIT3//数据线
#define CS      BIT6//片选线
#define BL_YELLOW  BIT2
#define BL_BLUE    BIT4

#define WR_HI   P5OUT|=WR;
#define WR_LO   P5OUT&=~WR;

#define DATA_HI   P5OUT|=DATA;
#define DATA_LO   P5OUT&=~DATA;

#define CS_HI   P5OUT|=CS;
#define CS_LO   P5OUT&=~CS;

#define BL_YELLOW_OFF  P3OUT|= BL_YELLOW;
#define BL_BLUE_OFF  P5OUT|= BL_BLUE;
#define BL_YELLOW_ON  P3OUT&=~ BL_YELLOW;
#define BL_BLUE_ON  P5OUT&=~ BL_BLUE;


#define HT1621_INIT  P5DIR|=(WR+DATA+CS);
#define HT1621_BL_YELLOW_INIT P3DIR |= BL_YELLOW;
#define HT1621_BL_BLUE_INIT P5DIR |= BL_BLUE;

#define BIAS   0x52 //0b  1000 0101 0010  1/3duty 4com 

#define SYSDIS 0X00     //0b1000 0000 0000  1????μí3μ′?÷oíLCD???1·￠éú?÷ 
#define SYSEN  0X02     //0b1000 0000 0010 ′ò?a?μí3??μ′?÷ 

#define LCD_OFF 0X04     //0b1000 0000 0100  1?LCD???1 
#define LCD_ON  0X06     //0b1000 0000 0110  ′ò?aLCD???1 

#define XTAL   0x28     //0b1000 0010 1000 ía2??óê±?ó 
#define RC256  0X30     //0b1000 0011 0000  ?ú2?ê±?ó 

#define TONEON  0X12    //0b1000 0001 0010  ′ò?aéùò?ê?3? 
#define TONEOFF 0X10    //0b1000 0001 0000 1?±?éùò?ê?3? 

#define WDTDIS  0X0A    //0b1000 0000 1010  ???1?′??1· 
#define data_length 8

#define UPLINE         BIT0 
#define DOWNLINE        BIT1
#define GIGTALCLOCKFLAG        BIT3
#define RATEFLAG        BIT4
//#define DOWNLINE        BIT1
//#define DOWNLINE        BIT1

#define   DisplayRateMode               BIT0
#define   DisplayClockMode              BIT1 


extern void Ht1621_Init(void);
extern void Ht1621WrHalfOneData(unsigned char Addr,unsigned char Data);
extern void Ht1621WrOneData(unsigned char Addr,unsigned char Data) ;
extern void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt) ;
extern void Ht1621ClearDisplay();
extern void Ht1621WriteNum(unsigned char type,unsigned char addr,unsigned char num);
extern void Display_Digit(unsigned char type, unsigned char add ,signed int Data );
extern void DisplayRate(unsigned int rate);
#endif