#ifndef __I2C_H__
#define __I2C_H__

//===================== EEPROM(24C04) PIN CONFIGURATION =======================//

#include "stm32f0xx_hal.h"

#define WP_PIN				GPIO_PIN_3 //9   EE Pin No 7
#define SCL_PIN       GPIO_PIN_8 //10  EE Pin No 6
#define SDA_PIN				GPIO_PIN_9 //11  EE Pin No 5

	
#define WP_PORT				GPIOB
#define SCL_PORT      GPIOB
#define SDA_PORT			GPIOB
	
 #define AddWr 0xa0  
 #define AddRd 0xa1 
 #define SetDELAY 100
 
 #define WP_ENABLE  HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_RESET)
 #define WP_DISABLE HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_SET)
 
void delay_EEPROM(uint32_t);     /// 17.50us(20MHz) for EEPROM_DELAY=1
void Write_iic_byte(uint8_t buffer,uint8_t Address);
uint8_t Read_iic_byte(uint8_t Address);

void Write_Function(uint32_t D_Write,uint16_t adress);
uint32_t Read_Function(uint16_t adress);	
#endif

