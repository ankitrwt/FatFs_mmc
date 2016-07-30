//**************************************************************
// ****** FUNCTIONS FOR SD RAW DATA TRANSFER *******
//**************************************************************
//Controller: ATmega32 (Clock: 8 Mhz-internal)
//Compiler	: AVR-GCC (winAVR with AVRStudio)
//Project V.: Version - 2.4.1
//**************************************************************
//#include <pic.h>
#include "stm32f0xx_hal.h"
#include "SD_routines.h"
#include "integer.h"
#include "diskio.h"

void SD_delay(uint32_t x){while(x--);}

#define DO_INIT()							/* Initialize port for MMC DO as input */
#define DO			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)	/* Test for MMC DO ('H':true, 'L':false) */

#define DI_INIT()								/* Initialize port for MMC DI as output */
#define DI_H()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)	/* Set MMC DI "high" */
#define DI_L()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)	/* Set MMC DI "low" */

#define CK_INIT()								/* Initialize port for MMC SCLK as output */
#define CK_H()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)	/* Set MMC SCLK "high" */
#define	CK_L()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)	/* Set MMC SCLK "low" */

#define CS_INIT()								/* Initialize port for MMC CS as output */
#define	CS_H()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)	/* Set MMC CS "high" */
#define CS_L()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)	/* Set MMC CS "low" */


#define SD_CS_ASSERT CS_H() 
#define SD_CS_DEASSERT CS_L() 

volatile unsigned long startBlock, totalBlocks;
volatile unsigned char SDHC_flag, cardType;//, buffer[512];
//******************************************************************

static
void dly_us (UINT n)	/* Delay n microseconds (avr-gcc -Os) */
{
	do {
			HAL_Delay(50);
		/*
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#if F_CPU >= 6000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 7000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 8000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 9000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 10000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 12000000
		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
#endif
#if F_CPU >= 14000000
#error Too fast clock
#endif*/
	} while (--n);
}



/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* MMC/SD command (SPI mode) */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define CMD13	(13)		/* SEND_STATUS */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */


static
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static
BYTE CardType;			/* b0:MMC, b1:SDv1, b2:SDv2, b3:Block addressing */



//******************************************************************
//Function	: to initialize the SD/SDHC card in SPI mode
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
/*-----------------------------------------------------------------------*/
/* Transmit bytes to the card (bitbanging)                               */
/*-----------------------------------------------------------------------*/

static
void xmit_mmc (
	const BYTE* buff,	/* Data to be sent */
	UINT bc				/* Number of bytes to send */
)
{
	BYTE d;


	do {
		d = *buff++;	/* Get a byte to be sent */
		if (d & 0x80) DI_H(); else DI_L();	/* bit7 */
		CK_H(); CK_L();
		if (d & 0x40) DI_H(); else DI_L();	/* bit6 */
		CK_H(); CK_L();
		if (d & 0x20) DI_H(); else DI_L();	/* bit5 */
		CK_H(); CK_L();
		if (d & 0x10) DI_H(); else DI_L();	/* bit4 */
		CK_H(); CK_L();
		if (d & 0x08) DI_H(); else DI_L();	/* bit3 */
		CK_H(); CK_L();
		if (d & 0x04) DI_H(); else DI_L();	/* bit2 */
		CK_H(); CK_L();
		if (d & 0x02) DI_H(); else DI_L();	/* bit1 */
		CK_H(); CK_L();
		if (d & 0x01) DI_H(); else DI_L();	/* bit0 */
		CK_H(); CK_L();
	} while (--bc);
}



/*-----------------------------------------------------------------------*/
/* Receive bytes from the card (bitbanging)                              */
/*-----------------------------------------------------------------------*/

static
void rcvr_mmc (
	BYTE *buff,	/* Pointer to read buffer */
	UINT bc		/* Number of bytes to receive */
)
{
	BYTE r;


	DI_H();	/* Send 0xFF */

	do {
		r = 0;	 if (DO) r++;	/* bit7 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit6 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit5 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit4 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit3 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit2 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit1 */
		CK_H(); CK_L();
		r <<= 1; if (DO) r++;	/* bit0 */
		CK_H(); CK_L();
		*buff++ = r;			/* Store a received byte */
	} while (--bc);
}
//******************************************************************
uint8_t a,b,c;

unsigned char SD_init(void)
{
unsigned char i, response, SD_version;
unsigned int retry=0 ;
unsigned char d;

d = 0xFF;	
 for(i=0;i<10;i++)
      xmit_mmc(&d,1);   //80 clock pulses spent before sending the first command

	a = 1;
SD_CS_ASSERT;
do
{
   b = response = SD_sendCommand(GO_IDLE_STATE, 0); //send 'reset & go idle' command
   retry++;  
   if(retry>0x20)
	 {
		  c = 1;
   	  return 1;   //time out, card not detected
	 } 
}while(response != 0x01);


while(1);
SD_CS_DEASSERT;
d = 0xff;
xmit_mmc(&d,1);
xmit_mmc(&d,1);

retry = 0;

SD_version = 2; //default set to SD compliance with ver2.x;
				//this may change after checking the next command
do
{
response = SD_sendCommand(SEND_IF_COND,0x000001AA); //Check power supply status, mendatory for SDHC card
retry++;
if(retry>0xfe)
   {
	//  TX_NEWLINE;
	  SD_version = 1;
	  cardType = 1;
	  break;
   } //time out

}while(response != 0x01);

retry = 0;

do
{
response = SD_sendCommand(APP_CMD,0); //CMD55, must be sent before sending any ACMD command
response = SD_sendCommand(SD_SEND_OP_COND,0x40000000); //ACMD41

retry++;
if(retry>0xfe)
   {
      //TX_NEWLINE;
	  return 2;  //time out, card initialization failed
   }

}while(response != 0x04);

retry = 0;
SDHC_flag = 0;

if (SD_version == 2)
{
   do
   {
	 response = SD_sendCommand(READ_OCR,0);
	 retry++;
	 if(retry>0xfe)
     {
       //TX_NEWLINE;
	   cardType = 0;
	   break;
     } //time out

   }while(response != 0x00);

   if(SDHC_flag == 1) cardType = 2;
   else cardType = 3;
}

//SD_sendCommand(CRC_ON_OFF, OFF); //disable CRC; deafault - CRC disabled in SPI mode
//SD_sendCommand(SET_BLOCK_LEN, 512); //set block size to 512; default size is 512


return 0; //successful return
}

//******************************************************************
//Function	: to send a command to SD card
//Arguments	: unsigned char (8-bit command value)
// 			  & unsigned long (32-bit command argument)
//return	: unsigned char; response byte
//******************************************************************
unsigned char SD_sendCommand(unsigned char cmd, unsigned long arg)
{
unsigned char response, retry=0, status,d;

//SD card accepts byte address while SDHC accepts block address in multiples of 512
//so, if it's SD card we need to convert block address into corresponding byte address by 
//multipying it with 512. which is equivalent to shifting it left 9 times
//following 'if' loop does that
/*
if(SDHC_flag == 0)		
if(cmd == READ_SINGLE_BLOCK     ||
   cmd == READ_MULTIPLE_BLOCKS  ||
   cmd == WRITE_SINGLE_BLOCK    ||
   cmd == WRITE_MULTIPLE_BLOCKS ||
   cmd == ERASE_BLOCK_START_ADDR|| 
   cmd == ERASE_BLOCK_END_ADDR ) 
   {
     arg = arg << 9;
   }	   
*/
SD_CS_ASSERT;
d = cmd | 0x40;
xmit_mmc(&d,1); //send command, first two bits always '01'
d = arg>>24;
xmit_mmc(&d,1);
d = arg>>16;
xmit_mmc(&d,1);
d = arg>>8;
xmit_mmc(&d,1);
d = arg;
xmit_mmc(&d,1);

if(cmd == SEND_IF_COND){	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
  d = 0x87;
	xmit_mmc(&d,1);    //for remaining commands, CRC is ignored in SPI mode
}else {
	d = 0x95;
  xmit_mmc(&d,1);
}
rcvr_mmc(&d,1);
//while((response = SPI_Read_8Bits()) == 0xff) //wait response
//   if(retry++ > 0xfe) break; //time out error
do{
    rcvr_mmc(&response,1);
//    response = SPI_Read_8Bits();
   if(retry++ > 0xfe) break; //time out error
}while(response == 0xff); //wait response


if(response == 0x00 && cmd == 58)  //checking response of CMD58
{
	rcvr_mmc(&d,1);
  status = d & 0x40;     //first byte of the OCR register (bit 31:24)
  if(status == 0x40) SDHC_flag = 1;  //we need it to verify SDHC card
  else SDHC_flag = 0;

  rcvr_mmc(&d,1); //remaining 3 bytes of the OCR register are ignored here
  rcvr_mmc(&d,1); //one can use these bytes to check power supply limits of SD
  rcvr_mmc(&d,1);
}

//SPI_Read_8Bits(); //extra 8 CLK
//SPI_Read_8Bits(); //extra 8 CLK
SD_CS_DEASSERT;
return response; //return state
}

//*****************************************************************
//Function	: to erase specified no. of blocks of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//*****************************************************************
/*unsigned char SD_erase (unsigned long startBlock, unsigned long totalBlocks)
{
unsigned char response;

response = SD_sendCommand(ERASE_BLOCK_START_ADDR, startBlock); //send starting block address
if(response != 0x00) //check for SD status: 0x00 - OK (No flags set)
  return response;

response = SD_sendCommand(ERASE_BLOCK_END_ADDR,(startBlock + totalBlocks - 1)); //send end block address
if(response != 0x00)
  return response;

response = SD_sendCommand(ERASE_SELECTED_BLOCKS, 0); //erase all selected blocks
if(response != 0x00)
  return response;

return 0; //normal return
}

//******************************************************************
//Function	: to read a single block from SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char SD_readSingleBlock(unsigned long startBlock)
{
unsigned char response;
unsigned int i, retry=0;

 response = SD_sendCommand(READ_SINGLE_BLOCK, startBlock); //read a Block command
 if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;
retry = 0;
//while(SPI_Read_8Bits() != 0xfe) //wait for start block token 0xfe (0x11111110)
//  if(retry++ > 0xfe){SD_CS_DEASSERT; return 1;} //return if time-out

for(i=0; i<512; i++) //read 512 bytes
  buffer[i] = SPI_Read_8Bits();

SPI_Read_8Bits(); //receive incoming CRC (16-bit), CRC is ignored here
SPI_Read_8Bits();

SPI_Read_8Bits(); //extra 8 clock pulses
SD_CS_DEASSERT;

return 0;
}

//******************************************************************
//Function	: to write to a single block of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char SD_writeSingleBlock(unsigned long startBlock)
{
unsigned char response;
unsigned int i, retry=0;

 response = SD_sendCommand(WRITE_SINGLE_BLOCK, startBlock); //write a Block command
  
 if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;

SPI_Send_8Bits(0xfe);     //Send start block token 0xfe (0x11111110)

for(i=0; i<512; i++)    //send 512 bytes data
  SPI_Send_8Bits('A');

SPI_Send_8Bits(0xff);     //transmit dummy CRC (16-bit), CRC is ignored here
SPI_Send_8Bits(0xff);

//do{
//response = SPI_Read_8Bits();
//}while((response & 0x0f) != 0x05);

if( (response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
{                              //AAA='101'-data rejected due to CRC error
  SD_CS_DEASSERT;              //AAA='110'-data rejected due to write error
  return response;
}
while(1);
while(!SPI_Read_8Bits()) //wait for SD card to complete writing and get idle
if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}

SD_CS_DEASSERT;
SPI_Send_8Bits(0xff);   //just spend 8 clock cycle delay before reasserting the CS line
SD_CS_ASSERT;         //re-asserting the CS line to verify if card is still busy

while(!SPI_Read_8Bits()) //wait for SD card to complete writing and get idle
   if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}
SD_CS_DEASSERT;

return 0;
}


#ifndef FAT_TESTING_ONLY

//***************************************************************************
//Function	: to read multiple blocks from SD card & send every block to UART
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//****************************************************************************
unsigned char SD_readMultipleBlock (unsigned long startBlock, unsigned long totalBlocks)
{
unsigned char response;
unsigned int i, retry=0;

retry = 0;

response = SD_sendCommand(READ_MULTIPLE_BLOCKS, startBlock); //write a Block command
  
if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;

while( totalBlocks )
{
  retry = 0;
  while(SPI_receive() != 0xfe) //wait for start block token 0xfe (0x11111110)
  if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;} //return if time-out

  for(i=0; i<512; i++) //read 512 bytes
    buffer[i] = SPI_receive();

  SPI_receive(); //receive incoming CRC (16-bit), CRC is ignored here
  SPI_receive();

  SPI_receive(); //extra 8 cycles
  TX_NEWLINE;
  transmitString_F(PSTR(" --------- "));
  TX_NEWLINE;

  for(i=0; i<512; i++) //send the block to UART
  {
    if(buffer[i] == '~') break;
    transmitByte ( buffer[i] );
  }

  TX_NEWLINE;
  transmitString_F(PSTR(" --------- "));
  TX_NEWLINE;
  totalBlocks--;
}

SD_sendCommand(STOP_TRANSMISSION, 0); //command to stop transmission
SD_CS_DEASSERT;
SPI_receive(); //extra 8 clock pulses

return 0;
}

//***************************************************************************
//Function: to receive data from UART and write to multiple blocks of SD card
//Arguments: none
//return: unsigned char; will be 0 if no error,
// otherwise the response byte will be sent
//****************************************************************************
unsigned char SD_writeMultipleBlock(unsigned long startBlock, unsigned long totalBlocks)
{
unsigned char response, data;
unsigned int i, retry=0;
unsigned long blockCounter=0, size;

response = SD_sendCommand(WRITE_MULTIPLE_BLOCKS, startBlock); //write a Block command

if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;

TX_NEWLINE;
transmitString_F(PSTR(" Enter text (End with ~): "));
TX_NEWLINE;

while( blockCounter < totalBlocks )
{
   i=0;
   do
   {
     data = receiveByte();
     if(data == 0x08)	//'Back Space' key pressed
	 { 
	   if(i != 0)
	   { 
	     transmitByte(data);
	     transmitByte(' '); 
	     transmitByte(data); 
	     i--; 
		 size--;
	   } 
	   continue;     
	 }
     transmitByte(data);
     buffer[i++] = data;
     if(data == 0x0d)
     {
        transmitByte(0x0a);
        buffer[i++] = 0x0a;
     }
	 if(i == 512) break;
   }while (data != '~');

   TX_NEWLINE;
   transmitString_F(PSTR(" ---- "));
   TX_NEWLINE;

   SPI_transmit(0xfc); //Send start block token 0xfc (0x11111100)

   for(i=0; i<512; i++) //send 512 bytes data
     SPI_transmit( buffer[i] );

   SPI_transmit(0xff); //transmit dummy CRC (16-bit), CRC is ignored here
   SPI_transmit(0xff);

   response = SPI_receive();
   if( (response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
   {                              //AAA='101'-data rejected due to CRC error
      SD_CS_DEASSERT;             //AAA='110'-data rejected due to write error
      return response;
   }

   while(!SPI_receive()) //wait for SD card to complete writing and get idle
     if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}

   SPI_receive(); //extra 8 bits
   blockCounter++;
}

SPI_transmit(0xfd); //send 'stop transmission token'

retry = 0;

while(!SPI_receive()) //wait for SD card to complete writing and get idle
   if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}

SD_CS_DEASSERT;
SPI_transmit(0xff); //just spend 8 clock cycle delay before reasserting the CS signal
SD_CS_ASSERT; //re assertion of the CS signal is required to verify if card is still busy

while(!SPI_receive()) //wait for SD card to complete writing and get idle
   if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}
SD_CS_DEASSERT;

return 0;
}
//*********************************************
*/
//#endif

//******** END ********************************
