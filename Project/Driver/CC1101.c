#include "include.h"

/* === GLOBALS ============================================================= */
char paTable[9] = {0xc0,0xc8,0x85,0x51,0x3a,0x06,0x1c,0x6c,0x68};//cc1101��Ӧ����ֵ10, 7�� 5�� 0��-5, -10��-15,-20,-30

char paTableValue = 0; //10dBm

extern unsigned char RxBuffer[64];

void RadioInit(void)
{
    TI_CC_SPISetup();                         // SPI�˿ڳ�ʼ��
    TI_CC_PowerupResetCCxxxx();               // �����λCCоƬ
    
    writeRFSettings();                        // ���üĴ�������
    
    //�����������    
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, &paTable[paTableValue], 1);//���÷��͹���  
   
    TI_CC_GDO0_PxSEL &=~ TI_CC_GDO0_PIN;   //������⹦��
    TI_CC_GDO0_PxIES |=TI_CC_GDO0_PIN;    
    TI_CC_GDO0_PxDIR &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN; 
    
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);    
    ReceiveOn();
    
}

void Transmit(char *buffer, unsigned char length)
{       
    RFSendPacket(buffer, length);
}

void ClearRecBuf()
{
  unsigned char i;
  for(i=0;i<64;i++)
  {
    RxBuffer[i] = 0;
  }
}

void ReceiveData()
{
  ClearRecBuf();
  RFReceivePacket(RxBuffer);       // �������ݰ��ж�x
}

/*
 * receive
 */
void ReceiveOn(void)
{
    TI_CC_GDO0_PxIES &=~ TI_CC_GDO2_PIN;  // up edge
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
