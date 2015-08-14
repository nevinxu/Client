#include "include.h"

/* === GLOBALS ============================================================= */
char paTable[9] = {0xc0,0xc8,0x85,0x51,0x3a,0x06,0x1c,0x6c,0x68};//cc1101��Ӧ����ֵ10, 7�� 5�� 0��-5, -10��-15,-20,-30

char paTableValue = 0; //10dBm

extern unsigned char RxBuffer[64];

void RadioInit(void)
{
//  static unsigned char txBuffer;
    TI_CC_SPISetup();                         // SPI�˿ڳ�ʼ��
    TI_CC_PowerupResetCCxxxx();               // �����λCCоƬ
    
    writeRFSettings();                        // ���üĴ�������
    
    //�����������    
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, &paTable[paTableValue], 1);//���÷��͹���  
   
    
    TI_CC_GDO0_PxSEL &=~ (TI_CC_GDO0_PIN+TI_CC_GDO2_PIN);   //������⹦��
    TI_CC_GDO0_PxDIR &=~ (TI_CC_GDO0_PIN+TI_CC_GDO2_PIN);
    
    TI_CC_GDO0_PxIES &=~ TI_CC_GDO0_PIN;    
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO0_PIN;
    TI_CC_GDO0_PxIE &=~ TI_CC_GDO0_PIN; 
 
#if 1  
    TI_CC_GDO0_PxIES |= TI_CC_GDO2_PIN;  // down edge
  //  TI_CC_GDO0_PxIES &=~ TI_CC_GDO2_PIN;  // down edge
    TI_CC_GDO0_PxIFG &=~ TI_CC_GDO2_PIN; // Clear a pending interrupt 
    TI_CC_GDO0_PxIE |= TI_CC_GDO2_PIN;   // Enable the interrupt
#endif
    
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);    
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX); 
    TI_CC_SPIStrobe(TI_CCxxx0_SFTX);
    TI_CC_SPIStrobe(TI_CCxxx0_SRX); // Strobe SRX  
    
  //  WOROn();
    
}

void Transmit(char *buffer, unsigned char length)
{     
    ReceiveOff();  //����ر�
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x2e); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x06); // GDO0 output pin config.
    RFSendPacket(buffer, length);
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x06); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x2e); // GDO0 output pin config.
    ReceiveOn();
    
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
    TI_CC_GDO0_PxIFG &=~ (TI_CC_GDO2_PIN); // Clear a pending interrupt 
    TI_CC_GDO0_PxIES |= (TI_CC_GDO2_PIN);  // down edge
    TI_CC_GDO0_PxIE |= (TI_CC_GDO2_PIN);   // Enable the interrupt 
    TI_CC_GDO0_PxIFG &=~ (TI_CC_GDO2_PIN); // Clear a pending interrupt 
  
    // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    TI_CC_SPIStrobe(TI_CCxxx0_SFTX);      // Flush RXFIFO
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
    TI_CC_SPIStrobe(TI_CCxxx0_SRX);

}


void ReceiveOff(void)
{
   
  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
  // such that the RXFIFO is empty prior to receiving a packet.
  TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
  TI_CC_SPIStrobe(TI_CCxxx0_SFRX); 
  TI_CC_SPIStrobe(TI_CCxxx0_SFTX);
  TI_CC_GDO0_PxIFG &=~ (TI_CC_GDO2_PIN); // Clear a pending interrupt 
  TI_CC_GDO0_PxIE &=~   (TI_CC_GDO2_PIN);   // Disable the interrupt 
  TI_CC_GDO0_PxIFG &=~ (TI_CC_GDO2_PIN); // Clear a pending interrupt 
 
  
}


void RFSetRx(void)
{ 
		//  WOR����
    TI_CC_SPIWriteReg(TI_CCxxx0_WORCTRL,0xF8);
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    Delay_us(30);
    TI_CC_SPIStrobe(TI_CCxxx0_SRX);
}

void WOROn(void)
{
  unsigned char calib1,calib0;
  //
  TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    //--------------------------------------------------------------------------------
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM2, 0x00); // RX_TIME = 0
    TI_CC_SPIReadReg(TI_CCxxx0_MCSM2);
    // (duty cycle = 1.563% when WOR_RES = 0)
//    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT1, 0x2b); // EVENT0 = 320ms
//    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT0, 0x55);  //  WOR_RES = 0
//    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT1, 0x1F); // EVENT0 = 230ms
//    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT0, 0x40);  //  WOR_RES = 0
    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT1, 0x87); // EVENT0 = 1000ms
    TI_CC_SPIWriteReg(TI_CCxxx0_WOREVT0, 0x6A);  //  WOR_RES = 0    
    TI_CC_SPIWriteReg(TI_CCxxx0_WORCTRL, 0x78); // EVENT1 = 111       x*750/fxosc
    // RC_CAL = 1
    // WOR_RES = 0
    //--------------------------------------------------------------------------------
    Delay_us(3000); // Wait for RCOSC calibration
   // TI_CC_SPIWriteReg(TI_CCxxx0_WORCTRL, 0x30); // EVENT1 = 3
    // RC_CAL = 0
    // WOR_RES = 0
    calib1 = TI_CC_SPIReadStatus(TI_CCxxx0_RCCTRL1_STATUS);
    calib0 = TI_CC_SPIReadStatus(TI_CCxxx0_RCCTRL0_STATUS);
    TI_CC_SPIWriteReg(TI_CCxxx0_RCCTRL1, calib1);
    TI_CC_SPIWriteReg(TI_CCxxx0_RCCTRL0, calib0);
  //  ReceiveOn();
    Delay_us(30);
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
    Delay_us(30);
    TI_CC_SPIStrobe(TI_CCxxx0_SWORRST);
    TI_CC_SPIStrobe(TI_CCxxx0_SWOR);
//--------------------------------------------------------------------------------
}

void WOROff(void)
{

}


