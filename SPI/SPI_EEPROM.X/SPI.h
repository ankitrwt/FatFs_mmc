/* 
 * File:   SPI.h
 * Author: Ankit
 *
 * Created on April 30, 2016, 1:26 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

#define SCK  RC3
#define nCS  RC6
#define SDO  RC5
#define SDI  RC4
#define nWP  RC2
#define nHLD RC7

void Write_Byte(u8 data,u8 address);
u8 Read_Byte(u8 address);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

