#include "include.h"

extern unsigned int TimerValue;
extern unsigned int time_num;
extern unsigned char VoiceLevel;;  //声音等级

extern unsigned int ModelAddress;  //床位号

extern unsigned char AlarmUpperValue;  //报警上限值
extern  unsigned char AlarmLowerValue; //报警下限值

extern signed char RSSIValue;

extern unsigned char CurrentSpeed;
extern unsigned int TotalDrop;
extern unsigned char TerminalPowerPrecent;


unsigned char UartStatusFlag; 

void UartInit()
{
  P6SEL |= BIT5+BIT6;                       // P6.5,6 = USCI_A0 RXD/TXD
  UCA0CTL1 |= UCSSEL1;                     // CLK = SMCLK
//  UCA0BR0 = 0x71;                           // 32k/9600 - 3.41
//                                             //8000000/9600  625
//  UCA0BR1 = 0x02;                           //
  
  UCA0BR0 = 0x41;                           // 32k/9600 - 3.41
                                             //8000000/9600  625
  UCA0BR1 = 0x03;                           //
  UCA0MCTL = 0x04;//0x03;                          // Modulation
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

void UartCommandstatus()
{
  UartSendByte(0x1b);
  UartSendByte(0x40);
  UartSendByte(CurrentSpeed);
  UartSendByte(TotalDrop>>8);
  UartSendByte(TotalDrop);
  UartSendByte(TerminalPowerPrecent);
  UartSendByte(AlarmUpperValue);
  UartSendByte(AlarmLowerValue);
  UartSendByte(ModelAddress>>8);
  UartSendByte(ModelAddress);
  UartSendByte(VoiceLevel);
  UartSendByte(RSSIValue);
//  UartSendByte(AlarmUpperValue);
//  UartSendByte(AlarmLowerValue);
//    UartSendByte(CurrentSpeed);
//  UartSendByte(TotalDrop>>8);
//  UartSendByte(TotalDrop);
//  UartSendByte(TerminalPowerPrecent);
//  UartSendByte(AlarmUpperValue);
//  UartSendByte(AlarmLowerValue);
}



#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  static unsigned uartbuffer[4];
  unsigned char i;
  for(i = 3; i>0;i--)
  {
    uartbuffer[i] = uartbuffer[i-1];
  }
  uartbuffer[0] = UCA0RXBUF ;                 
  if((uartbuffer[0] == 0xcc) &&(uartbuffer[1] == 0x99)&&(uartbuffer[2] == 0x26)&&(uartbuffer[3] == 0x48))
  {
    UartStatusFlag = 1; 
  }
  
  
  
  
}