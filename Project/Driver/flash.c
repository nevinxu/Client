#include "include.h"

unsigned char Internal_flashdata[64];

void ReadFlash(unsigned long int waddr,unsigned int length)  //最多只能64个字节
{
  while(FCTL3&BUSY);														// wait for write finished
  unsigned char i=0;
  if(length>64) length=64;
  while(length--)
  {
    Internal_flashdata[i++]=*(unsigned char *)waddr++;
  }
}
//----------------------------------------------------------------------
//功能：将数据写入flash；
//日期：2006.12.13
//输入：waddr为flash的地址，length为数据长度，*buf为指向数据的指针
//输出：无
//----------------------------------------------------------------------
unsigned char  WriteFlash(unsigned long int waddr,const unsigned char *buf,unsigned int length)
{   
    unsigned int i;
    unsigned char ErrorFlag = 0;     
    unsigned char reg1,reg2,temp_char;
    FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator    
        while (FCTL3&BUSY);		//wait for idle
        reg1=_BIC_SR(GIE);		// disabe all the GIE
  //      reg2=SFRIE1;
  //      SFRIE1=0;			//save interrupt& disabe all the interrupt
        //Unlock Info Memory
 //       FCTL4 = FWKEY;                 //Clear LOCKINFO
        //Unlock SegmentA
        FCTL3 = FWKEY;		       // Clear Lock bit
        if(FCTL3&LOCKA)          //   1->0
        {
            FCTL3=FWKEY+LOCKA;
        }
        FCTL1 =FWKEY+WRT;			// Set WRT bit for write operation
        for (i=0; i<length; i++)
        {
            __data20_write_char(waddr,*buf);	 // Write value to flash
            waddr++ = *buf; 
            temp_char=__data20_read_char(waddr);	 // read value from flash
            if (temp_char != *buf)		 //compare read_value with write_value
            {
                ErrorFlag = 1; 
            }
            
            waddr++;
            buf++;
        }
        
        while(FCTL3&BUSY);		//wait for write finished
        FCTL1 = FWKEY;      
 //       FCTL4 = FWKEY+LOCKINFO;         //set LOCKINFO
        FCTL3 = FWKEY+LOCK;             // Lock the flash
        if(!(FCTL3&LOCKA))              //0->1
        {
            FCTL3=FWKEY+LOCKA+LOCK;
        }
        if(reg1&GIE)
            _BIS_SR(GIE);               // return the GIE
  //      SFRIE1=reg2; 

    
    return ErrorFlag;

}


/************************************************************
* Description: erase 430flash page  
* Parameters: waddr - 430flash page start address 
* Returns: none
************************************************************/
void EraseFlash(unsigned long int Flash_Address)
{ 
    unsigned char reg1,reg2;    
    
    while(FCTL3&BUSY);               //wait for idle
    reg1=_BIC_SR(GIE);  
 //   reg2=SFRIE1;
 //   SFRIE1=0;
    //Unlock Info Memory
//    FCTL4 = FWKEY;  //Clear LOCKINFO
    //Unlock SegmentA
    FCTL3 = FWKEY;				// Clear Lock bit
    if(FCTL3&LOCKA)     //  //1->0
    {
        FCTL3=FWKEY+LOCKA;
    }
    FCTL1 = FWKEY+ERASE;             //set erase bit
    __data20_write_char(Flash_Address,0);
    while(FCTL3&BUSY);               // wait for erase finished
    FCTL1 = FWKEY;                  // Clear WRT bit
//   FCTL4 = FWKEY+LOCKINFO;  //set LOCKINFO
    FCTL3 = FWKEY+LOCK;             // Lock the flash
    if(!(FCTL3&LOCKA))   //0->1
    {
        FCTL3=FWKEY+LOCKA+LOCK;
    }
    if(reg1&GIE)
        _BIS_SR(GIE);
  //  SFRIE1=reg2;
}