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


#define SCK_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define SCK_LOW  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)

#define nCS_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define nCS_LOW  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)

#define SDO_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)
#define SDO_LOW  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)

#define SDI    HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)

//#define nWP_HIGH  (PORTC |= 0x04)
//#define nWP_LOW  (PORTC &= ~0x04)

//#define nHLD_HIGH  (PORTC |= 0x80)
//#define nHLD_LOW  (PORTC &= ~0x80)

u8 SPI_Read_8Bits(void);
void SPI_Send_8Bits(u8 data);

//void Write_Byte(u8 data,u8 address);
//u8 Read_Byte(u8 address);

#ifdef	__cplusplus
}
#endif


#endif	/* SPI_H */



