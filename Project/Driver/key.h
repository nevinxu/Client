#ifndef _KEY_
#define _KEY_
#include "include.h"
#define KeyDIR  P1DIR
#define KeyIE   P1IE
#define KeyIES  P1IES
#define KeyIFG  P1IFG
#define KeyIN   P1IN

#define KeyvolumeUp  BIT6
#define KeyvolumeDown  BIT7
#define KeyUpperLimit  BIT5
#define KeyLowLimit  BIT4

#define CLOSE           0
#define UPPERSETMODE      1
#define LOWERSETMODE      2
#define VOLUMESETMODE   4

#define NONE    0


#define VOICELEVELMID   4

extern  void KeyInit();
extern  void KeyFunction();
#endif