
/*
 * File:   LCD.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:32 PM
 */

#include "SPI.h"
#include "pic.h"

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
u8 Read_8Bits()
{
    u8 temp,i;
    for(i=0;i<8;i++)
    {
       SCK = 0;
       SPI_delay(SDELAY);
       temp = temp<<1;
       if(SDI)
           temp = temp+1;
       else
           temp = temp+0;

       SCK = 1;
       SPI_delay(SDELAY);
       SPI_delay(SDELAY);
       //Clock();
    }
    return temp;
}

void Send_8Bits(u8 data)
{
    u8 temp,i;
    temp = data;
    for(i=0;i<8;i++)
    {
       SCK = 0;
       SPI_delay(SDELAY);

        if(temp&0x80)
            SDO = 1;
        else
            SDO = 0;

        temp = temp<<1;
       SPI_delay(SDELAY);
        SCK = 1;
        SPI_delay(SDELAY);
        SPI_delay(SDELAY);
        //Clock();
    }
}

void Write_Enable()
{
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(WREN);
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);
}

void Write_Disable()
{
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(WRDI);
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);
}

u8 Read_Status()
{
    u8 temp;
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(RDSR);
    SPI_delay(SDELAY);
    temp = Read_8Bits();
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);
return temp;
}

void Write_Status(u8 data)
{
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(WRSR);
    SPI_delay(SDELAY);
    Send_8Bits(data);
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);
}

void Write_Byte(u8 data,u8 address)
{
    nWP = 1;
    //Write_Status(NONE_PAGE_PROTECTED);
    Write_Enable();
    while(WRITE_ENABLE_LATCH);
    Write_Status(NONE_PAGE_PROTECTED);
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(WRITE);
    SPI_delay(SDELAY);
    Send_8Bits(address);
    SPI_delay(SDELAY);
    Send_8Bits(data);
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);

    while(WRITE_IN_PROGRESS);

    nWP = 0;
//Write_Disable();
}

u8 Read_Byte(u8 address)
{
    u8 temp;
    nCS = 0;
    SPI_delay(SDELAY);
    Send_8Bits(READ);
    SPI_delay(SDELAY);
    Send_8Bits(address);
    SPI_delay(SDELAY);
    temp = Read_8Bits();
    SPI_delay(SDELAY);
    nCS = 1;
    SPI_delay(SDELAY);

    return temp;
}