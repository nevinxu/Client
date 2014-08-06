#ifndef _DISPLAY_
#define _DISPLAY_

#define YELLOWLED       BIT0
#define BLUELED         BIT1
#define LEDDIR          P7DIR
#define LEDOUT          P7OUT

#ifdef TestMode 
#define TestLed BIT0   //BIT3=TP26/    BIT1=TP24   BIT2 = TP25
#define TestLEDOUT P7OUT
#define TestLEDIR P7DIR
#endif

#define ON      1
#define OFF     0

#define DisplayRateStartMode    BIT5
#define DisplayRateMode    BIT0
#define DisplayClockMode   BIT1
#define DisplaySleepMode   BIT2
#define DisplayTestStartMode    BIT3
#define DisplayTestMode    BIT4
#define DisplayInitMode    BIT6   //初始化模式

#define ALARMUPPERDEFINEVALUE    70
#define ALARMLOWERDEFINEVALUE    30


#define RSSILEVEL4  -60
#define RSSILEVEL3  -70
#define RSSILEVEL2  -80
#define RSSILEVEL1  -90
#define RSSILEVEL0  -100

extern void  IRSensorInit();
extern void LEDInit(void);
extern void  LEDOn(unsigned char num);
extern void  LEDOff(unsigned char num);

extern void LCDInit(void);
extern void  DisplayNone();
extern void DisplayLine(unsigned char status);
extern void DisplayBedNum(unsigned int num);
extern void DisplayRate(unsigned int rate);
extern void DisplayBattery(unsigned int level);
extern void DisplayMoon(unsigned char status);
extern void DisplaySun(unsigned char status);
extern void DisplayUpAlarm(unsigned int alarmvalue);
extern void DisplayDownAlarm(unsigned int alarmvalue);
extern void DisplayVoice(unsigned char level);
extern void RefreshAlarmValue();
extern void DisplayRFRSSI(unsigned char level);
extern void DisplayRFRSSILevel(signed char rssi);
#endif