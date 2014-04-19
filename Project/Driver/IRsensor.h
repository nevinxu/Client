#ifndef _IRSENSOR_
#define _IRSENSOR_
#include "include.h"

#define IR_CTL  BIT0
#define IR_CTL_DIR  P5DIR
#define IR_CTL_OUT  P5OUT
#define IR_CTL_SEL  P5SEL


#define IR_RE  BIT3
#define IR_RE_DIR  P1DIR
#define IR_RE_OUT  P1OUT
#define IR_RE_IN   P1IN
#define IR_RE_SEL  P1SEL
#define IR_RE_IE    P1IE
#define IR_RE_IFG    P1IFG
#define IR_RE_IES    P1IES


#define Open    1
#define Close   0


extern void IRSensorPortInit(void);
extern unsigned char IRReceiveStatus();
extern void IRRecCaptureInit();
extern unsigned int GetRate();
extern void TA0Init();




#endif