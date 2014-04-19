#include "include.h"

unsigned char TxBuffer[64];
char RxBuffer[64];
unsigned int MsgBegin = 0xA5B4;   //消息识别符
unsigned int MsgLength;  //消息长度
unsigned char CommandId; //命令或相应类型
unsigned int ModelAddress = 0x0001;  //床位号
long SequenceId = 0;   //消息流水号
unsigned char MsgStatus;  //消息状态
unsigned char TerminalID[6] = {0x00,0x00,0x00,0x00,0x00,0x01};//唯一标识该终端
unsigned char DataRecFlag;   //数据接收标志

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

unsigned char WorkingStateSendSuccessFlag =0;


void MessageHeader()
{
  TxBuffer[0] = MsgBegin;
  TxBuffer[1] = MsgBegin>>8;
  TxBuffer[2] = MsgLength;
  TxBuffer[3] = MsgLength>>8;
  TxBuffer[4] = CommandId;
  TxBuffer[5] = ModelAddress;
  TxBuffer[6] = ModelAddress>>8;
  TxBuffer[7] = SequenceId;
  TxBuffer[8] = SequenceId>>8;
  TxBuffer[9] = SequenceId>>16;
  TxBuffer[10] = SequenceId>>24;       
  TxBuffer[11] = MsgStatus;      
  TxBuffer[12] = TerminalID[0];      
  TxBuffer[13] = TerminalID[1];        
  TxBuffer[14] = TerminalID[2];        
  TxBuffer[15] = TerminalID[3]; 
  TxBuffer[16] = TerminalID[4];        
  TxBuffer[17] = TerminalID[5];       
}


void WorkingStateMsgTransmit(void)
{
  MsgStatus = WorkingStatus;
  MsgLength = MessageHeaderLength+4;
  SequenceId++;
  CommandId = 0x07;
  MessageHeader();
  CurrentSpeed +=3;
  if(CurrentSpeed>=90)
  {
    CurrentSpeed =70;
  }
  TxBuffer[18] = CurrentSpeed;
  TxBuffer[19] = TotalDrip;
  TxBuffer[20] = TotalDrip>>8;
  TxBuffer[21] = TerminalPowerPrecent;
  Transmit( TxBuffer, MsgLength);  
}

void HeartBeatTransmit(void)
{
  MsgStatus = HeartBeatStatus;
  MsgLength = MessageHeaderLength+2;
  SequenceId++;
  CommandId = 0x06;
  TxBuffer[18] = LastTimeDelay;
  TxBuffer[19] = PackageLoseNum;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}

void LoginTransmit(void)
{
  MsgStatus = 0;
  MsgLength = MessageHeaderLength+6;
  SequenceId++;
  CommandId = 0x01;
  MessageHeader();
  TxBuffer[18] = ProtocolVersion;
  TxBuffer[19] = ProtocolVersion>>8;
  TxBuffer[20] = HardwareVersion;
  TxBuffer[21] = HardwareVersion>>8;       
  TxBuffer[22] = Reversed;
  TxBuffer[23] = Reversed;
  Transmit( TxBuffer, MsgLength);  
}

void LogoutTransmit(void)
{
  MsgStatus = LogoutStatus;
  MsgLength = MessageHeaderLength;
  SequenceId++;
  CommandId = 0x03;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}

void LogoutAckTransmit(void)
{
  MsgStatus = LogoutStatus;
  MsgLength = MessageHeaderLength;
  SequenceId++;
  CommandId = 0x04;
  MessageHeader();
  Transmit( TxBuffer, MsgLength);
}


void RecDataCheck()
{
  if((RxBuffer[CommandIdByte] == TerminalWorkingStateAckCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    WorkingStateSendSuccessFlag = 1;
  }
  else if((RxBuffer[CommandIdByte] == TerminalWorkingStateReqCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    WorkingStateSendSuccessFlag = 0;
    WorkingStateMsgTransmit();
    
  }
}
