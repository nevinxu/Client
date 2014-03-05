#include "include.h"

extern unsigned int TimerValue;
extern unsigned int time_num;

void UartInit()
{
  P6SEL |= BIT5+BIT6;                       // P6.5,6 = USCI_A0 RXD/TXD
  UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
  UCA0BR0 = 0x71;                           // 32k/9600 - 3.41
                                             //6000000/9600  625
  UCA0BR1 = 0x02;                           //
  UCA0MCTL = 0;//0x03;                          // Modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}


void UartSendByte(unsigned char byte)
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = byte;
}

void  UartSend(unsigned char *string, unsigned char num)
{
  for(unsigned char i=0;i<num;i++)
  {
    UartSendByte(string[i]);
  }
}

void UartCommand1(unsigned int Rate,unsigned int MaxValue,unsigned int MinValue)
{
  UartSendByte(0x1b);
  UartSendByte(0x40);
  UartSendByte(Rate>>8);
  UartSendByte(Rate&0xff);
  UartSendByte(MaxValue>>8);
  UartSendByte(MaxValue&0xff);
  UartSendByte(MinValue>>8);
  UartSendByte(MinValue&0xff);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  static unsigned char buf; 
  buf = UCA0RXBUF ;                  // 'u' received?
  UCA0TXBUF = buf;
}