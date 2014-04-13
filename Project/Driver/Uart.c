#include "include.h"

extern unsigned int TimerValue;
extern unsigned int time_num;
extern unsigned int ModelAddress;  //´²Î»ºÅ

static unsigned char UartBuf; 

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

void  UartRec()
{
  static unsigned char i = 0;
  static unsigned char buf[10];
  buf[i] = UartBuf;
  if(i >= 10)  //µØÖ·»º´æÒç³ö
  {
    i = 4;
    buf[0] = buf[i-4];
    buf[1] = buf[i-3];
    buf[2] = buf[i-2];
    buf[3] = buf[i-1];
    buf[4] = buf[i];
  }
  if((buf[i-4] == 'a') &&(buf[i-3] == 'd') && (buf[i-2] == 'd') && (buf[i-1] == 'r'))   //µØÖ·Ç°×º
  {
    ModelAddress = buf[i];
  }
  i++;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  UartBuf = UCA0RXBUF ;                  // 'u' received?
 // UCA0TXBUF = UartBuf;
}