#include "CC1100-CC2500.h"
#include "TI_CC_CC1100-CC2500.h"
#include "TI_CC_spi.h"
#include "include.h"
#define TI_CC_RF_FREQ  433     //433 , 868, 915, 2400 无线频率设置 
#define RF_RATR        1200

extern unsigned char Rf_Interval_Num;  //RF打开间断时间


signed char RSSIValue;
//-------------------------------------------------------------------------------------------------------
//  void writeRFSettings(void)
//
//  DESCRIPTION:
//  Used to configure the CCxxxx registers.  There are five instances of this
//  function, one for each available carrier frequency.  The instance compiled
//  is chosen according to the system variable TI_CC_RF_FREQ, assigned within
//  the header file "TI_CC_hardware_board.h".
//
//  ARGUMENTS:
//      none
//-------------------------------------------------------------------------------------------------------
void writeRFSettings(void)
{
#if TI_CC_RF_FREQ == 433                          // 433 MHz
// Chipcon
// Product = CC1101
// Chip version = A   (VERSION = 0x04)
// Crystal accuracy = 10 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 541.666667 kHz
// Deviation = 127 kHz
// Datarate = 249.938965 kBaud
// Modulation = (1) GFSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 432.999817 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (41) CHIP_RDY
    
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x06); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x10); // GDO0 output pin config.
       
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL1,  0x0C); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL0,  0x00); // Freq synthesizer control.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ2,    0x10); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ1,    0xA7); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ0,    0x62); // Freq control word, low byte.
    
 //   TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2D); // Modem configuration.
 //   TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x3B); // Modem configuration.
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x25); // Modem configuration. 2.5k
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x83); // Modem configuration.
    
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x28); // Modem configuration.  10k
 //   TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x93); // Modem configuration.
    
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2A); // Modem configuration.  30k
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x2E); // Modem configuration.  
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2B); // Modem configuration.  100k
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0xF8); // Modem configuration. 
    
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2d); // Modem configuration.  250k
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x3b); // Modem configuration. 
    
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2c); // Modem configuration.  150k
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x7a); // Modem configuration. 
    
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0x2A); // Modem configuration.  50k
//    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0xF8); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG2,  0x13); // Modem configuration.   GFSK   检测到30/32 同步字位
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG1,  0x22); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG0,  0xF8); // Modem configuration.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_CHANNR,   RFCHANNAL); // Channel number.
    TI_CC_SPIWriteReg(TI_CCxxx0_DEVIATN,  0x62); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_CCxxx0_ADDR,     0x00); // Device address.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_FREND1,   0xB6); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREND0,   0x10); // Front end RX configuration.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM1 ,   0x30); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    
    TI_CC_SPIWriteReg(TI_CCxxx0_FOCCFG,   0x1D); // Freq Offset Compens. Config
    TI_CC_SPIWriteReg(TI_CCxxx0_BSCFG,    0x1C); //  Bit synchronization config.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL2, 0xC7); // AGC control.
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL1, 0x00); // AGC control.
    TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL0, 0xB0); // AGC control.

    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL3,   0xEA); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL2,   0x2A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL0,   0x1F); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSTEST,   0x59); // Frequency synthesizer cal.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST2,    0x88); // Various test settings.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST1,    0x31); // Various test settings.   
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST0,    0x09); // Various test settings.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, 0x04+0x08); // Packet automation control.   
    //当CRC 不为OK 时，开启自动   CRC_AUTOFLUSH
//RX FIFO 刷新。这就要求RXIFIFO
//中只有一个数据包，且数据包长度不
//能超过RX FIFO 的大小。
    
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL0, 0x05); // Packet automation control.
    
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTLEN,   0xFF); // Packet length.
    TI_CC_SPIWriteReg(TI_CCxxx0_FIFOTHR,  0x07);
    
    
    TI_CC_SPIWriteReg(TI_CCxxx0_SYNC1,    RFSYNC1);        // Sync word, high byte
    TI_CC_SPIWriteReg(TI_CCxxx0_SYNC0,    RFSYNC0);        // Sync word, low byte
    
    
#endif    
}

//-----------------------------------------------------------------------------
//  void RFSendPacket(char *txBuffer, char size)
//
//  DESCRIPTION:
//  This function transmits a packet with length up to 63 bytes.  To use this
//  function, GD00 must be configured to be asserted when sync word is sent and
//  de-asserted at the end of the packet, which is accomplished by setting the
//  IOCFG0 register to 0x06, per the CCxxxx datasheet.  GDO0 goes high at
//  packet start and returns low when complete.  The function polls GDO0 to
//  ensure packet completion before returning.
//
//  ARGUMENTS:
//      char *txBuffer
//          Pointer to a buffer containing the data to be transmitted
//
//      char size
//          The size of the txBuffer
//-----------------------------------------------------------------------------
void RFSendPacket(char *txBuffer, char size)
{   
  unsigned int i = 0;
 // __bic_SR_register(GIE);
  TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    __delay_cycles(30000);
    TI_CC_SPIStrobe(TI_CCxxx0_SFTX);         // Change state to TX, initiating
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
    __delay_cycles(30000);
    TI_CC_SPIWriteReg(TI_CCxxx0_TXFIFO,size);
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_TXFIFO, txBuffer, size); // Write TX data
   __delay_cycles(40000); 
    TI_CC_SPIStrobe(TI_CCxxx0_STX);         // Change state to TX, initiating
                                            // data transfer
    while (!(TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN))  //2014.6.14  配置的延时时间  否则会出现crc错误
    {
      i++;
      if(i>200)
        break;
      __delay_cycles(4000);
    }  
                                            // Wait GDO0 to go hi -> sync TX'ed//
    while (TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN)
    {
      i++;
      if(i>200)
        break;
      __delay_cycles(4000);
    }
                                            // Wait GDO0 to clear -> end of pkt//
    __delay_cycles(30000);  //一定要延时 否则数据CRC校验有问题
    TI_CC_SPIStrobe(TI_CCxxx0_SIDLE); 
    TI_CC_SPIStrobe(TI_CCxxx0_SFTX);      // Flush RXFIFO
    TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
    TI_CC_SPIStrobe(TI_CCxxx0_SRX);
//	__bis_SR_register(GIE);
}



//-----------------------------------------------------------------------------
//  char RFReceivePacket(char *rxBuffer, char *length)
//
//  DESCRIPTION:
//  Receives a packet of variable length (first byte in the packet must be the
//  length byte).  The packet length should not exceed the RXFIFO size.  To use
//  this function, APPEND_STATUS in the PKTCTRL1 register must be enabled.  It
//  is assumed that the function is called after it is known that a packet has
//  been received; for example, in response to GDO0 going low when it is
//  configured to output packet reception status.
//
//  The RXBYTES register is first read to ensure there are bytes in the FIFO.
//  This is done because the GDO signal will go high even if the FIFO is flushed
//  due to address filtering, CRC filtering, or packet length filtering.
//
//  ARGUMENTS:
//      char *rxBuffer
//          Pointer to the buffer where the incoming data should be stored
//      char *length
//          Pointer to a variable containing the size of the buffer where the
//          incoming data should be stored. After this function returns, that
//          variable holds the packet length.
//
//  RETURN VALUE:
//      char
//          0x80:  CRC OK
//          0x00:  CRC NOT OK (or no pkt was put in the RXFIFO due to filtering)
//-----------------------------------------------------------------------------


char RFReceivePacket(char *rxBuffer)
{
  static unsigned char i,j;
  char status[2];
  unsigned char pktLen;
  TI_CC_SPIStrobe(TI_CCxxx0_SRX); 
  __delay_cycles(60000);
  if ((TI_CC_SPIReadStatus(TI_CCxxx0_RXBYTES)&TI_CCxxx0_NUM_RXBYTES))
  {
      TI_CC_SPIReadReg(TI_CCxxx0_RXFIFO);
      TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, rxBuffer, 4); // Pull data
      if((rxBuffer[0] == 0xb4) && (rxBuffer[1] == 0xa5))
      {
        pktLen = rxBuffer[2] - 4;
        TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, rxBuffer+4, pktLen);
      }
      TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, status, 2);
      RSSIValue = status[0];
      Rf_Interval_Num = 0;  //RF打开间断时间
      i++;
                                            // Read appended status byte
  }                                       // Return CRC_OK bit
  else
  {
      TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
      TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
      TI_CC_SPIStrobe(TI_CCxxx0_SFTX);
      TI_CC_SPIStrobe(TI_CCxxx0_SRX);
      Rf_Interval_Num = 0;  //RF打开间断时间
      j++;
      return 0;                             // Error
  }
  TI_CC_SPIStrobe(TI_CCxxx0_SIDLE);
  TI_CC_SPIStrobe(TI_CCxxx0_SFTX);
  TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
  TI_CC_SPIStrobe(TI_CCxxx0_SRX);
  return 1;
}
