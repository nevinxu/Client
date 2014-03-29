#ifndef _DATASENDREC_
#define _DATASENDREC_

#define HeartBeatInterval    180   //心跳包间隔时间   3分钟
#define HeartBeatTimeOut    60   //心跳包发送 接收超时时间 60秒
#define HeartBeatSendTimer      3 //心跳包发送次数  3次

#define WorkingStateTimeOut     60 //工作状态发送  接收超时时间 60秒
#define WorkingStateSendTimer   3  //工作状态发送次数  3次

extern void WorkingStateMsgTransmit(void);
extern void HeartBeatTransmit(void);
extern void LoginTransmit(void);
extern void LogoutTransmit(void);
extern void LogoutAckTransmit(void);
#endif