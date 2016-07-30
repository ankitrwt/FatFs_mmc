/* 
 * File:   LCD.c
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:32 PM
 */
#include "LCD.h"
#include <pic18f4515.h>
//#include <pic.h>

void LCD_delay(u16 x){while(x--);}

void LCD_enable(){
//    en = 1;
    PORTD |= 0x20;
    LCD_delay(500);
//    en = 0;
    PORTD &= ~0x20;
    LCD_delay(500);
}

void LCD_init(){
    LCD_func(0x01,CMD);     //clear display screen
    LCD_func(0x02,CMD);     //return home
    //LCD_func(0x05,CMD);   //shift display right
    LCD_func(0x06,CMD);     //increament cursor
    //LCD_func(0x07,CMD);   //shift display left
    //LCD_func(0x08,CMD);   //display off cursor off
    //LCD_func(0x0a,CMD);   //display off cursor on
    //LCD_func(0x0e,CMD);   //display on cursor blinking
    //LCD_func(0x10,CMD);   //sift cursor position to left
    //LCD_func(0x14,CMD);   //sift cursor position to right
    LCD_func(0x0c,CMD);     //display ON cursor off
    LCD_func(0x28,CMD);     //using 2 lines, 5x7 matrix 4 bit mode
    //LCD_func(0x38,CMD);   //using 2 lines, 5x7 matrix 8 bit mode
    LCD_func(0x80,CMD);     //set cursor at beginning of first line
    //LCD_func(0xc0,CMD);   //set cursor at beginning of second line
}

void LCD_func(u8 data, u8 cmd){
//    rs = (bit)cmd;
    PORTD = cmd? PORTD|0x10:PORTD&~0x10;
    LCD(data>>4);
    LCD_enable();

    LCD(data&0x0F);
    LCD_enable();
}

void LCD_cur(u8 row,u8 col){
    LCD_func(((0x80 + 0x40 * (row - 1)) + (col - 1)),CMD);
}

void LCD_string(u8 *ptr){
    while(*ptr != '\0'){
        LCD_func(*ptr,DTA);
        ptr++;
    }
}