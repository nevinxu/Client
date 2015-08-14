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
  
  UCA0BR0 = 0x45;                           // 32k/9600 - 3.41
                                             //8000000/9600  625
  UCA0BR1 = 0x00;                           //8000000/115200 69
  UCA0MCTL = 0x00;//0x03;                          // Modulation
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

//******************************************************************************
// 描述:发送一字符串往PC
// 输入:const unsigned char *data_point 字符串数组
// 输出:无
void uart_send(const unsigned char *data_point)
{
  while(1)
  {
    while(!(IFG2&UCA0TXIFG)); //query tx ready?
    if((*data_point=='\0'))
    	break;
    else
    {
    	UCA0TXBUF=*data_point;
    	data_point++;
    }
  }
}


void EraseUpDataFlag(void)
{
  unsigned char buffer[2] = {0x01,0x01};
  Flash_SegmentErase(0x1080);
  FlashWrite_8(buffer,0x1080,2);
}

void UpData(void)
{
 // WDTCTL = WDTPW + WDT_MRST_0_064;
  asm(" mov &0xFFFE, PC;");                 // 在C中调用汇编指令,实现地址转移
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{

}