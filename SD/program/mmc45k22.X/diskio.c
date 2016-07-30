/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "mcc_generated_files/mcc.h"	/* Example: Header file of existing USB MSD control module */
#ifdef _XC16

#endif
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
//#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
//#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define USB		2	/* Example: Map USB MSD to physical drive 2 */


//---------------------------------------------------------------------------*/

/* Definitions for MMC/SDC command */
#define CMD0   (0)			/* GO_IDLE_STATE */
#define CMD1   (1)			/* SEND_OP_COND */
#define ACMD41 (41|0x80)	/* SEND_OP_COND (SDC) */
#define CMD8   (8)			/* SEND_IF_COND */
#define CMD9   (9)			/* SEND_CSD */
#define CMD10  (10)			/* SEND_CID */
#define CMD12  (12)			/* STOP_TRANSMISSION */
#define ACMD13 (13|0x80)	/* SD_STATUS (SDC) */
#define CMD16  (16)			/* SET_BLOCKLEN */
#define CMD17  (17)			/* READ_SINGLE_BLOCK */
#define CMD18  (18)			/* READ_MULTIPLE_BLOCK */
#define CMD23  (23)			/* SET_BLOCK_COUNT */
#define ACMD23 (23|0x80)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)			/* WRITE_BLOCK */
#define CMD25  (25)			/* WRITE_MULTIPLE_BLOCK */
#define CMD41  (41)			/* SEND_OP_COND (ACMD) */
#define CMD55  (55)			/* APP_CMD */
#define CMD58  (58)			/* READ_OCR */

#define CT_MMC 0x01
#define CT_SD1 0x02
#define CT_SD2 0x04
#define CT_SDC (CT_SD1|CT_SD2)
#define CT_BLOCK 0x08


static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */


static
BYTE CardType;


/*-----------------------------------------------------------------------*/
/* wait for card ready                                                     */
/*-----------------------------------------------------------------------*/
static
BYTE wait_ready (void)
{
	BYTE tmr;

        for(tmr = 5000; tmr; tmr--){
            if(sd_rx() == 0xFF) break;
            __delay_us(100);
        }

        return tmr ? 1 : 0;
}
/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                                    */
/*-----------------------------------------------------------------------*/

static
void deselect(void)
{
    SD_CS_SetHigh();
    sd_rx();
}

/*-----------------------------------------------------------------------*/
/* Select the card and wait for ready                                                        */
/*-----------------------------------------------------------------------*/
static
BYTE select(void)
{
    SD_CS_SetLow();
    sd_rx();
    if(wait_ready()) return 1;

    deselect();
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock (	/* 1:OK, 0:Failed */
	BYTE *buff,			/* Data buffer to store received data */
	UINT btr			/* Byte count (must be multiple of 4) */
)
{
	BYTE token;
        UINT tmr;

	Timer1 = 100;
	do {							/* Wait for data packet in timeout of 100ms */
		token = xchg_spi(0xFF);
	} while ((token == 0xFF) && Timer1);

	if(token != 0xFE) return 0;		/* If not valid data token, retutn with error */

	rcvr_spi_multi(buff, btr);		/* Receive the data block into buffer */
	xchg_spi(0xFF);					/* Discard CRC */
	xchg_spi(0xFF);

	return 1;						/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
static
int xmit_datablock (	/* 1:OK, 0:Failed */
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data token */
)
{
	BYTE resp;


	if (!wait_ready()) return 0;

	xchg_spi(token);		/* Xmit a token */
	if (token != 0xFD) {	/* Not StopTran token */
		xmit_spi_multi(buff, 512);	/* Xmit the data block to the MMC */
		xchg_spi(0xFF);				/* CRC (Dummy) */
		xchg_spi(0xFF);
		resp = xchg_spi(0xFF);		/* Receive a data response */
		if ((resp & 0x1F) != 0x05)	/* If not accepted, return with error */
			return 0;
	}

	return 1;
}
#endif



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		deselect();
		if (!select()) return 0xFF;
	}

	/* Send command packet */
	xchg_spi(0x40 | cmd);			/* Start + Command index */
	xchg_spi((BYTE)(arg >> 24));	/* Argument[31..24] */
	xchg_spi((BYTE)(arg >> 16));	/* Argument[23..16] */
	xchg_spi((BYTE)(arg >> 8));		/* Argument[15..8] */
	xchg_spi((BYTE)arg);			/* Argument[7..0] */
	n = 0x01;						/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;		/* Valid CRC for CMD0(0) + Stop */
	if (cmd == CMD8) n = 0x87;		/* Valid CRC for CMD8(0x1AA) + Stop */
	xchg_spi(n);

	/* Receive command response */
	if (cmd == CMD12) xchg_spi(0xFF);	/* Skip a stuff byte on stop to read */
	n = 10;							/* Wait for a valid response in timeout of 10 attempts */
	do
		res = xchg_spi(0xFF);
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}


#endif
