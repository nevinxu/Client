#ifndef _IRSENSOR_
#define _IRSENSOR_
#include "include.h"

#define IR_CTL  BIT3
#define IR_CTL_DIR  P2DIR
#define IR_CTL_OUT  P2OUT
#define IR_CTL_SEL  P2SEL


#define IR_RE  BIT1
#define IR_RE_DIR  P3DIR
#define IR_RE_OUT  P3OUT
#define IR_RE_IN   P3IN
#define IR_RE_SEL  P3SEL
#define IR_RE_IE    P3IE
#define IR_RE_IFG    P3IFG
#define IR_RE_IES    P3IES

#define LED1  BIT2
#define LED2  BIT1
#define LEDDIR P4DIR
#define LEDOUT P4OUT
#ifdef TestMode 
#define TestLed BIT0   //BIT3=TP26/    BIT1=TP24   BIT2 = TP25
#define TestLEDOUT P7OUT
#define TestLEDIR P7DIR
#endif



#define Open    1
#define Close   0


extern void IRSensorPortInit(void);
extern unsigned char IRReceiveStatus();
extern void LEDInit(void);
extern void  LEDOn(unsigned char num);
extern void  LEDOff(unsigned char num);
extern void IRRecCaptureInit();
extern unsigned int GetRate();
extern void TA0Init();




#endif