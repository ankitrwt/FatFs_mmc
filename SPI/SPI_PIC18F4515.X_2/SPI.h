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

#define SCK_HIGH  (PORTC |= 0x08)
#define SCK_LOW  (PORTC &= ~0x08)

#define nCS_HIGH  (PORTC |= 0x40)
#define nCS_LOW  (PORTC &= ~0x40)

#define SDO_HIGH  (PORTC |= 0x20)
#define SDO_LOW  (PORTC &= ~0x20)

//#define SDI    (PORTC & 0x10)

#define nWP_HIGH  (PORTC |= 0x04)
#define nWP_LOW  (PORTC &= ~0x04)

#define nHLD_HIGH  (PORTC |= 0x80)
#define nHLD_LOW  (PORTC &= ~0x80)

u8 SPI_Read_8Bits();
void SPI_Send_8Bits(u8 data);

//void Write_Byte(u8 data,u8 address);
//u8 Read_Byte(u8 address);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

