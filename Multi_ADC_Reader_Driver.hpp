/*
 * project_lib.hpp
 *
 *  Created on: Aug 14, 2023
 *      Author: bhtyr
 *
 *##############################################################################################################################*
 * 	Description:																												*
 *		- This library is for an adc input multiplexer developed using the 74hc595 integration  and the cd4051 integration.		*
 *		- The library is built on standard HAL libraries and performs all GPIO operations through HAL.							*
 *		- The library is provided as open source,  and modification and  republication is   permitted as long as the  first		*
 *		  author name is shared.																								*
 *																																*
 *	Things to do before using the library:																						*
 *		- The stm32 HAL ADC and GPIO libraries must be included in the project to be used.										*
 *		- There are 2 additional operating modes in the library. Debug mode and test mode.										*
 *		- To activate  test mode, the  #define TEST_MODE 1500  line  in the library must  be  removed from the comment line.	*
 *		  The value 1500 here is the waiting time and is changeable.															*
 *		- Debug mode is the mode in which information messages are shared via  UART during operation.  In order  to use this	*
 *		  mode, define DEBUG_MODE line must be removed from the comment line. In order to use this mode a UART_HandleTypeDef	*
 *		  type struct reference must be given by  calling the function  "void Multi_ADC_Com_Init(UART_HandleTypeDef *huart)"	*
 *		  which becomes available after defining DEBUG_MODE.																	*
 *																																*
 *	After the library presets are made:																							*
 *		- Include project_lib.hpp header file  in your main file,  after that you should create an object  of MLT_ADC_READER	*
 * 		  class.																												*
 *		- There are 2 Multi_ADC_Init functions in the created object. If you are going to use an  ADC_HandleTypeDef that you	*
 *		  have configured with your ow n definitions, you  must give a  reference to the  init function. If you call an init	*
 *		  function without giving a reference, the ADC1 predefined in the library will be active and will start reading over	*
 *		  CHANNEL_1.																											*
 *																																*
 *	Pin configuration by default in the library:																				*
 *			- PA1 ->ADC_IN																										*
 *			- PE7 -> DATA																										*
 *			- PE9 ->OE																											*
 *			- PE11 ->CLCK																										*
 *			- PE13 ->MR																											*
 *			- PE15 ->INH																										*
 *		is in the form of. For different pin uses, definitions corresponding to each pin must be  made. Macro names should be 	*
 *		as follows:																												*
 *			-USER_DEF_SHTFR_DATA																								*
 *			-USER_DEF_SHFTR_OE																									*
 *			-USER_DEF_SHFTR_SCLK																								*
 *			-USER_DEF_SHFTR_LCLK																								*
 *			-USER_DEF_SHFTR_MR																									*
 *			-USER_DEF_MLT_DMLT_INH																								*
 * 		The ports in which the pins are included (except for analogue reading) must be included in the same port.  PE port is 	*
 *		used by default in the library. For port change																			*
 *			-USER_DEF_MLT_ADC_READER_PORT																						*
 *		macro must be defined and the new port must be specified in this macro.													*
 *																																*
 *	After all definitions, you should call Multi_ADC_Read_Channel to read on a single channel and Multi_ADC_Read functions to 	*
 *	read all channels at once.																									*
 *##############################################################################################################################*									
 * */
#ifndef INC_PROJECT_LIB_H_
#define INC_PROJECT_LIB_H_

#include "stm32f4xx_hal.h"

/* #define TEST_MODE		1500 */
#define DEBUG_MODE

#ifdef DEBUG_MODE
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#endif


/*_____________________GPIO Pins Macro Define_____________________*/
#ifndef USER_DEF_MAX_ADC_CHNL
#define	MAX_ADC_CHNL					15
#else
#define MAX_ADC_CHNL					USER_DEF_MAX_ADC_CHNL
#endif

#ifndef USER_DEF_MLT_ADC_READER_PORT
#define MLT_ADC_READER_PORT 			GPIOE
#else
#define MLT_ADC_READER_PORT				USER_MLT_ADC_READER_PORT
#endif

#ifndef USER_DEF_SHTFR_DATA
#define SHFTR_DATA 						GPIO_PIN_7		// Default Data Pin
#else
#define SHFTR_DATA 						USER_SHTFR_DATA
#endif

#ifndef USER_DEF_SHFTR_OE
#define SHFTR_OE 						GPIO_PIN_9		// Default Output En/Dis Pin
#else
#define SHFTR_OE 						USER_SHFTR_OE
#endif

#ifndef USER_DEF_SHFTR_SCLK
#define SHFTR_SCLK 						GPIO_PIN_11		// Default Shifter Clock Pin
#else
#define SHFTR_SCLK 						USER_SHFTR_SCLK
#endif

#ifndef USER_DEF_SHFTR_LCLK
#define SHFTR_LCLK 						SHFTR_SCLK		// Default Latch Clock Pin
#else
#define SHFTR_LCLK 						USER_SHFTR_LCLK
#endif

#ifndef USER_DEF_SHFTR_MR
#define SHFTR_MR 						GPIO_PIN_13		// Default Reset Pin
#else
#define SHFTR_MR 						USER_SHFTR_MR
#endif

#ifndef USER_DEF_MLT_DMLT_INH
#define MLT_DMLT_INH 					GPIO_PIN_15
#else
#define MLT_DMLT_INH 					USER_MLT_DMLT_INH
#endif

/*___________________Channel ID  Macro Define_____________________*/

#define CHANNEL_0_ID					0x00	
#define CHANNEL_1_ID					0x01	
#define CHANNEL_2_ID					0x02	
#define CHANNEL_3_ID					0x03	
#define CHANNEL_4_ID					0x04	
#define CHANNEL_5_ID					0x05	
#define CHANNEL_6_ID					0x06	
#define CHANNEL_7_ID					0x07	
#define CHANNEL_8_ID					0x0F	
#define CHANNEL_9_ID					0x17	
#define CHANNEL_10_ID					0x1F	
#define CHANNEL_11_ID					0x27	
#define CHANNEL_12_ID					0x2F	
#define CHANNEL_13_ID					0x37	
#define CHANNEL_14_ID					0x3F	

#if MAX_ADC_CHNL > 15 && MAX_ADC_CHNL < 32
#define CHANNEL_15_ID					0x007F	
#define CHANNEL_16_ID					0x00BF	
#define CHANNEL_17_ID					0x00FF	
#define CHANNEL_18_ID					0x013F	
#define CHANNEL_19_ID					0x017F	
#define CHANNEL_20_ID					0x01BF	
#define CHANNEL_21_ID					0x01FF	
#define CHANNEL_22_ID					0x03FF	
#define CHANNEL_23_ID					0x05FF	
#define CHANNEL_24_ID					0x09FF	
#define CHANNEL_25_ID					0x0BFF	
#define CHANNEL_26_ID					0x0DFF	
#define CHANNEL_27_ID					0x0FFF	
#define CHANNEL_28_ID					0x1FFF	
#define CHANNEL_29_ID					0x2FFF	
#define CHANNEL_30_ID					0x3FFF	
#define CHANNEL_31_ID					0x4FFF	
#endif

#define HIGH							GPIO_PIN_SET
#define LOW								GPIO_PIN_RESET

/*_____________________Function Macro Define______________________*/

#define __MLT_ADC_READER_PIN_SET(PIN, STATUS)	HAL_GPIO_WritePin(MLT_ADC_READER_PORT, PIN, STATUS)

/*____________________________Enum Define_________________________*/

typedef enum
{
	MSB_FIRST = 0,
	LSB_FIRST
}MSB_LSB_STATUS;


/*__________________________Class Define__________________________*/

class MLT_ADC_READER{
	public:
		void Multi_ADC_Init(void);
		void Multi_ADC_Init(ADC_HandleTypeDef *hadc);
#ifdef DEBUG_MODE
		void Multi_ADC_Com_Init(UART_HandleTypeDef *huart);
#endif
		void Multi_ADC_Read(uint32_t Buffer[]);
		uint32_t Multi_ADC_Read_Channel(uint8_t Channel_ID);
	private:
		ADC_HandleTypeDef *To_Be_Used_ADC;
		ADC_HandleTypeDef hadc;
#ifdef DEBUG_MODE
		UART_HandleTypeDef *com_uart;
		void Print_Message(char *format, ...);
#endif
		HAL_StatusTypeDef Multi_ADC_GPIO_Init(void);
		HAL_StatusTypeDef Multi_ADC_ADC_Init(void);
		HAL_StatusTypeDef Multi_ADC_SHFTR_Reset(void);
		HAL_StatusTypeDef Multi_ADC_Send_Data_Shifter(uint8_t data, MSB_LSB_STATUS MS_LS_sts);
		uint32_t Multi_ADC_Read_Adc_Value(void);
};

#endif /* INC_PROJECT_LIB_H_ */
