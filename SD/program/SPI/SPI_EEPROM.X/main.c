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
#include "SD_routines.h"
#include "FAT32.h"

/*
 *
 *
 */
void delay(u32 x){while(x--);}

void init(){
//    ANSELA = 0x00;
    TRISC = 0x10;
    TRISD = 0x00;
}

int main(int argc, char** argv) {

unsigned char option, error, data, FAT32_active;
unsigned int i;
unsigned char fileName[13];
    init();
    LCD_init();

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
LCD_string("...SD/MMC card...");
while(1);/*
switch (cardType)
{
  case 1:LCD_string("Standard Capacity Card (Ver 1.x) Detected!");
  		 break;
  case 2:LCD_string("High Capacity Card Detected!");
  		 break;
  case 3:LCD_string("Standard Capacity Card (Ver 2.x) Detected!");
  		 break;
  default:LCD_string("Unknown SD Card Detected!");
  		 break;
}


//SPI_HIGH_SPEED;	//SCK - 4 MHz
//_delay_ms(1);   //some delay


FAT32_active = 1;
error = getBootSectorData (); //read boot sector and keep necessary data in global variables
if(error)
{
//  TX_NEWLINE;
  LCD_string("FAT32 not found!");  //FAT32 incompatible drive
  FAT32_active = 0;
}

while(1)
{/*
LCD_string("Press any key...");
//option = receiveByte();
LCD_string("> 0: Erase Blocks ");
LCD_string("> 1: Write single Block \n2: Read single Block");

#ifndef FAT_TESTING_ONLY
TX_NEWLINE;
transmitString_F(PSTR("> 3: Write multiple Blocks       4: Read multiple Blocks"));
#endif

TX_NEWLINE;
transmitString_F(PSTR("> 5: Get file list               6: Read File"));
TX_NEWLINE;
transmitString_F(PSTR("> 7: Write File                  8: Delete File"));
TX_NEWLINE;
transmitString_F(PSTR("> 9: Read SD Memory Capacity     a: Show Date & Time"));

TX_NEWLINE;
transmitString_F(PSTR("> b: Update Date                 c: Update Time"));
TX_NEWLINE;

TX_NEWLINE;
TX_NEWLINE;
transmitString_F(PSTR("> Select Option (0-9/a/b/c): "));
*/

/*WARNING: If option 0, 1 or 3 is selected, the card data may not be detected by PC/Laptop again,
as it may disturb the FAT format. In such a case you will need to format the card again with FAT32.
This options are given for learnig the raw data transfer to & from the SD Card*/
/*
option = 2;//receiveByte();
//transmitByte(option);

if(option >=0x35 && option <=0x39)  //options 5 to 9 disabled if FAT32 not found
{
  if(!FAT32_active)
  {
  //  TX_NEWLINE;
   // TX_NEWLINE;
	LCD_string("FAT32 options disable!");
	continue;
  }
}


if((option >= 0x30) && (option <=0x34)) //get starting block address for options 0 to 4
{
//TX_NEWLINE;
//TX_NEWLINE;
LCD_string("Enter the Block number (0000-9999):");
data = 1;//receiveByte(); transmitByte(data);
startBlock = (data & 0x0f) * 1000;
data = 2;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 100;
data = 3;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 10;
data = 4;//receiveByte(); transmitByte(data);
startBlock += (data & 0x0f);
//TX_NEWLINE;
}

totalBlocks = 1;

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
          //TX_NEWLINE;
          if(error)
            LCD_string("Read failed..");
          else
          {
            for(i=0;i<512;i++)
            {
                if(buffer[i] == '~') break;
                //transmitByte(buffer[i]);
            }
           //TX_NEWLINE;
           // TX_NEWLINE;
            LCD_string("Read successful!");
          }

          break;
//next two options will work only if following macro is cleared from SD_routines.h
#ifndef FAT_TESTING_ONLY

case '3':
          error = SD_writeMultipleBlock (startBlock, totalBlocks);
          TX_NEWLINE;
          if(error)
            transmitString_F(PSTR("Write failed.."));
          else
            transmitString_F(PSTR("Write successful!"));
          break;

case '4': error = SD_readMultipleBlock (startBlock, totalBlocks);
          TX_NEWLINE;
          if(error)
            transmitString_F(PSTR("Read failed.."));
          else
            transmitString_F(PSTR("Read successful!"));
          break;
#endif

case '5': //TX_NEWLINE;
  		  findFiles(GET_LIST,0);
          break;

case '6':
case '7':
case '8': //TX_NEWLINE;
	//	  TX_NEWLINE;
          LCD_string("Enter file name: ");
          for(i=0; i<13; i++)
			fileName[i] = 0x00;   //clearing any previously stored file name
          i=0;
          while(1)
          {
            data = 3;//receiveByte();
            if(data == 0x0d) break;  //'ENTER' key pressed
			if(data == 0x08)	//'Back Space' key pressed
	 		{
	   			if(i != 0)
	   			{
	     			//transmitByte(data);
				//	transmitByte(' ');
	     			//transmitByte(data);
	     			i--;
	   			}
	   			continue;
	 		}
			if(data <0x20 || data > 0x7e) continue;  //check for valid English text character
			//transmitByte(data);
            fileName[i++] = data;
            if(i==13){LCD_string(" file name too long.."); break;}
          }
          if(i>12) break;

	     // TX_NEWLINE;
		  if(option == '6')
		     readFile( READ, fileName);
		  if(option == '7')
		  	 writeFile(fileName);
 		  if(option == '8')
		     deleteFile(fileName);
          break;

case '9': memoryStatistics();
          break;

case 'a':
case 'A': //RTC_displayDate();
	//	  RTC_displayTime();
		  break;
case 'b':
case 'B': //RTC_updateDate();
		  break;
case 'c':
case 'C': //RTC_updateTime();
	      break;

default: //TX_NEWLINE;
         //TX_NEWLINE;
         LCD_string(" Invalid option!");
         //TX_NEWLINE;
}

//TX_NEWLINE;
}*/
    return (EXIT_SUCCESS);
}
