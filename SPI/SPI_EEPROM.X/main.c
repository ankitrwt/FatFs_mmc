/*
 * File:   newmain.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pic.h>
#include "LCD.h"
#include "SPI.h"
/*
 *
 */
void delay(u32 x){while(x--);}

void init(){
//    ANSELA = 0x00;
    TRISC = 0x10;
    TRISD = 0x00;
}

int main(int argc, char** argv) {
    init();
    LCD_init();

    Write_Byte('A',21);delay(500);
    Write_Byte('N',22);delay(500);
    Write_Byte('K',23);
   // Write_Byte('I',24);
   // Write_Byte('T',25);
   // Write_Byte(' ',26);
    while(1){
        LCD_func(Read_Byte(21),DTA);
        LCD_func(Read_Byte(22),DTA);
       // LCD_func(Read_Byte(23),DTA);
       // LCD_func(Read_Byte(24),DTA);
       // LCD_func(Read_Byte(25),DTA);
       // LCD_func(Read_Byte(26),DTA);

        delay(500000);
    }

    return (EXIT_SUCCESS);
}
