

#include "I2C.h"

//#include "display.h"
//------------------------------------------------------------------------------------------------
//extern TIM_HandleTypeDef htim1;
uint8_t present_time[4];
uint8_t jj;

#define _Bool uint8_t 
 
#define I2C_SCL_HIGH 	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET)
#define I2C_SCL_LOW		HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET)
#define I2C_SDA_HIGH 	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET)
#define I2C_SDA_LOW		HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET)
#define I2C_SDA_READ 	HAL_GPIO_ReadPin (SDA_PORT, SDA_PIN)
 
void delay_EEPROM(uint32_t k)     /// 17.50us(20MHz) for EEPROM_DELAY=1
{
 uint32_t i=0;
	while(k--)
		i++;
}

void GPIO_IN()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
	delay_EEPROM(SetDELAY);
}
void GPIO_OUT()
{
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
_Bool check_ack(void) 
{ 
_Bool b=0;
	GPIO_IN();
	I2C_SCL_HIGH;  //scl = 1; 
	delay_EEPROM(SetDELAY); 
	b = (_Bool)I2C_SDA_READ;             //sda
	I2C_SCL_LOW;//scl = 0; 
	delay_EEPROM(SetDELAY); 
	if(b==(_Bool)0) 
		return 0; 

	return 1; 
}

//*******************************************************************************
void Send(uint8_t input)
{
uint8_t temp=0;//,input = Pdata;
	GPIO_OUT();
	delay_EEPROM(SetDELAY);
//HAL_TIM_Base_Stop_IT(&htim1);
	for(temp=0;temp<8;temp++)
	{
		if((uint8_t)(input & 0x80))
      I2C_SDA_HIGH;
		else //if(!((uint8_t)(input & 0x80)))
			I2C_SDA_LOW;
		
		delay_EEPROM(SetDELAY);
		I2C_SCL_HIGH;  //scl=1;
		delay_EEPROM(SetDELAY);
		I2C_SCL_LOW;//scl=0;
		delay_EEPROM(SetDELAY);
		input=input<<1;
	}
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

//*******************************************************************************
void Write_iic_byte(uint8_t buffer_1,uint8_t Address)
{
_Bool Error_bit;
uint8_t Address8Bit,Pdata;
	
	//HAL_TIM_Base_Stop_IT(&htim1);
//	HAL_GPIO_WritePin(WP_PORT, WP_PIN, GPIO_PIN_RESET); //wp=0;
	
	Pdata = buffer_1;
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
	
	    
	do
	{	
		Send(9);
		Error_bit=check_ack();
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
_Bool Error_bit;
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

