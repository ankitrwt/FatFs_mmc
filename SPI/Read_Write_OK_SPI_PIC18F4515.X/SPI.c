
/*
 * File:   LCD.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:32 PM
 */

#include "SPI.h"
#include <pic18f4515.h>
//#include <pic.h>

#define SDELAY 1

#define READ  0x03
#define WRITE 0x02
#define WRDI  0x04
#define WREN  0x06
#define RDSR  0x05
#define WRSR  0x01

#define WRITE_IN_PROGRESS    (!(Read_Status()&0x01) == 0x01)
#define WRITE_ENABLE_LATCH   (!(Read_Status()&0x02) == 0x02)

#define ALL_PAGE_PROTECTED   0x0C
#define NONE_PAGE_PROTECTED  0x00

void SPI_delay(u16 x){while(x--);}

/*
void Clock()
{
    SCK = 1;
    SPI_delay(SDELAY);
    SCK = 0;
    SPI_delay(SDELAY);
}
*/
u8 SPI_Read_8Bits()
{
 u8 temp,i;
    for(i=0;i<8;i++)
    {
       SCK_LOW;
       SPI_delay(SDELAY);
       temp = temp<<1;
       if(PORTC & 0x10)
           temp = temp+1;
       else
           temp = temp+0;

       SCK_HIGH;
       SPI_delay(SDELAY);
       SPI_delay(SDELAY);
       //Clock();
    }
    return temp;
}

void SPI_Send_8Bits(u8 data)
{
    u8 temp,i;
    temp = data;
    for(i=0;i<8;i++)
    {
       SCK_LOW;
       SPI_delay(SDELAY);

        if(temp&0x80)
            SDO_HIGH;
        else
            SDO_LOW;

        temp = temp<<1;
       SPI_delay(SDELAY);
        SCK_HIGH;
        SPI_delay(SDELAY);
        SPI_delay(SDELAY);
        //Clock();
    }
}

