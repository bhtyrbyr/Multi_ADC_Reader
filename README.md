# Multi_ADC_Reader
This library is for an adc input multiplexer developed using the 74hc595 integration  and the cd4051 integration.
##############################################################################################################################
 Description:																												
	- This library is for an adc input multiplexer developed using the 74hc595 integration  and the cd4051 integration.		
	- The library is built on standard HAL libraries and performs all GPIO operations through HAL.							
	- The library is provided as open source,  and modification and  republication is   permitted as long as the  first		
	  author name is shared.																								
																															
Things to do before using the library:																						
	- The stm32 HAL ADC and GPIO libraries must be included in the project to be used.										
	- There are 2 additional operating modes in the library. Debug mode and test mode.										
	- To activate  test mode, the  #define TEST_MODE 1500  line  in the library must  be  removed from the comment line.	
	  The value 1500 here is the waiting time and is changeable.															
	- Debug mode is the mode in which information messages are shared via  UART during operation.  In order  to use this	
	  mode, define DEBUG_MODE line must be removed from the comment line. In order to use this mode a UART_HandleTypeDef	
	  type struct reference must be given by  calling the function  "void Multi_ADC_Com_Init(UART_HandleTypeDef *huart)"	
	  which becomes available after defining DEBUG_MODE.																	
																															
After the library presets are made:																							
	- Include project_lib.hpp header file  in your main file,  after that you should create an object  of MLT_ADC_READER	
 	  class.																												
	- There are 2 Multi_ADC_Init functions in the created object. If you are going to use an  ADC_HandleTypeDef that you	
	  have configured with your ow n definitions, you  must give a  reference to the  init function. If you call an init	
	  function without giving a reference, the ADC1 predefined in the library will be active and will start reading over	
	  CHANNEL_1.																											
																															
Pin configuration by default in the library:																				
		- PA1 ->ADC_IN																										
		- PE7 -> DATA																										
		- PE9 ->OE																											
		- PE11 ->CLCK																										
		- PE13 ->MR																											
		- PE15 ->INH																										
	is in the form of. For different pin uses, definitions corresponding to each pin must be  made. Macro names should be 	
	as follows:																												
		-USER_DEF_SHTFR_DATA																								
		-USER_DEF_SHFTR_OE																									
		-USER_DEF_SHFTR_SCLK																								
		-USER_DEF_SHFTR_LCLK																								
		-USER_DEF_SHFTR_MR																									
		-USER_DEF_MLT_DMLT_INH																								
 	The ports in which the pins are included (except for analogue reading) must be included in the same port.  PE port is 	
	used by default in the library. For port change																			
		-USER_DEF_MLT_ADC_READER_PORT																						
	macro must be defined and the new port must be specified in this macro.													
																															
After all definitions, you should call Multi_ADC_Read_Channel to read on a single channel and Multi_ADC_Read functions to 	
read all channels at once.																									
##############################################################################################################################