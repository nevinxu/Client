#ifndef _IDS2100_
#define _IDS2100_

#include <msp430.h>
#include "include.h"

#define SCLK      BIT5//时钟线
#define MOSI    BIT2//数据线
#define SSB      BIT4//片选线
#define SOMI    BIT0//数据线
#define RDY    BIT3//数据线

#define SOMIIN  P6IN
#define RDYIN  P7IN

#define ISD2100_MOSIDIR P7DIR
#define ISD2100_SCLKDIR P7DIR
#define ISD2100_SSBDIR  P7DIR
#define ISD2100_SOMIDIR P6DIR
#define ISD2100_RDYDIR  P7DIR

#define MOSI_HI   P7OUT|=MOSI;
#define MOSI_LO   P7OUT&=~MOSI;

#define SCLK_HI   P7OUT|=SCLK;
#define SCLK_LO   P7OUT&=~SCLK;

#define SSB_HI   P7OUT|=SSB;
#define SSB_LO   P7OUT&=~SSB;

extern void ISD2100Init(void);
extern void VoicePlay(unsigned int num);
extern unsigned char ReadVoice();
extern void WriteVoice(unsigned char value);
extern void WriteVoiceLevel(unsigned char level);
extern void Play_Drop_Start();
extern void Play_Drop_Stop();
extern void Play_OverQuick();
extern void Play_TooSlow();
extern void VoiceUp();
extern void VoiceDown();
extern void VoiceFunction();

#endif