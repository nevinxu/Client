#include "TI_CC_spi.h"
#include "TI_CC_msp430.h"
#include "TI_CC_CC1100-CC2500.h"
#include  <stdio.h>   //使用Printf输出，必须包括这两个头文件
#include  <stdlib.h>  //使用Printf输出，必须包括这两个头文件
//----------------------------------------------------------------------------
//  void TI_CC_SPISetup(void)
//
//  DESCRIPTION:
//  Configures the assigned interface to function as a SPI port and
//  initializes it.
//----------------------------------------------------------------------------
//  void TI_CC_SPIWriteReg(char addr, char value)
//
//  DESCRIPTION:
//  Writes "value" to a single configuration register at address "addr".
//----------------------------------------------------------------------------
//  void TI_CC_SPIWriteBurstReg(char addr, char *buffer, char count)
//
//  DESCRIPTION:
//  Writes values to multiple configuration registers, the first register being
//  at address "addr".  First data byte is at "buffer", and both addr and
//  buffer are incremented sequentially (within the CCxxxx and MSP430,
//  respectively) until "count" writes have been performed.
//----------------------------------------------------------------------------
//  char TI_CC_SPIReadReg(char addr)
//
//  DESCRIPTION:
//  Reads a single configuration register at address "addr" and returns the
//  value read.
//----------------------------------------------------------------------------
//  void TI_CC_SPIReadBurstReg(char addr, char *buffer, char count)
//
//  DESCRIPTION:
//  Reads multiple configuration registers, the first register being at address
//  "addr".  Values read are deposited sequentially starting at address
//  "buffer", until "count" registers have been read.
//----------------------------------------------------------------------------
//  char TI_CC_SPIReadStatus(char addr)
//
//  DESCRIPTION:
//  Special read function for reading status registers.  Reads status register
//  at register "addr" and returns the value read.
//----------------------------------------------------------------------------
//  void TI_CC_SPIStrobe(char strobe)
//
//  DESCRIPTION:
//  Special write function for writing to command strobe registers.  Writes
//  to the strobe at address "addr".
//----------------------------------------------------------------------------


// Delay function. # of CPU cycles delayed is similar to "cycles". Specifically,
// it's ((cycles-15) % 6) + 15.  Not exact, but gives a sense of the real-time
// delay.  Also, if MCLK ~1MHz, "cycles" is similar to # of useconds delayed.
void TI_CC_Wait(unsigned int cycles)
{
  while(cycles>15)                          // 15 cycles consumed by overhead
    cycles = cycles - 6;                    // 6 cycles consumed each iteration
}

void sent_byte(unsigned char data)
{
  while((IFG2&UCA0TXIFG)==0);//判断发送缓冲区是否结束
  UCA0TXBUF=data;  
}

//*************************************************************************
//              串口1 printf接口函数，函数名称和格式必须是下面的
//*************************************************************************
int putchar(int c)
{
   sent_byte(c);        //这里加入自己的串口0或1发送字符函数
   return(c);
}
// SPI port functions

#if TI_CC_RF_SER_INTF == TI_CC_SER_INTF_USCIB0


void TI_CC_SPISetup(void)
{
#if 1

  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;
  TI_CC_CSn_PxDIR |= TI_CC_CSn_PIN;         // /CS disable

  UCB0CTL0 |= UCMST+UCCKPL+UCMSB+UCSYNC;    // 3-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2;                     // 无法使用ACLK     
  UCB0BR0 |= 0x02;                          // UCLK/2
  UCB0BR1 = 0;
  //UCB0MCTL = 0;
  TI_CC_SPI_USCIB0_PxSEL |= TI_CC_SPI_USCIB0_SIMO | TI_CC_SPI_USCIB0_SOMI | TI_CC_SPI_USCIB0_UCLK;
                                            // SPI option select
  TI_CC_SPI_USCIB0_PxDIR |= TI_CC_SPI_USCIB0_SIMO | TI_CC_SPI_USCIB0_UCLK;
                                            // SPI TXD out direction
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
#endif
}

void TI_CC_SPIWriteReg(char addr, char value)
{
    TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;      // /CS enable
    while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
    IFG2 &= ~UCB0RXIFG;                     // Clear flag
    UCB0TXBUF = addr;                       // Send address
    while (!(IFG2&UCB0RXIFG));              // Wait for TX to finish
    IFG2 &= ~UCB0RXIFG;                     // Clear flag
    UCB0TXBUF = value;                      // Send data
    while (!(IFG2&UCB0RXIFG));              // Wait for TX to finish
    TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;       // /CS disable
}

void TI_CC_SPIWriteBurstReg(char addr, char *buffer, char count)
{
    unsigned int i;

    TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;      // /CS enable
    while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = addr | TI_CCxxx0_WRITE_BURST;// Send address
    while (!(IFG2&UCB0RXIFG));              // Wait for TX to finish
    for (i = 0; i < count; i++)
    {
      IFG2 &= ~UCB0RXIFG;
      UCB0TXBUF = buffer[i];                // Send data
      while (!(IFG2&UCB0RXIFG));            // Wait for TX to finish
    }
    //while (!(IFG2&UCB0RXIFG));
    TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;       // /CS disable
}

char TI_CC_SPIReadReg(char addr)
{ 
  char x;
  __bic_SR_register(GIE);                   //关全局中断
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  while (!(IFG2&UCB0TXIFG));                // Wait for TX to finish
  UCB0TXBUF = (addr | TI_CCxxx0_READ_SINGLE);// Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TX to finish
  UCB0TXBUF = 0;                            // Dummy write so we can read data
  // Address is now being TX'ed, with dummy byte waiting in TXBUF...
  while (!(IFG2&UCB0RXIFG));                // Wait for RX to finish
  // Dummy byte RX'ed during addr TX now in RXBUF
  IFG2 &= ~UCB0RXIFG;                       // Clear flag set during addr write
  while (!(IFG2&UCB0RXIFG));                // Wait for end of dummy byte TX
  // Data byte RX'ed during dummy byte write is now in RXBUF
  x = UCB0RXBUF;                            // Read data
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable
__bis_SR_register(GIE);                     //开全局中断
  return x;
}

void TI_CC_SPIReadBurstReg(char addr, char *buffer, char count)
{
  char i;
  __bic_SR_register(GIE);                     //关全局中断 如果不关闭全局中断，CC1101 会死机的。
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
  IFG2 &= ~UCB0RXIFG;                       // Clear flag
  UCB0TXBUF = (addr | TI_CCxxx0_READ_BURST);// Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = 0;                            // Dummy write to read 1st data byte
  // Addr byte is now being TX'ed, with dummy byte to follow immediately after
  while (!(IFG2&UCB0RXIFG));                // Wait for end of addr byte TX
  IFG2 &= ~UCB0RXIFG;                       // Clear flag
  while (!(IFG2&UCB0RXIFG));                // Wait for end of 1st data byte TX
  // First data byte now in RXBUF
  for (i = 0; i < (count-1); i++)
  {
    UCB0TXBUF = 0;                          //Initiate next data RX, meanwhile..
    buffer[i] = UCB0RXBUF;                  // Store data from last data RX
    while (!(IFG2&UCB0RXIFG));              // Wait for RX to finish
  }
  buffer[count-1] = UCB0RXBUF;              // Store last RX byte in buffer
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable
  __bis_SR_register(GIE);                   //开全局中断
}

char TI_CC_SPIReadStatus(char addr)
{
  char x;
  __bic_SR_register(GIE);                     //关全局中断 如果不关闭全局中断，CC1101 会死机的。
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  while (TI_CC_SPI_USCIB0_PxIN & TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
  IFG2 &= ~UCB0RXIFG;                       // Clear flag set during last write
  UCB0TXBUF = (addr | TI_CCxxx0_READ_BURST);// Send address
  while (!(IFG2&UCB0RXIFG));                // Wait for TX to finish
  IFG2 &= ~UCB0RXIFG;                       // Clear flag set during last write
  UCB0TXBUF = 0;                            // Dummy write so we can read data
  while (!(IFG2&UCB0RXIFG));                // Wait for RX to finish
  x = UCB0RXBUF;                            // Read data
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable
  __bis_SR_register(GIE);                     //开全局中断
  return x;
}

void TI_CC_SPIStrobe(char strobe)
{
  __bic_SR_register(GIE);                      //关全局中断 如果不关闭全局中断，CC1101 会死机的。
  IFG2 &= ~UCB0RXIFG;                       // Clear flag
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
  UCB0TXBUF = strobe;                       // Send strobe
  // Strobe addr is now being TX'ed
  while (!(IFG2&UCB0RXIFG));                // Wait for end of addr TX
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable
  __bis_SR_register(GIE);                   //开全局中断
}

void TI_CC_PowerupResetCCxxxx(void)
{
#if 1
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;
  TI_CC_Wait(30);
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;
  TI_CC_Wait(30);
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;
  TI_CC_Wait(45);
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
  UCB0TXBUF = TI_CCxxx0_SRES;               // Send strobe
  // Strobe addr is now being TX'ed
  IFG2 &= ~UCB0RXIFG;                       // Clear flag
  while (!(IFG2&UCB0RXIFG));                // Wait for end of addr TX
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable
#endif 
#if 0
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;
  TI_CC_Wait(30);
  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;
  TI_CC_Wait(30);
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;
  TI_CC_Wait(45);

  TI_CC_CSn_PxOUT &= ~TI_CC_CSn_PIN;        // /CS enable
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);// Wait for CCxxxx ready
  UCB0TXBUF = TI_CCxxx0_SRES;               // Send strobe
  // Strobe addr is now being TX'ed
  IFG2 &= ~UCB0RXIFG;                       // Clear flag
  while (!(IFG2&UCB0RXIFG));                // Wait for end of addr TX
  while (TI_CC_SPI_USCIB0_PxIN&TI_CC_SPI_USCIB0_SOMI);
  TI_CC_CSn_PxOUT |= TI_CC_CSn_PIN;         // /CS disable  
#endif
}

#endif
