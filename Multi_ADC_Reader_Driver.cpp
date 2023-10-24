/*
 * project_lib.c++
 *
 *  Created on: Aug 14, 2023
 *      Author: bhtyr
 */

#include <Multi_ADC_Reader_Driver.hpp>
#include "main.h"
#include <string.h>

#ifdef	DEBUG_MODE
void MLT_ADC_READER::Print_Message(char *format, ...)
{
	char msg[255];
	va_list va_list;
	va_start(va_list, format);
	vsprintf(msg, format, va_list);
	HAL_UART_Transmit(this->com_uart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	va_end(va_list);
}

void MLT_ADC_READER::Multi_ADC_Com_Init(UART_HandleTypeDef *huart)
{
	this->com_uart = huart;
}
#endif

void MLT_ADC_READER::Multi_ADC_Init(void)
{
#ifdef	DEBUG_MODE
	  this->Print_Message("Initiating driver identification.\r\n");
#endif
	if(this->Multi_ADC_GPIO_Init() != HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		Error_Handler();
	}
#ifdef	DEBUG_MODE
	  this->Print_Message("GPIOs initialised.\r\n");
#endif
	if(this->Multi_ADC_ADC_Init() != HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		Error_Handler();
	}
#ifdef	DEBUG_MODE
	  this->Print_Message("ADC initialised. Default ADC definitions were used.\r\nThe initialisation of the drives is complete.\r\n");
#endif
}

void MLT_ADC_READER::Multi_ADC_Init(ADC_HandleTypeDef *hadc)
{
#ifdef	DEBUG_MODE
	  this->Print_Message("Initiating driver identification.\r\n");
#endif
	if(this->Multi_ADC_GPIO_Init() != HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		Error_Handler();
	}
#ifdef	DEBUG_MODE
	  this->Print_Message("GPIOs initialised.\r\n");
#endif
	this->To_Be_Used_ADC = hadc;
#ifdef	DEBUG_MODE
	  this->Print_Message("ADC initialised. User ADC definitions were used.\r\nThe initialisation of the drives is complete.\r\n");
#endif
}

HAL_StatusTypeDef MLT_ADC_READER::Multi_ADC_GPIO_Init(void)
{
#ifdef	DEBUG_MODE
	  this->Print_Message("Pin definitions for the Shifter integration are being initiated.\r\n");
#endif
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__MLT_ADC_READER_PIN_SET(SHFTR_DATA|SHFTR_SCLK|SHFTR_LCLK|SHFTR_OE|SHFTR_MR|MLT_DMLT_INH, LOW);

	GPIO_InitStruct.Pin = SHFTR_DATA|SHFTR_OE|SHFTR_LCLK|SHFTR_SCLK|SHFTR_MR|MLT_DMLT_INH;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

#ifdef	DEBUG_MODE
	  this->Print_Message("Pin definitions for the Shifter integration have been completed.\r\n");
#endif
	HAL_GPIO_Init(MLT_ADC_READER_PORT, &GPIO_InitStruct);
	__MLT_ADC_READER_PIN_SET(SHFTR_OE, HIGH);
	__MLT_ADC_READER_PIN_SET(SHFTR_MR, LOW);
	__MLT_ADC_READER_PIN_SET(SHFTR_MR, HIGH);
	__MLT_ADC_READER_PIN_SET(SHFTR_LCLK, HIGH);
	__MLT_ADC_READER_PIN_SET(SHFTR_DATA|SHFTR_SCLK|SHFTR_OE, LOW);
#ifdef	DEBUG_MODE
	  this->Print_Message("The pins defined for the Shifter integration were set to the starting position.\r\n");
#endif
	return HAL_OK;
}

HAL_StatusTypeDef MLT_ADC_READER::Multi_ADC_ADC_Init(void)
{
#ifdef	DEBUG_MODE
#endif
	  ADC_ChannelConfTypeDef sConfig = {0};
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

#ifdef	DEBUG_MODE
	  this->Print_Message("Default driver settings are made for ADC readings.\r\n");
#endif
	    this->hadc.Instance = ADC1;
	    this->hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	    this->hadc.Init.Resolution = ADC_RESOLUTION_12B;
	    this->hadc.Init.ScanConvMode = DISABLE;
	    this->hadc.Init.ContinuousConvMode = ENABLE;
	    this->hadc.Init.DiscontinuousConvMode = DISABLE;
	    this->hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	    this->hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	    this->hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	    this->hadc.Init.NbrOfConversion = 1;
	    this->hadc.Init.DMAContinuousRequests = DISABLE;
	    this->hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	    if (HAL_ADC_Init(&this->hadc) != HAL_OK)
	    {
	      Error_Handler();
	    }

#ifdef	DEBUG_MODE
	  this->Print_Message("Default driver settings were made for ADC readings.\r\nADC channel adjustments are being made.\r\n");
#endif
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  if (HAL_ADC_ConfigChannel(&this->hadc, &sConfig) != HAL_OK)
	  {
		  Error_Handler();
	  }

		this->To_Be_Used_ADC = &this->hadc;

#ifdef	DEBUG_MODE
	  this->Print_Message("ADC channel adjustments are made.\r\nADC pin adjustments are made.\r\n");
#endif
	  __HAL_RCC_ADC1_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  GPIO_InitStruct.Pin = GPIO_PIN_1;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#ifdef	DEBUG_MODE
	  this->Print_Message("ADC pin adjustments were made. ADC definitions were completed successfully.\r\n");
#endif
	  return HAL_OK;
}

void MLT_ADC_READER::Multi_ADC_Read(uint32_t Buffer[])
{
#ifndef TEST_MODE
	__disable_irq();
#endif
#if MAX_ADC_CHNL <= 15
	uint8_t channel_ind = 0;
#endif
#if MAX_ADC_CHNL > 15
	uint16_t channel_ind = 0;
#endif
	uint32_t temp = 0x00000000U;
	for(channel_ind = 0; channel_ind < MAX_ADC_CHNL; channel_ind++)
	{
#ifdef	DEBUG_MODE
		this->Print_Message("Reading channel %2d. (Channel ID = 0x%2x)\r\n", channel_ind, (((channel_ind % 7)<<((channel_ind /7)*3))|
				 (0xFFFFFFFF>>(32-((channel_ind /7)*3)))));
#endif
		Buffer[channel_ind] = this->Multi_ADC_Read_Channel(
						((channel_ind % 7)<<((channel_ind /7)*3))|
						 (0xFFFFFFFF>>(32-((channel_ind /7)*3))));
#ifdef	DEBUG_MODE
		this->Print_Message("Channel %x read. Read value : %d\n", channel_ind, Buffer[channel_ind]);
#endif

#ifdef TEST_MODE
		HAL_Delay(TEST_MODE);
#endif
	}
#ifndef TEST_MODE
	__enable_irq();
#endif
}

uint32_t MLT_ADC_READER::Multi_ADC_Read_Channel(uint8_t Channel_ID)
{
	uint32_t temp = 0;
	this->Multi_ADC_SHFTR_Reset();
	__MLT_ADC_READER_PIN_SET(MLT_DMLT_INH, HIGH);
	this->Multi_ADC_Send_Data_Shifter(Channel_ID, MSB_FIRST);
	__MLT_ADC_READER_PIN_SET(MLT_DMLT_INH, LOW);
	temp = Multi_ADC_Read_Adc_Value();
	return temp;

}

uint32_t MLT_ADC_READER::Multi_ADC_Read_Adc_Value(void)
{
	uint32_t adc_value = 0;

	HAL_ADC_Start(this->To_Be_Used_ADC);
	while(HAL_ADC_PollForConversion(this->To_Be_Used_ADC, HAL_MAX_DELAY) != HAL_OK)
	{}
	adc_value = HAL_ADC_GetValue(this->To_Be_Used_ADC);
	HAL_ADC_Stop(this->To_Be_Used_ADC);
	return adc_value;
}


HAL_StatusTypeDef MLT_ADC_READER::Multi_ADC_SHFTR_Reset(void)
{
	__MLT_ADC_READER_PIN_SET(SHFTR_MR, LOW);
	__MLT_ADC_READER_PIN_SET(SHFTR_MR, HIGH);
	__MLT_ADC_READER_PIN_SET(SHFTR_OE, HIGH);
	__MLT_ADC_READER_PIN_SET(SHFTR_OE, LOW);
	return HAL_OK;
}

HAL_StatusTypeDef MLT_ADC_READER::Multi_ADC_Send_Data_Shifter(uint8_t data,  MSB_LSB_STATUS MS_LS_sts)
{
	int8_t bit_ind = 0;
	uint8_t temp   = 0;
	if(MS_LS_sts == MSB_FIRST)
	{
		for(bit_ind = 8; bit_ind >= 0; bit_ind--)
		{
			temp = (data &(0x01U << (bit_ind - 1))) >> (bit_ind - 1);
			if(temp == 0x01U)	__MLT_ADC_READER_PIN_SET(SHFTR_DATA, HIGH);
			else				__MLT_ADC_READER_PIN_SET(SHFTR_DATA, LOW);
			__MLT_ADC_READER_PIN_SET(SHFTR_SCLK|SHFTR_LCLK, HIGH);
			__MLT_ADC_READER_PIN_SET(SHFTR_SCLK|SHFTR_LCLK, LOW);
		}
	}
	else
	{
		for(bit_ind = 0; bit_ind < 9; bit_ind++)
		{
			temp = (data &(0x01U << bit_ind)) >> (bit_ind);
			if(temp == 0x01U)	__MLT_ADC_READER_PIN_SET(SHFTR_DATA, HIGH);
			else				__MLT_ADC_READER_PIN_SET(SHFTR_DATA, LOW);
			__MLT_ADC_READER_PIN_SET(SHFTR_SCLK|SHFTR_LCLK, HIGH);
			__MLT_ADC_READER_PIN_SET(SHFTR_SCLK|SHFTR_LCLK, LOW);
		}
	}
	return HAL_OK;
}

