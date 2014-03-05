#include "include.h"

unsigned char Sen,Min,Hour,Day,Month,Year;
unsigned char MonthDay[13] = {0,31,30,31,30,31,30,31,31,30,31,30,31};

void InitDigtalClock()
{
  Sen =0;
  Min =0;
  Hour = 0;
  Day = 12;
  Month = 12;
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
void DisplayDigtalClockClock()
{
  Display_Digit(DOWNLINE, 3, Hour);
  Display_Digit(DOWNLINE, 7, Min);
  Display_Digit(DOWNLINE, 11,Sen);
  Display_Digit(UPLINE, 27, Month);
  Display_Digit(UPLINE, 23, Day);
}