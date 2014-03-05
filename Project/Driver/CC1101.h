#ifndef _CC1101_
#define _CC1101_
#include "TI_CC_CC1100-CC2500.h"
#include "TI_CC_spi.h"
#include "CC1100-CC2500.h"

#define  PACKET_LEN         (0x02)
#define  RSSI_IDX           (PACKET_LEN+1)  // Index of appended RSSI 
#define  CRC_LQI_IDX        (PACKET_LEN+2)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit 
#define  PATABLE_VAL        (0x51)          // 0 dBm output 

extern void InitRadio(void);
extern void Transmit(char *buffer, unsigned char length);
extern void ReceiveOn(void);
extern void ReceiveOff(void);
extern void WorOn(void);
extern void TransmitRate(unsigned int Rate);
void ReceiveData(char length);
#endif