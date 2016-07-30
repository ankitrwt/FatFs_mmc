/* 
 * File:   LCD.h
 * Author: Ankit
 *
 * Created on April 9, 2016, 6:33 PM
 */

#ifndef LCD_H
#define	LCD_H

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

#define en RA5
#define rs RA4

#define LCD(x) (PORTA = (PORTA&0xF0)|x)

//#define D4 RD0
//#define D5 RD1
//#define D6 RD2
//#define D7 RD3

#define CMD 0
#define DTA 1

void LCD_func(u8 data,u8 cmd);
void LCD_init();
void LCD_cur(u8 row,u8 col);
void LCD_string(u8 *ptr);


#endif	/* LCD_H */

