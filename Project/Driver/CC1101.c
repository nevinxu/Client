#include "include.h"

/* === GLOBALS ============================================================= */
char paTable[9] = {0xc0,0xc8,0x85,0x51,0x3a,0x06,0x1c,0x6c,0x68};//cc1101对应功率值10, 7， 5， 0，-5, -10，-15,-20,-30

char paTableValue = 0; //10dBm

extern unsigned char RxBuffer[64];

void RadioInit(void)
{
    TI_CC_SPISetup();                         // SPI端口初始化
    TI_CC_PowerupResetCCxxxx();               // 软件复位CC芯片
    
    writeRFSettings();                        // 配置寄存器设置
    
    //TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    //txBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_MARCSTATE); 
    //SPI验证
 //   TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_IOCFG2);//,   0x06
 //   TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_IOCFG0);//,   0x06  
 //   TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_ADDR);  //,   0x00
    
 //   TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_VERSION);  //,   0x04 CC1101
 //   TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_PARTNUM);  //,   0x04 CC1101
    
    //输出功率设置    
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, &paTable[paTableValue], 1);//配置发送功率  
//    TxBuffer[63]= TI_CC_SPIReadReg(TI_CCxxx0_PATABLE);
//    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_PATABLE);
    
     
    
    TI_CC_GDO0_PxSEL &=~ TI_CC_GDO0_PIN;   //清除特殊功能
    TI_CC_GDO0_PxIES |=TI_CC_GDO0_PIN;
//    TI_CC_GDO0_PxREN |=TI_CC_GDO0_PIN;      
    TI_CC_GDO0_PxDIR &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN; 
    
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    
//    TxBuffer[63]= TI_CC_SPIReadStatus(TI_CCxxx0_MARCSTATE); 
    
    ReceiveOn();
    
}

void Transmit(char *buffer, unsigned char length)
{       
    RFSendPacket(buffer, length);
}

void ReceiveData(char length)
{
   RFReceivePacket(RxBuffer,&length);       // 接收数据包判断x
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
