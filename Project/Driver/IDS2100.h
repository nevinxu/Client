#ifndef _IDS2100_
#define _IDS2100_

#include <msp430.h>
#include "include.h"

#define SCLK      BIT0//时钟线
#define MOSI    BIT1//数据线
#define SSB      BIT7//片选线
#define SOMI    BIT6//数据线
#define RDY    BIT2//数据线

#define SOMIIN  P7IN
#define RDYIN  P5IN

#define ISD2100_MOSIDIR P5DIR
#define ISD2100_SCLKDIR P5DIR
#define ISD2100_SSBDIR P1DIR
#define ISD2100_SOMIDIR P7DIR
#define ISD2100_RDYDIR P5DIR

#define MOSI_HI   P5OUT|=MOSI;
#define MOSI_LO   P5OUT&=~MOSI;

#define SCLK_HI   P5OUT|=SCLK;
#define SCLK_LO   P5OUT&=~SCLK;

#define SSB_HI   P1OUT|=SSB;
#define SSB_LO   P1OUT&=~SSB;

extern void InitISD2100(void);
extern void VoicePlay(unsigned int num);
extern unsigned char ReadVoice();
extern void WriteVoice(unsigned char value);
extern void VoiceUp();
extern void VoiceDown();

#endif