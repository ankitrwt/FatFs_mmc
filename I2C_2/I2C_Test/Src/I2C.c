

#include "I2C.h"
#include <stdbool.h>
//#include "display.h"
//------------------------------------------------------------------------------------------------
//extern TIM_HandleTypeDef htim1;
uint8_t present_time[4];
uint8_t jj;

//typedef enum {false=0, true=1} bool;
//#define bool uint8_t
//#define true 1
//#define false 0

/*
#define I2CSPEED 100
void I2C_delay( void );
bool read_SCL( void ){return HAL_GPIO_ReadPin (SCL_PORT, SCL_PIN);}  // Set SCL as input and return current level of line, 0 or 1
bool read_SDA( void ){return HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN);}; // Set SDA as input and return current level of line, 0 or 1
void set_SCL( void ){HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);}     // Actively drive SCL signal high
void clear_SCL( void ){HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);} // Actively drive SCL signal low
void set_SDA( void ){HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);} // Actively drive SDA signal high
void clear_SDA( void ){HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);} // Actively drive SDA signal low
//void arbitration_lost( void );

bool started = false; // global data
*/

#define SET_SCL_HIGH 	while(!HAL_GPIO_ReadPin (SCL_PORT, SCL_PIN))
#define SET_SCL_LOW		while(HAL_GPIO_ReadPin (SCL_PORT, SCL_PIN))
#define SET_SDA_HIGH 	while(!HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN))
#define SET_SDA_LOW		while(HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN))
//#define I2C_SDA_READ 	HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN)
//#define I2C_SCL_READ  HAL_GPIO_ReadPin (SCL_PORT, SCL_PIN)
 
#define I2C_SCL_HIGH 	{HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET); delay_EEPROM(SetDELAY); SET_SCL_HIGH;}
#define I2C_SCL_LOW		{HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET); delay_EEPROM(SetDELAY); SET_SCL_LOW;}
#define I2C_SDA_HIGH 	{HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET); delay_EEPROM(SetDELAY); SET_SDA_HIGH;}
#define I2C_SDA_LOW		{HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET); delay_EEPROM(SetDELAY); SET_SDA_LOW;}
#define I2C_SDA_READ 	HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN)
#define I2C_SCL_READ  HAL_GPIO_ReadPin (SCL_PORT, SCL_PIN)


void delay_EEPROM(uint32_t k)     /// 17.50us(20MHz) for EEPROM_DELAY=1
{
 uint32_t i=0;
	while(k--)
		i++;
}

void GPIO_IN()
{
	//I2C_SDA_HIGH
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
	delay_EEPROM(SetDELAY);
}
void GPIO_OUT()
{
	I2C_SDA_LOW
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
	delay_EEPROM(SetDELAY);
}


void Start(void)
{ 
	GPIO_OUT();
	I2C_SDA_HIGH;  //sda=1;
	delay_EEPROM(SetDELAY);
	I2C_SCL_HIGH;  //scl=1;
	delay_EEPROM(SetDELAY);
	I2C_SDA_LOW;//sda=0;
	delay_EEPROM(SetDELAY);
	I2C_SCL_LOW;//scl=0;
}

void Stop(void)
{
	GPIO_OUT();
	I2C_SDA_LOW;	//sda=0;
	delay_EEPROM(SetDELAY);
	I2C_SCL_HIGH;  //scl=1;
	delay_EEPROM(SetDELAY);
	I2C_SDA_HIGH;  //sda=1;
	delay_EEPROM(SetDELAY);	
}
//****************************************************************************
//void I2C_delay( void ) 
//{ 
//  volatile int v;
//  int i;
//  for( i = 0; i < I2CSPEED / 2; i++ )
//  {
//    v;
//  }
//}
//****************************************************************************
//void i2c_start_cond( void ) 
//{
//  if( started ) 
//  { 
//    // if started, do a restart cond
//    // set SDA to 1
//    set_SDA();
//    I2C_delay();

//    while( read_SCL() == 0 ) 
//    {  // Clock stretching
//      // You should add timeout to this loop
//    }

//    // Repeated start setup time, minimum 4.7us
//    I2C_delay();

//  }

//  if( read_SDA() == 0 ) 
//  {
////    arbitration_lost();
//  }

//  // SCL is high, set SDA from 1 to 0.
//  clear_SDA();
//  I2C_delay();
//  clear_SCL();
//  started = true;

//}
////****************************************************************************
//void i2c_stop_cond( void )
//{
//  // set SDA to 0
//  clear_SDA();
//  I2C_delay();

//  // Clock stretching
//  while( read_SCL() == 0 ) 
//  {
//    // add timeout to this loop.
//  }

//  // Stop bit setup time, minimum 4us
//  I2C_delay();

//  // SCL is high, set SDA from 0 to 1
//  set_SDA();
//  I2C_delay();

//  if( read_SDA() == 0 ) 
//  {
////    arbitration_lost();
//  }

//  I2C_delay();
//  started = false;

//}

////****************************************************************************
//void i2c_write_bit( bool bit ) 
//{
//  if( bit ) 
//  {
//    set_SDA();
//  } 
//  else 
//  {
//    clear_SDA();
//  }

//  // SDA change propagation delay
//  I2C_delay();  

//  // Set SCL high to indicate a new valid SDA value is available
//  set_SCL();

//  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
//  I2C_delay();

//  while( read_SCL() == 0 ) 
//  { // Clock stretching
//    // You should add timeout to this loop
//  }

//  // SCL is high, now data is valid
//  // If SDA is high, check that nobody else is driving SDA
//  if( bit && ( read_SDA() == 0 ) )
//  {
//  //  arbitration_lost();
//  }

//  // Clear the SCL to low in preparation for next change
//  clear_SCL();
//}

////****************************************************************************
//bool i2c_read_bit( void ) 
//{
//  bool bit;

//  // Let the slave drive data
//  set_SDA();

//  // Wait for SDA value to be written by slave, minimum of 4us for standard mode
//  I2C_delay();

//  // Set SCL high to indicate a new valid SDA value is available
//  set_SCL();

//  while( read_SCL() == 0 ) 
//  { // Clock stretching
//    // You should add timeout to this loop
//  }

//  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
//  I2C_delay();

//  // SCL is high, read out bit
//  bit = read_SDA();

//  // Set SCL low in preparation for next operation
//  clear_SCL();

//  return bit;

//}


////****************************************************************************
//bool i2c_write_byte( bool          send_start ,
//                     bool          send_stop  ,
//                     unsigned char byte         ) 
//{
//  unsigned bit;
//  bool     nack;

//  if( send_start ) 
//  {
//    i2c_start_cond();
//  }

//  for( bit = 0; bit < 8; bit++ ) 
//  {
//    i2c_write_bit( ( byte & 0x80 ) != 0 );
//    byte <<= 1;
//  }

//  nack = i2c_read_bit();

//  if (send_stop) 
//  {
//    i2c_stop_cond();
//  }

//  return nack;

//}

////****************************************************************************
//unsigned char i2c_read_byte( bool nack , bool send_stop ) 
//{
//  unsigned char byte = 0;
//  unsigned char bit;

//  for( bit = 0; bit < 8; bit++ ) 
//  {
//    byte = ( byte << 1 ) | i2c_read_bit();
//  }

//  i2c_write_bit( nack );

//  if( send_stop ) 
//  {
//    i2c_stop_cond();
//  }

//  return byte;

//}
////*******************************************************************************
//void Write_iic_byte(uint8_t buffer_1,uint8_t Address)
//{
//bool Error_bit;
//uint8_t Address8Bit,Pdata;
//	
//	//HAL_TIM_Base_Stop_IT(&htim1);
////	HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_RESET); //wp=0;
//	
//	Pdata = buffer_1;
//	i2c_start_cond();
//	delay_EEPROM(SetDELAY);
//	
//	do
//	{
//		i2c_write_byte(AddWr);
//		Error_bit=check_ack();
//	}while(Error_bit);
//	jj = 1;	
//	
////	do
////	{	
////		Address8Bit=(uint8_t)(Address>>8);
////		Send(Address8Bit);
////		Error_bit=check_ack();
////	}while(Error_bit);
//	
//	do
//	{	
////		Address8Bit=(uint8_t)Address;
//		Send(Address);
//		Error_bit=check_ack();
//	}while(Error_bit);
//	jj = 2;
//	    
//	do
//	{	
//		Send(9);
//		Error_bit=check_ack();
//		jj++;
//	}while(Error_bit);
//	jj=10;

//	
//	Stop();	
//	//HAL_TIM_Base_Start_IT(&htim1);
//	delay_EEPROM((uint32_t)(SetDELAY*200));
////	HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_SET);  //wp=1;
//}

////********************************RANDOM READ*************************************

//uint8_t Read_iic_byte(uint8_t Address)
//{
//bool Error_bit;
//uint8_t Pdata,Address8Bit;
//	Start();
//	delay_EEPROM(SetDELAY);
//	
//	do
//	{
//		Send(AddWr);
//		Error_bit=check_ack();
//	}while(Error_bit);

////	do
////	{	
////		Address8Bit=(uint8_t)(Address>>8);
////		Send(Address8Bit);
////		Error_bit=check_ack();
////	}while(Error_bit);
//	
//	do
//	{	
////		Address8Bit=(uint8_t)Address;
//		Send(Address);
//		Error_bit=check_ack();
//	}while(Error_bit);
//	
//	Start();
//	delay_EEPROM(SetDELAY);
//	
//	do
//	{
//		Send(AddRd);
//		Error_bit=check_ack();
//	}while(Error_bit);	
//	
//	
//	do
//	{
//		Pdata=Read();
//		Error_bit=check_ack();
//	}while(!Error_bit);	
//	
//	Stop();
////	delay_EEPROM((uint32_t)(SetDELAY*500));
//	return(Pdata);
//}
//****************************************************************************

bool check_ack(void) 
{ 
bool bx=0;
	GPIO_IN();
	I2C_SCL_HIGH;  //scl = 1; 
	delay_EEPROM(SetDELAY);
//	delay_EEPROM(SetDELAY);
	bx = (bool)I2C_SDA_READ;	//sda
//	delay_EEPROM(SetDELAY);
	I2C_SCL_LOW;//scl = 0; 
	delay_EEPROM(SetDELAY);
		return (bx?1:0);  
}

//*******************************************************************************
uint8_t flg,chk_data[8],chk_add[8];

void Send(uint8_t input)
{
uint8_t temp=0;//,input = Pdata;
	GPIO_OUT();
	delay_EEPROM(SetDELAY);
	
//HAL_TIM_Base_Stop_IT(&htim1);
	for(temp=0;temp<8;temp++)
	{
		if((uint8_t)(input & 0x80))
		{
      I2C_SDA_HIGH
			if(flg == 1)
				chk_add[temp] = 1;
			if(flg == 2)
				chk_data[temp] = 1;
		}
		else //if(!((uint8_t)(input & 0x80)))
		{
			I2C_SDA_LOW
			if(flg == 1)
				chk_add[temp] = 0;
			if(flg == 2)
				chk_data[temp] = 0;
		}
		
		input=(uint8_t)(input<<1);
		//delay_EEPROM(SetDELAY);
		I2C_SCL_HIGH;  //scl=1;
		delay_EEPROM(SetDELAY);
		I2C_SCL_LOW;//scl=0;
		delay_EEPROM(SetDELAY);
	}
//	I2C_SDA_LOW
//HAL_TIM_Base_Start_IT( &htim1);
}

//*******************************************************************************

uint8_t Read(void)
{
uint8_t temp=0;
uint8_t BitCounter=0;
	GPIO_IN();
	for(BitCounter=0;BitCounter<8;BitCounter++)
	{
		I2C_SCL_HIGH;  //scl=1;
		temp=(uint8_t)(temp<<1);
		temp=(uint8_t)(temp|((uint8_t)I2C_SDA_READ));//sda							
		delay_EEPROM(SetDELAY);
		I2C_SCL_LOW; //scl=0;
		delay_EEPROM(SetDELAY);
	}
return temp;
}
bool err1,err2,err3;
uint8_t Pdata;
//*******************************************************************************
void Write_iic_byte(uint8_t buffer,uint8_t Address)
{
bool Error_bit;
uint8_t Address8Bit;
	
	//HAL_TIM_Base_Stop_IT(&htim1);
//	HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_RESET); //wp=0;
	
	Pdata = buffer;
	Start();
	delay_EEPROM(SetDELAY);
	
	do
	{
		flg =1;
		Send(AddWr);
		Error_bit=check_ack();
		err1=Error_bit;
	}while(Error_bit);
	jj = 1;	
	
//	do
//	{	
//		Address8Bit=(uint8_t)(Address>>8);
//		Send(Address8Bit);
//		Error_bit=check_ack();
//	}while(Error_bit);
	
	do
	{	
//		Address8Bit=(uint8_t)Address;
		flg =1;
		Send(Address);
		Error_bit=check_ack();
		err2 = Error_bit;
	}while(Error_bit);
	jj = 2;
	    
	do
	{	
		flg = 2;
		Send(Pdata);
		Error_bit=check_ack();
		err3 = Error_bit;
		jj++;
	}while(Error_bit);
	jj=10;

	
	Stop();	
	//HAL_TIM_Base_Start_IT(&htim1);
	delay_EEPROM((uint32_t)(SetDELAY*200));
//	HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_SET);  //wp=1;
}

//********************************RANDOM READ*************************************

uint8_t Read_iic_byte(uint8_t Address)
{
bool Error_bit;
uint8_t Pdata,Address8Bit;
	Start();
	delay_EEPROM(SetDELAY);
	
	do
	{
		Send(AddWr);
		Error_bit=check_ack();
	}while(Error_bit);

//	do
//	{	
//		Address8Bit=(uint8_t)(Address>>8);
//		Send(Address8Bit);
//		Error_bit=check_ack();
//	}while(Error_bit);
	
	do
	{	
//		Address8Bit=(uint8_t)Address;
		Send(Address);
		Error_bit=check_ack();
	}while(Error_bit);
	
	Start();
	delay_EEPROM(SetDELAY);
	
	do
	{
		Send(AddRd);
		Error_bit=check_ack();
	}while(Error_bit);	
	
	
	do
	{
		Pdata=Read();
		Error_bit=check_ack();
	}while(!Error_bit);	
	
	Stop();
//	delay_EEPROM((uint32_t)(SetDELAY*500));
	return(Pdata);
}
//************************************************************************************
union 
  {
  unsigned char Char[4];
  unsigned long Long;
  }
  Long;
//----------------------------------------------------------------------
 uint32_t CharToLong(uint8_t *add)
  {
		uint8_t kk;
		for(kk=0;kk<4;kk++)
		 Long.Char[kk]  = *add++;
			
  return(Long.Long);
  }
//----------------------------------------------------------------------
 void LongToChar(uint32_t LongData, uint8_t *add)
  {
		uint8_t kk;
    Long.Long = LongData;
		
		for(kk=0;kk<4;kk++)
		  *add++    = Long.Char[kk];
 }
//----------------------------------------------------------------------		
void Write_Function(uint32_t D_Write,uint16_t adress)
{
	uint8_t cc;
		LongToChar(D_Write,(uint8_t *)&present_time[0]);

		for(cc=0;cc<4;cc++)
		{
			Write_iic_byte(present_time[cc],(adress+cc));
		//	delay_EEPROM(500);
		}

}
//----------------------------------------------------------------------
uint32_t Read_Function(uint16_t adress)
{
	uint8_t cc;
	uint32_t Temp_Pw;

		for(cc=0;cc<4;cc++)
			present_time[cc]=Read_iic_byte(adress+cc);

		Temp_Pw = CharToLong((uint8_t*)&present_time[0]);	
		return(Temp_Pw);	
}	

