#ifndef _BATTERY_
#define _BATTERY_

#define BATTERYLEVEL3    484      //47*3.7/146*2.5 = y/1023
#define BATTERYLEVEL2    462
#define BATTERYLEVEL1    440
#define BATTERYLEVEL0    418

#define CHGPINDIR        P1DIR
#define CHGPININ        P1DIR
#define CHGPIN          BIT2

#define CHGSET          CHGPINDIR &=~ CHGPIN
#define CHGSTATUS       CHGPININ & CHGPIN

extern unsigned char  GetBatteyLevel(void);
extern unsigned int  GetBatteyValue();

#endif