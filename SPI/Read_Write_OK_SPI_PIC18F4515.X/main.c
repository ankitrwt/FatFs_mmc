/*
 * File:   newmain.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pic18f4515.h>
#include "diskio.h"
#include "ff.h"
//#include <pic.h>
#include "LCD.h"
//#include "SPI.h"
//#include "SPI_EE.h"
//#include "SD_routines.h"


#define READ    (PORTD & 0x80)
#define WRITE   (PORTD & 0x40)
#define ERASE   (PORTC & 0x80)
//#include "FAT32.h"

/*
 *
 *
 */
void delaymain(unsigned long int x){while(x--);}
FATFS FatFs;		/* FatFs work area needed for each volume */
FIL fp;			/* File object needed for each open file */

void init(){
//    ANSELA = 0x00;
    TRISC = 0x10;
    TRISD = 0xC0;
    TRISB = 0x20;
}

unsigned char buffer[512];

int main(int argc, char** argv) {
    unsigned char mytext[16] = "hello world";
   // unsigned char temp[512];
    unsigned char status =1;
    unsigned int timeout = 0;
    unsigned int cnt = 32;
    char *ptr;
    unsigned int ret;
    unsigned char chck;

    init();
    LCD_init();
    while(status != 0)
    {
        status = (unsigned char)disk_initialize(0);
        timeout++;
        if(timeout == 150)
        {
            LCD_string((u8 *)"INIT ERROR      ");
            while(1);
        }
    }
    LCD_string((u8 *)"SD/MMC card OK! ");
    delaymain(100000); delaymain(100000);

/*    chck = f_mount(&FatFs, "", 0);
    if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("mount Fails    ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }

    while(1)
    {
       chck = f_open(&fp,"text.txt",FA_WRITE | FA_CREATE_ALWAYS);
        if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("Open Fails     ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }

        chck = f_write (&fp,"hello there\r\n",13, &ret);
        if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("Written Fails  ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }


        chck = f_close(&fp);
        if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("Close Fails    ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }
        while(1);
    }
*/
while(1)
{
    if(READ){
        while(READ);
     //   DRESULT disk_read (
//	BYTE drv,			/* Physical drive nmuber (0) */
//	BYTE *buff,			/* Pointer to the data buffer to store read data */
//	DWORD sector,		/* Start sector number (LBA) */
//	UINT count			/* Sector count (1..128) */
//)
        status = (unsigned char)disk_read(0,buffer,1,1);
        if(status != 0){
            LCD_cur(1,1);
            LCD_string("Error Read      ");
        }
        else{
             LCD_cur(1,1);
             LCD_string("Read OK!       ");
        }
    }
    else if(WRITE){
        while(WRITE);
//        DRESULT disk_write (
//	BYTE drv,			/* Physical drive nmuber (0) */
//	const BYTE *buff,	/* Pointer to the data to be written */
//	DWORD sector,		/* Start sector number (LBA) */
//	UINT count			/* Sector count (1..128) */
//)
        for(int i=0; i<512;i++)
            buffer[i] = 'D';

        status = (unsigned char)disk_write(0,buffer,1,1);
        if(status != 0){
            LCD_cur(1,1);
            LCD_string("Error Write     ");
        }
        else{
             LCD_cur(1,1);
             LCD_string("Write OK!      ");
        }
    }
    else if(ERASE)
    {
        while(ERASE);
    }
}
    return (EXIT_SUCCESS);
}
