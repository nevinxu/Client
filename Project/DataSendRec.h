#ifndef _DATASENDREC_
#define _DATASENDREC_

#define HeartBeatInterval    180   //���������ʱ��   3����
#define HeartBeatTimeOut    60   //���������� ���ճ�ʱʱ�� 60��
#define HeartBeatSendTimer      3 //���������ʹ���  3��

#define WorkingStateTimeOut     60 //����״̬����  ���ճ�ʱʱ�� 60��
#define WorkingStateSendTimer   3  //����״̬���ʹ���  3��

extern void WorkingStateMsgTransmit(void);
extern void HeartBeatTransmit(void);
extern void LoginTransmit(void);
extern void LogoutTransmit(void);
extern void LogoutAckTransmit(void);
#endif