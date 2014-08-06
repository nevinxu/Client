#include "include.h"

unsigned char Sen,Min,Hour,Day,Month,Year;
unsigned char MonthDay[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};

void DigtalClockInit()
{
  Sen =0;
  Min =32;
  Hour = 16;
  Day = 12;
  Month = 21;
}


void DigtalClockRun()
{
  Sen++;
  if(Sen >=60)
  {
    Sen = 0;
    Min++;
    if(Min >= 60)
    {
      Min = 0;
      Hour++;
      if(Hour >= 24)
      {
        Hour =0;
        Day++;
        if(Day > MonthDay[Month])
        {
          Month++;
          if(Month >= 12)
          {
            Year++;
            if(Year>=99)
            {
                Year =0;
            }

          }
        }
      }
    }
  }
}

void DisplayDot()
{
 // Ht1621WrHalfOneData(0, 0x80);
 // Ht1621WrHalfOneData(13,0x80);
}
void DisplayDigtalClock()
{
 // Ht1621ClearDisplay();
 // DisplayDot();
 // Display_Digit(DOWNLINE+ GIGTALCLOCKFLAG, 3, Hour);
 // Display_Digit(DOWNLINE+ GIGTALCLOCKFLAG, 7, Min);
 // Display_Digit(DOWNLINE+ GIGTALCLOCKFLAG, 11,Sen);
 // Display_Digit(UPLINE+ GIGTALCLOCKFLAG, 27, Month);
 // Display_Digit(UPLINE+ GIGTALCLOCKFLAG, 23, Day);
  
}