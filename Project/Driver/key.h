#ifndef _KEY_
#define _KEY_
#include "include.h"
#define KeyDIR  P1DIR
#define KeyIE   P1IE
#define KeyIES  P1IES
#define KeyIFG  P1IFG
#define KeyIN   P1IN
#define KeyvolumeUp  BIT4
#define KeyvolumeDown  BIT6
#define KeyMaxRate  BIT5
#define KeyMinRate  BIT3


extern  void InitKey();

#endif