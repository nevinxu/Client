#include "include.h"

//unsigned char TxBuffer[64];
char TxBuffer[64];
char RxBuffer[64];
unsigned int MsgBegin = 0xA5B4;   //消息识别符
unsigned int MsgLength;  //消息长度
unsigned char CommandId; //命令或相应类型
unsigned int ModelAddress;  //床位号
long SequenceId = 0;   //消息流水号
unsigned char MsgStatus;  //消息状态
unsigned char TerminalID[8] = {0x00,0x00,0x00,0x00,0x00,0x01};//唯一标识该终端

extern unsigned char AlarmUpperValue;
extern unsigned char AlarmLowerValue;

extern unsigned char LogInFlag;  //登陆标志

#define MessageHeaderLength  20

#define ProtocolVersion         0x0001
#define HardwareVersion         0x0001
#define Reversed                0x00

unsigned char LogoutStatus;
unsigned char HeartBeatStatus;
extern unsigned char WorkingStatus ;

unsigned char LastTimeDelay;
unsigned char PackageLoseNum;

extern unsigned char CurrentSpeed;
extern unsigned int TotalDrop;
extern unsigned char TerminalPowerPrecent;

unsigned char WorkingStateSendSuccessFlag =0;


void MessageHeader()
{
  TxBuffer[0] = MsgBegin;
  TxBuffer[1] = MsgBegin>>8;
  TxBuffer[2] = MsgLength;
  TxBuffer[3] = MsgLength>>8;
  TxBuffer[4] = ModelAddress;
  TxBuffer[5] = ModelAddress>>8;
  TxBuffer[6] = CommandId;  
  TxBuffer[7] = MsgStatus;   
  TxBuffer[8] = SequenceId;
  TxBuffer[9] = SequenceId>>8;
  TxBuffer[10] = SequenceId>>16;
  TxBuffer[11] = SequenceId>>24;    
   
  TxBuffer[12] = TerminalID[0];      
  TxBuffer[13] = TerminalID[1];        
  TxBuffer[14] = TerminalID[2];        
  TxBuffer[15] = TerminalID[3]; 
  TxBuffer[16] = TerminalID[4];        
  TxBuffer[17] = TerminalID[5];       
  TxBuffer[18] = TerminalID[6];        
  TxBuffer[19] = TerminalID[7];       
}


void WorkingStateMsgTransmit(void)
{
  MsgStatus = 0;
  MsgLength = MessageHeaderLength+6;
  SequenceId++;
  CommandId = 0x07;
  MessageHeader();
  TerminalPowerPrecent = GetBatteyValue()- BATTERYLEVEL0;
 // TxBuffer[20] = 55;
 // TotalDrip ++;
  TxBuffer[20] = CurrentSpeed;
  TxBuffer[21] = AlarmLowerValue;
  TxBuffer[22] = AlarmUpperValue;
  TxBuffer[23] = TerminalPowerPrecent;
  TxBuffer[24] = TotalDrop;
  TxBuffer[25] = TotalDrop>>8;;
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
  TxBuffer[20] = ProtocolVersion;
  TxBuffer[21] = ProtocolVersion>>8;
  TxBuffer[22] = HardwareVersion;
  TxBuffer[23] = HardwareVersion>>8;       
  TxBuffer[24] = Reversed;
  TxBuffer[25] = Reversed;
  Transmit( TxBuffer, MsgLength);  
}

void LoginAckTransmit(void)
{
  MsgStatus = 0;
  MsgLength = MessageHeaderLength;
  SequenceId++;
  CommandId = TerminalLogin_AckCommand;
  MessageHeader();
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


void WorReqTransmit(void)
{
  MsgStatus = 0;
  MsgLength = MessageHeaderLength;
  SequenceId++;
  CommandId = TerminalWOR_ReqCommand;
  MessageHeader();
  Transmit( TxBuffer, MsgLength-2);  
}

void RecDataCheck()
{
  
  static unsigned int m =0;;
  if((RxBuffer[CommandIdByte] == TerminalWorkingStateAckCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    WorkingStateSendSuccessFlag = 1;
  }
  else if((RxBuffer[CommandIdByte] == TerminalWorkingStateReqCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    WorkingStateSendSuccessFlag = 0;   //状态清零
    WorkingStateMsgTransmit();   
  }
  
  else if((RxBuffer[CommandIdByte] == TerminalWorkingStateAckCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {  
   // WOROn(); 
  }
  else if((RxBuffer[CommandIdByte] == TerminalLogin_AckCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    LogInFlag = 1;
   // WOROn();
  }
  else if((RxBuffer[CommandIdByte] == TerminalLogin_ReqCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    LoginTransmit();  
  }
  else if((RxBuffer[CommandIdByte] == TerminalWOR_AckCommand) &&(RxBuffer[StatusByte] == SUCCESS))
  {
    WorReqTransmit();
    m++;
  //  RFSetRx();
  }
  else 
  {
    return;
  }
}
