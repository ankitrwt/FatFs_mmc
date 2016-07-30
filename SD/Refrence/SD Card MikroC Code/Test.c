/*..............................................................
SD CARD PROJECT
===============
In this project a SD card is connected to PORTC as follows:
CS RC2
CLK RC3
DO RC4
DI RC5
The program opens a file called Test.TXT on the SD card
and writes the string "Writing to SD Card..." to this file.
Author: studentcompanion.co.za
Date: July 2013
File: Test.C
..............................................................*/
char filename[] = "Test.TXT";
unsigned char txt[] = "Writing to SD Card...";

// MMC module connections
sbit Mmc_Chip_Select           at LATC2_bit;  // for writing to output pin always use latch (PIC18 family)
sbit Mmc_Chip_Select_Direction at TRISC2_bit;


void main() 
{
  ANSELC = 0;                      // Configure AN pins as digital
  SLRCON = 0;                      // Configure all PORTS at the standard Slew Rate
//
// Initialise the SPI bus
//
SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV16,_SPI_DATA_SAMPLE_MIDDLE,
_SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);

// use fat16 quick format instead of init routine if a formatting is needed
  if (Mmc_Fat_Init() == 0) {
    // reinitialize spi at higher speed
    SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
    
    Mmc_Fat_Assign(&filename, 0x80);          // Find existing file or create a new one
    Mmc_Fat_Rewrite();

    Mmc_Fat_Write(txt,21);
    }

    for(;;);
}