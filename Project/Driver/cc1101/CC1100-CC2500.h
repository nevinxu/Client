#ifndef _CC1100CC2540_
#define _CC1100CC2540_
//----------------------------------------------------------------------------
//  Description:  CC1100-CC2500.c function declarations
//
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//----------------------------------------------------------------------------


extern void writeRFSettings(void);
extern void RFSendPacket(char *, char);
extern char RFReceivePacket(char *);
#endif