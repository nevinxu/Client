#include "include.h"

/* === GLOBALS ============================================================= */
//char paTable[8] = {0x17,0x1c,0x26,0x57,0x60,0x8d,0xc8,0xc0};//cc1100对应功率值-20,-15，-10，-5，0，5，7，10
char paTable[8] = {0x0E,0x1D,0x34,0x68,0x60,0x84,0xc8,0xc0};//cc1101对应功率值-20,-15，-10，-5，0，5，7，10
//char paTable[8] = {0x97,0x6e,0x7f,0x97,0xa9,0xbb,0xfe,0xff};//cc2500对应功率值-10，-8，-6，-4，-2，0，1
//char paTable[8] = {0x0e,0x1c,0x26,0x2b,0x60,0x86,0xcb,0xc2};//cc1100e对应功率值-20,-15，-10，-5，0，5，7，10
char paTableLen = 7; //10dBm

char TxBuffer[64]= {PACKET_LEN, 0xBB,0xCC,0xDD,0xEE,0xFF};
char RxBuffer[64];

void InitRadio(void)
{
    TI_CC_SPISetup();                         // SPI端口初始化
    TI_CC_PowerupResetCCxxxx();               // 软件复位CC芯片
    
    writeRFSettings();                        // 配置寄存器设置
    
    //TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    //txBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_MARCSTATE); 
    //SPI验证
    TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_IOCFG2);//,   0x06
    TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_IOCFG0);//,   0x06  
    TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_ADDR);  //,   0x00
    
    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_VERSION);  //,   0x04 CC1101
    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_PARTNUM);  //,   0x04 CC1101
    
    //输出功率设置    
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, &paTable[paTableLen], 1);//配置发送功率  
    TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_PATABLE);
    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_PATABLE);
    
     
    
    TI_CC_GDO0_PxSEL &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIES |=TI_CC_GDO0_PIN;
//    TI_CC_GDO0_PxREN |=TI_CC_GDO0_PIN;      
    TI_CC_GDO0_PxDIR &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN; 
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_MARCSTATE); 
    
    ReceiveOn();
    
}

void Transmit(char *buffer, unsigned char length)
{       
    RFSendPacket(buffer, length);
    Delay_ms(10);
    RFSendPacket(buffer, length);  
}

void TransmitRate(unsigned int Rate)
{
  TxBuffer[0] = 10;
  TxBuffer[1] = (Rate>>8);
  TxBuffer[2] = Rate&0xff;
  Transmit( TxBuffer, TxBuffer[0]);  
}


void ReceiveData(char length)
{
   RxBuffer[CRC_LQI_IDX] = RFReceivePacket(RxBuffer,&length);       // 接收数据包判断x
}

/*
 * receive
 */
void ReceiveOn(void)
{
    TI_CC_GDO0_PxIES &=~ TI_CC_GDO2_PIN;  // Falling edge
    TI_CC_GDO0_PxIFG &= ~TI_CC_GDO2_PIN; // Clear a pending interrupt 
    TI_CC_GDO0_PxIE |= TI_CC_GDO2_PIN;   // Enable the interrupt 
    
    // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX); 
    TI_CC_SPIStrobe(TI_CCxxx0_SRX); // Strobe SRX  
}

/*
 * Wor On
 */
void WorOn(void)
{
    TI_CC_GDO0_PxIES &=~ TI_CC_GDO2_PIN;  //  UP     edge
    TI_CC_GDO0_PxIFG &= ~TI_CC_GDO2_PIN; // Clear a pending interrupt 
    TI_CC_GDO0_PxIE |= TI_CC_GDO2_PIN;   // Enable the interrupt 
    
    // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX); 
    TI_CC_SPIStrobe(TI_CCxxx0_SWOR);           // Starts Wake-on-RadiO
}


void ReceiveOff(void)
{
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO2_PIN; // Clear a pending interrupt 
    TI_CC_GDO0_PxIE &=~ TI_CC_GDO2_PIN;   // Disable the interrupt 
   
  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
  // such that the RXFIFO is empty prior to receiving a packet.
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX);                      
}
