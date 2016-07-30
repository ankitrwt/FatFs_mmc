/* 
 * File:   SPI.h
 * Author: Ankit
 *
 * Created on April 30, 2016, 1:26 AM
 */

#ifndef SPI_EE_H
#define	SPI_EE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

#define SCK_HIGH  (PORTB |= 0x80)
#define SCK_LOW  (PORTB &= ~0x80)

#define nCS_HIGH  (PORTB |= 0x10)
#define nCS_LOW  (PORTB &= ~0x10)

#define SDO_HIGH  (PORTB |= 0x40)
#define SDO_LOW  (PORTB &= ~0x40)

//#define SDI    (PORTB & 0x10)

#define nWP_HIGH  (PORTB |= 0x08)
#define nWP_LOW  (PORTB &= ~0x08)

#define nHLD_HIGH  (PORTB |= 0x04)
#define nHLD_LOW  (PORTB &= ~0x4)


void Write_Byte(u8 data,u8 address);
u8 Read_Byte(u8 address);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

