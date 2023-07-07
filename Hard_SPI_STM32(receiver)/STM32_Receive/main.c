#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stdio.h"


#define SPIx_RCC		RCC_APB2Periph_SPI1
#define SPIx			SPI1
#define SPI_GPIO_RCC		RCC_APB2Periph_GPIOA
#define SPI_GPIO		GPIOA
#define SPI_PIN_MOSI		GPIO_Pin_7
#define SPI_PIN_MISO		GPIO_Pin_6
#define SPI_PIN_SCK		GPIO_Pin_5
#define SPI_PIN_SS		GPIO_Pin_4

typedef SPI_InitTypeDef Spi_ConfigType;
typedef uint8_t Std_ReturnType;
typedef uint8_t Spi_ChannelType;
typedef uint8_t Spi_DataBufferType;

#define E_OK 			(Std_ReturnType)0
#define E_NOT_OK 		(Std_ReturnType)1

void Spi_Init (Spi_ConfigType* ConfigPtr);
Std_ReturnType Spi_DeInit (void);
Std_ReturnType Spi_WriteIB (Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr);
Std_ReturnType Spi_ReadIB (Spi_ChannelType Channel,Spi_DataBufferType* DataBufferPointer);
uint8_t receivedData;

int main(void){
	Spi_ConfigType ConfigPtr;
	Spi_Init (&ConfigPtr);
	while(1)
	{
		if(GPIO_ReadInputDataBit(SPI_GPIO,SPI_PIN_SS) == 0){		
		Spi_ReadIB(1,&receivedData);
		}
	}
}

void Spi_Init (Spi_ConfigType* ConfigPtr){
	/*Initialize SPI*/
	if(SPIx == SPI1)RCC_APB2PeriphClockCmd(SPIx_RCC, ENABLE);
	else if((SPIx == SPI2))RCC_APB1PeriphClockCmd(SPIx_RCC, ENABLE);
	ConfigPtr->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	ConfigPtr->SPI_CPHA = SPI_CPHA_1Edge;
	ConfigPtr->SPI_CPOL = SPI_CPOL_Low;
	ConfigPtr->SPI_DataSize = SPI_DataSize_8b;
	ConfigPtr->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	ConfigPtr->SPI_FirstBit = SPI_FirstBit_MSB;
	ConfigPtr->SPI_Mode = SPI_Mode_Master;
	ConfigPtr->SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_Init(SPIx, ConfigPtr); 
	SPI_Cmd(SPIx, ENABLE);
}

Std_ReturnType Spi_DeInit (void){
	if(SPIx == SPI1 || SPIx == SPI2)
	{
		SPI_Cmd(SPIx,DISABLE);
		RCC_APB2PeriphClockCmd(SPIx_RCC,DISABLE);
		RCC_APB2PeriphClockCmd(SPI_GPIO_RCC,DISABLE);
		return E_OK;
	}
	else return E_NOT_OK;
}

Std_ReturnType Spi_WriteIB (Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	switch(Channel)
	{
		case 1:
			RCC_APB2PeriphClockCmd(SPI_GPIO_RCC, ENABLE);

			GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
		
			GPIO_InitStruct.GPIO_Pin = SPI_PIN_SS;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
		break;
		case 2:
			
			RCC_APB1PeriphClockCmd(SPI_GPIO_RCC, ENABLE);
		
			GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
		
			GPIO_InitStruct.GPIO_Pin = SPI_PIN_SS;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	if(DataBufferPtr == NULL)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		return E_NOT_OK;
	}
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, *DataBufferPtr);
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	return E_OK;
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,Spi_DataBufferType* DataBufferPointer)
{
	switch(Channel)
	{
		case 1:
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			*DataBufferPointer = SPI_I2S_ReceiveData(SPI1);
			break;
		case 2:
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
			*DataBufferPointer = SPI_I2S_ReceiveData(SPI2);
			break;
	}
	if(DataBufferPointer == NULL){
		return E_NOT_OK;
	}else return E_OK;
}

