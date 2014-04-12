#ifndef _CC1101_
#define _CC1101_
#include "TI_CC_CC1100-CC2500.h"
#include "TI_CC_spi.h"
#include "CC1100-CC2500.h"


extern void RadioInit(void);
extern void Transmit(char *buffer, unsigned char length);
extern void ReceiveOn(void);
extern void ReceiveOff(void);
extern void WorOn(void);
extern void TransmitRate(unsigned int Rate);
extern void ReceiveData(void);
#endif