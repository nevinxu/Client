#include "include.h"

unsigned char TxBuffer[64];
unsigned int MsgBegin = 0xA5B4;   //消息识别符
unsigned int MsgLength;  //消息长度
unsigned char CommandId; //命令或相应类型
unsigned int ModelAddress = 0x0001;  //床位号
long Sequence_Id = 0;   //消息流水号
unsigned char MsgStatus;  //消息状态
unsigned char Terminal_ID[6] = {0x00,0x00,0x00,0x00,0x00,0x01};//唯一标识该终端

#define MessageHeaderLength  18

#define ProtocolVersion         0x0001
#define HardwareVersion         0x0001
#define Reversed                0x00

unsigned char LogoutStatus;
unsigned char HeartBeatStatus;
extern unsigned char WorkingStatus ;

unsigned char LastTimeDelay;
unsigned char PackageLoseNum;

extern unsigned char CurrentSpeed;
extern unsigned int TotalDrip;
extern unsigned char TerminalPowerPrecent;

char RxBuffer[64];

void MessageHeader()
{
  TxBuffer[0] = MsgBegin;
  TxBuffer[1] = MsgBegin>>8;
  TxBuffer[2] = MsgLength;
  TxBuffer[3] = MsgLength>>8;
  TxBuffer[4] = CommandId;
  TxBuffer[5] = ModelAddress;
  TxBuffer[6] = ModelAddress>>8;
  TxBuffer[7] = Sequence_Id;
  TxBuffer[8] = Sequence_Id>>8;
  TxBuffer[9] = Sequence_Id>>16;
  TxBuffer[10] = Sequence_Id>>32;       
  TxBuffer[11] = MsgStatus;      
  TxBuffer[12] = Terminal_ID[0];      
  TxBuffer[13] = Terminal_ID[1];        
  TxBuffer[14] = Terminal_ID[2];        
  TxBuffer[15] = Terminal_ID[3]; 
  TxBuffer[14] = Terminal_ID[4];        
  TxBuffer[15] = Terminal_ID[5];       
}


void WorkingStateMsgTransmit(void)
{
  MsgStatus = WorkingStatus;
  MsgLength = MessageHeaderLength+4;
  Sequence_Id++;
  CommandId = 0x07;
  MessageHeader();
  TxBuffer[16] = CurrentSpeed;
  TxBuffer[17] = TotalDrip;
  TxBuffer[18] = TotalDrip>>8;
  TxBuffer[19] = TerminalPowerPrecent;
  Transmit( TxBuffer, MsgLength);  
}

void HeartBeatTransmit(void)
{
  MsgStatus = HeartBeatStatus;
  MsgLength = MessageHeaderLength+2;
  Sequence_Id++;
  CommandId = 0x06;
  TxBuffer[16] = LastTimeDelay;
  TxBuffer[17] = PackageLoseNum;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}

void LoginTransmit(void)
{
  MsgStatus = 0;
  MsgLength = MessageHeaderLength+6;
  Sequence_Id++;
  CommandId = 0x01;
  MessageHeader();
  TxBuffer[16] = ProtocolVersion;
  TxBuffer[17] = ProtocolVersion>>8;
  TxBuffer[18] = HardwareVersion;
  TxBuffer[19] = HardwareVersion>>8;       
  TxBuffer[20] = Reversed;
  TxBuffer[21] = Reversed;
  Transmit( TxBuffer, MsgLength);  
}

void LogoutTransmit(void)
{
  MsgStatus = LogoutStatus;
  MsgLength = MessageHeaderLength;
  Sequence_Id++;
  CommandId = 0x03;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}

void LogoutAckTransmit(void)
{
  MsgStatus = LogoutStatus;
  MsgLength = MessageHeaderLength;
  Sequence_Id++;
  CommandId = 0x04;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}

