#ifndef _TI_CC_MSP430_
#define _TI_CC_MSP430_
//#include "msp430xG46x.h"                     // Adjust this according to the
//#include "io430x21x2.h"                      // MSP430 device being used.


// SPI port definitions                     // Adjust the values for the chosen
#define TI_CC_SPI_USART0_PxSEL  P3SEL       // interfaces, according to the pin
#define TI_CC_SPI_USART0_PxDIR  P3DIR       // assignments indicated in the
#define TI_CC_SPI_USART0_PxIN   P3IN        // chosen MSP430 device datasheet.
#define TI_CC_SPI_USART0_SIMO   0x04
#define TI_CC_SPI_USART0_SOMI   0x02
#define TI_CC_SPI_USART0_UCLK   0x08

#define TI_CC_SPI_USART1_PxSEL  P5SEL
#define TI_CC_SPI_USART1_PxDIR  P5DIR
#define TI_CC_SPI_USART1_PxIN   P5IN
#define TI_CC_SPI_USART1_SIMO   0x02
#define TI_CC_SPI_USART1_SOMI   0x04
#define TI_CC_SPI_USART1_UCLK   0x08

#define TI_CC_SPI_USCIA0_PxSEL  P3SEL
#define TI_CC_SPI_USCIA0_PxDIR  P3DIR
#define TI_CC_SPI_USCIA0_PxIN   P3IN
#define TI_CC_SPI_USCIA0_SIMO   0x10
#define TI_CC_SPI_USCIA0_SOMI   0x20
#define TI_CC_SPI_USCIA0_UCLK   0x01

#define TI_CC_SPI_USCIA1_PxSEL  P7SEL
#define TI_CC_SPI_USCIA1_PxDIR  P7DIR
#define TI_CC_SPI_USCIA1_PxIN   P7IN
#define TI_CC_SPI_USCIA1_SIMO   0x02
#define TI_CC_SPI_USCIA1_SOMI   0x04
#define TI_CC_SPI_USCIA1_UCLK   0x08



#define TI_CC_SPI_USCIB1_PxSEL  P3SEL
#define TI_CC_SPI_USCIB1_PxDIR  P3DIR
#define TI_CC_SPI_USCIB1_PxIN   P3IN
#define TI_CC_SPI_USCIB1_SIMO   0x02
#define TI_CC_SPI_USCIB1_SOMI   0x04
#define TI_CC_SPI_USCIB1_UCLK   0x08






#endif
