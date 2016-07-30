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

    LCD_string((u8 *)"SD/MMC card OK! ");
    delaymain(100000); delaymain(100000);

    LCD_string((u8 *)"Wait... Wait... ");
    delaymain(100000); delaymain(100000);

    chck = f_mount(&FatFs, "", 1);
    if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("mount Fails    ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }

       chck = f_open(&fp,"text.txt",FA_WRITE | FA_CREATE_ALWAYS);
        if( chck != FR_OK){
            LCD_cur(1,1);
            LCD_string("Open Fails     ");
            LCD_func(chck + '0',DTA);
            delaymain(100000); delaymain(100000);
        }

        chck = f_write (&fp,"Ankit Rawat\r\n",13, &ret);
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

    return (EXIT_SUCCESS);
}

