//----------------------------------------------------------------------------
//  Description:  This file contains definitions specific to the hardware board.
//  Specifically, the definitions include hardware connections with the
//  CCxxxx connector port, LEDs, and switches.
//
//  MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//----------------------------------------------------------------------------






#define TI_CC_LED_PxOUT         P1OUT
#define TI_CC_LED_PxDIR         P1DIR

#define TI_CC_LED1              BIT0  //P1.0 P1.1作为LED 指示灯
#define TI_CC_LED2              BIT1
//#define TI_CC_LED3              BIT2
//#define TI_CC_LED4              TI_CC_LED1

//#define TI_CC_TXE_PxOUT         P2OUT
//#define TI_CC_TXE_PxDIR         P2DIR
//#define TI_CC_TXE_PIN           BIT3

//#define TI_CC_RXE_PxOUT         P2OUT
//#define TI_CC_RXE_PxDIR         P2DIR
//#define TI_CC_RXE_PIN           BIT4

#define TI_CC_SW_PxDIR          P2DIR //P2.0 P2.1 作为按键IO
#define TI_CC_SW_PxOUT          P2OUT
#define TI_CC_SW_PxIN           P2IN
#define TI_CC_SW_PxIE           P2IE
#define TI_CC_SW_PxIES          P2IES
#define TI_CC_SW_PxIFG          P2IFG
#define TI_CC_SW4               0x10
#define TI_CC_SW3               0x20
#define TI_CC_SW2               BIT1
#define TI_CC_SW1               BIT0  //P2.0 作为按键 




//#define TI_CC_GDO1_PxOUT        P3OUT //GDO1 接P3.5
//#define TI_CC_GDO1_PxIN         P3IN
//#define TI_CC_GDO1_PxDIR        P3DIR
//#define TI_CC_GDO1_PIN          BIT5



//----------------------------------------------------------------------------
// Select which port will be used for interface to CCxxxx
//----------------------------------------------------------------------------

//#define TI_CC_RF_SER_INTF      TI_CC_SER_INTF_BITBANG 
