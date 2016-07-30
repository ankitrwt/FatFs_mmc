/* 
 * File:   newmain.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pic18f458.h>
#include "LCD.h"
/*
 * 
 */
void delay(u32 x){while(x--);}

void init(){
    TRISD = 0x00;
}

int main(int argc, char** argv) {
    init();
    LCD_init();

    while(1){
        LCD_string("ANKIT");
        delay(500000);
    }

    return (EXIT_SUCCESS);
}

