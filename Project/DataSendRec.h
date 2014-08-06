#ifndef _DATASENDREC_
#define _DATASENDREC_

#define HeartBeatInterval    180   //心跳包间隔时间   3分钟
#define HeartBeatTimeOut    60   //心跳包发送 接收超时时间 60秒
#define HeartBeatSendTimer      3 //心跳包发送次数  3次

#define WorkingStateTimeOut     60 //工作状态发送  接收超时时间 60秒
#define WorkingStateSendTimer   3  //工作状态发送次数  3次


#define CommandIdByte  6
#define StatusByte      7

#define SUCCESS 0

#define TerminalLoginCommand  0x01
#define TerminalLogin_AckCommand  0x02
#define TerminalLogin_ReqCommand  0x0a
#define TerminalLogoutCommand  0x03
#define TerminalLogoutAckCommand  0x04
#define HEARTBEATCommand  0x05
#define HEARTBEATAckCommand  0x06
#define TerminalWorkingStateCommand  0x07
#define TerminalWorkingStateAckCommand  0x08
#define TerminalWorkingStateReqCommand  0x09
#define TerminalWOR_AckCommand  0x0b
#define TerminalWOR_ReqCommand  0x0c

extern void WorkingStateMsgTransmit(void);
extern void HeartBeatTransmit(void);
extern void LoginTransmit(void);
extern void LoginAckTransmit(void);
extern void LogoutTransmit(void);
extern void LogoutAckTransmit(void);
extern void WorReqTransmit(void);
extern void RecDataCheck(void);
#endif