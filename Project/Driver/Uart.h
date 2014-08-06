#ifndef _UART_
#define _UART_
#include <msp430.h>

extern void UartInit();
extern void UartSendByte(unsigned char byte);
extern void  UartSend(unsigned char *string, unsigned char num);
extern void UartCommandstatus();
#endif