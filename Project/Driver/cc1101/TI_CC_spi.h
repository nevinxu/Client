//----------------------------------------------------------------------------
//  Description:  Header file for TI_CC_spi.c
//
//  MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//----------------------------------------------------------------------------
#ifndef _TI_CC_SPI_
#define _TI_CC_SPI_

#include "msp430f4152.h"                      // MSP430 device being used.

#define TI_CC_SPI_USCIB0_PxSEL  P6SEL //for MSP430F4152
#define TI_CC_SPI_USCIB0_PxDIR  P6DIR
#define TI_CC_SPI_USCIB0_PxIN   P6IN
#define TI_CC_SPI_USCIB0_SIMO   BIT2  //P3.1
#define TI_CC_SPI_USCIB0_SOMI   BIT1  //P3.2
#define TI_CC_SPI_USCIB0_UCLK   BIT4  //P3.3

//msp430f4152
#define TI_CC_GDO0_PxOUT        P1OUT //GDO0 ½ÓP1.0
#define TI_CC_GDO0_PxIN         P1IN
#define TI_CC_GDO0_PxREN        P1REN
#define TI_CC_GDO0_PxSEL        P1SEL
#define TI_CC_GDO0_PxDIR        P1DIR
#define TI_CC_GDO0_PxIE         P1IE
#define TI_CC_GDO0_PxIES        P1IES
#define TI_CC_GDO0_PxIFG        P1IFG
#define TI_CC_GDO0_PIN          BIT0

#define TI_CC_GDO2_PxOUT        P1OUT //GDO2 ½ÓP1.5
#define TI_CC_GDO2_PxIN         P1IN
#define TI_CC_GDO2_REN          P1REN
#define TI_CC_GDO2_PxDIR        P1DIR
//#define TI_CC_GDO2_PIN          BIT1
#define TI_CC_GDO2_PIN          BIT2

//mspf4152  use
#define TI_CC_CSn_PxOUT         P6OUT //CSn ½ÓP3.6
#define TI_CC_CSn_PxDIR         P6DIR
#define TI_CC_CSn_PxREN         P6REN //pullup/ pulldown resistor
#define TI_CC_CSn_PIN           BIT3  

#define TI_CC_RF_SER_INTF      TI_CC_SER_INTF_USCIB0   // Interface to CCxxxx

extern void TI_CC_SPISetup(void);
extern void TI_CC_PowerupResetCCxxxx(void);
extern void TI_CC_SPIWriteReg(char, char);
extern void TI_CC_SPIWriteBurstReg(char, char*, char);
extern char TI_CC_SPIReadReg(char);
extern void TI_CC_SPIReadBurstReg(char, char *, char);
extern char TI_CC_SPIReadStatus(char);
extern void TI_CC_SPIStrobe(char);
extern void TI_CC_Wait(unsigned int);
extern void TI_CC_SPI_bitbang_out(char value);

#endif


