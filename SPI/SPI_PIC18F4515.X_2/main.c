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
//#include <pic.h>
#include "LCD.h"
#include "SPI.h"
//#include "SPI_EE.h"
#include "SD_routines.h"


#define READ    (PORTD & 0x80)
#define WRITE   (PORTD & 0x40)
#define ERASE   (PORTC & 0x80)
//#include "FAT32.h"

/*
 *
 *
 */
void delay(u32 x){while(x--);}

void init(){
//    ANSELA = 0x00;
    TRISC = 0x10;
    TRISD = 0xC0;
    TRISB = 0x20;
}

int main(int argc, char** argv) {

unsigned char option, error, data, FAT32_active;
unsigned int i;
unsigned char fileName[13];
    init();
    LCD_init();

    delay(5000);

cardType = 0;


for (i=0; i<10; i++)
{
  error = SD_init();
  if(!error) break;
}

if(error)
{
  if(error == 1) LCD_string("Crd not detected");
  if(error == 2) LCD_string("Crd Init failed.");

  while(1);  //wait here forever if error in SD init
}
//LCD_string("...SD/MMC card...");

switch (cardType)
{
  case 1:LCD_string("Std Cap Ver 1.x");
  		 break;
  case 2:LCD_string("High Cap Dtect!");
  		 break;
  case 3:LCD_string("Std Cap Ver 2.x");
  		 break;
  default:LCD_string("Unknown SD Card");
  		 break;
}

//SPI_HIGH_SPEED;	//SCK - 4 MHz
//_delay_ms(1);   //some delay
LCD_cur(1,1);
LCD_func('A',DTA);
FAT32_active = 1;
error = getBootSectorData (); //read boot sector and keep necessary data in global variables
if(error)
{
  LCD_func('Z',DTA);
//  TX_NEWLINE;
  LCD_string("FAT32 not found!");  //FAT32 incompatible drive
  FAT32_active = 0;
  while(1);
}


delay(50000);delay(50000);
delay(50000);delay(50000);
LCD_cur(1,1);
startBlock = 0x00000000;
totalBlocks = 1;
strcpy(buffer,"THIS IS JUST A TEST    ~");
/*
while(1)
{
            error = SD_writeSingleBlock (startBlock);
            if(error)
                LCD_string("Write failed..");
            else
                LCD_string("Write successful!");
   while(1);
}
*/

while(1)
{
    error = SD_readSingleBlock (startBlock);

    if(error)
     LCD_string("Read failed..");
    else
     LCD_string("Read successful!");

    while(1);
}

while(1)
{
/*
WARNING: If option 0, 1 or 3 is selected, the card data may not be detected by PC/Laptop again,
as it may disturb the FAT format. In such a case you will need to format the card again with FAT32.
This options are given for learnig the raw data transfer to & from the SD Card
*/
// Select option
/*if(READ){
    while(READ);
    option = 2 + '2';
}
else if(WRITE){
    while(WRITE);
    option = 1 + '1';
}
else if(ERASE){
    while(ERASE);
    option = 0 + '0';
}
*/

/*
if((option >= 0x30) && (option <=0x34)) //get starting block address for options 0 to 4
{
data = 3;
startBlock = (data & 0x0f) * 1000;
data = 2;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 100;
data = 3;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 10;
data = 4;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f);
//TX_NEWLINE;
}
*/

/*
#ifndef FAT_TESTING_ONLY

if((option == 0x30) || (option == 0x33) || (option == 0x34)) //get total number of blocks for options 0, 3 or 4
{
TX_NEWLINE;
TX_NEWLINE;
transmitString_F(PSTR("How many blocks? (000-999):"));
data = receiveByte(); transmitByte(data);
totalBlocks = (data & 0x0f) * 100;
data = receiveByte(); transmitByte(data);
totalBlocks += (data & 0x0f) * 10;
data = receiveByte(); transmitByte(data);
totalBlocks += (data & 0x0f);
TX_NEWLINE;
}
#endif
*/
switch (option)
{
case '0': //error = SD_erase (block, totalBlocks);
          error = SD_erase (startBlock, totalBlocks);
         // TX_NEWLINE;
          if(error)
             LCD_string("Erase failed..");
          else
              LCD_string("Erased!");
          break;

case '1': //TX_NEWLINE;
          LCD_string(" Enter text (End with ~):");
          i=0;
            do
            {
                data = 1;//receiveByte();
                //transmitByte(data);
                buffer[i++] = data;
                if(data == 0x0d)
                {
                    //transmitByte(0x0a);
                    buffer[i++] = 0x0a;
                }
                if(i == 512) break;
            }while (data != '~');

            error = SD_writeSingleBlock (startBlock);
           // TX_NEWLINE;
            //TX_NEWLINE;
            if(error)
                LCD_string("Write failed..");
            else
                LCD_string("Write successful!");
            break;

case '2': error = SD_readSingleBlock (startBlock);
          if(error)
            LCD_string("Read failed..");
          else
          {
           // for(i=0;i<512;i++)
           // {
           //     if(buffer[i] == '~') break;
           // }
            LCD_string("Read successful!");
           // while(1);
          }
while(1);
          break;
default:
         LCD_string(" Invalid option!");
}

}
    return (EXIT_SUCCESS);
}
