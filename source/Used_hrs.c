/****************************************************************************
* File Name    :
* Developed by :
* Version      :
* Description  :

*****************************************************************************/

#include "LPC55S16.h"
#include "Color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Driver_Timer.h"
#include "Main_Menu.h"
#include <FR_CALC.h>
#include <Consumed_hrs.h>
#include "Motor.h"
#include"Driver_I2C.h"
#include <Shapes.h>
#include "Power_off.h"

uint8_t minute=0;
uint8_t hrs=0;
uint8_t factory_reset_flag=0;

extern uint16_t temp_ADC_values;
Reached UTILIZED;

extern uint8_t ON_Flag;
extern uint16_t used_hrs_count;
extern volatile uint8_t normal_run;
void usedhrs_status(void);
void Packing_reset_frame(void);
void Reset_loading(uint8_t loading_level);
void Complete_Reset();
void Complete_reset_frame(void);

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void machine_on_used_hrs(void)
{

		if(UTILIZED.device_sec_cnt>5)
		{
			UTILIZED.device_sec_cnt=0;
			UTILIZED.device_min_cnt++;
		}

		if(UTILIZED.device_min_cnt>5)
		{
			UTILIZED.device_min_cnt=0;
			UTILIZED.device_hrs_cnt++;
		}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void battery_used_hrs(void)
{
	if(UTILIZED.battery_sec_cnt>5)
	{
		UTILIZED.battery_sec_cnt=0;
		UTILIZED.battery_min_cnt++;
	}

	if(UTILIZED.battery_min_cnt>5)
	{
		UTILIZED.battery_min_cnt=0;
		UTILIZED.battery_hrs_cnt++;
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void run_stop_used_hrs(void)
{

	if(UTILIZED.running_sec_cnt>5)
	{
		UTILIZED.running_sec_cnt=0;
		UTILIZED.running_min_cnt++;
	}

	if(UTILIZED.running_min_cnt>5)
	{
		UTILIZED.running_min_cnt=0;
		UTILIZED.running_hrs_cnt++;
	}

}

void usedhrs_status(void)
{
	run_stop_used_hrs();
	machine_on_used_hrs();
	if(BATT.mode)
	{
		battery_used_hrs();
	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Packing_reset()
{

	 Packing_reset_frame();
	 Reset_loading(1);

	 /****************************LEVEL1*******************************/

	 /*****************************************************************/
	 Reset_loading(2);
	 /****************************LEVEL2*******************************/

	 /*****************************************************************/
	 Reset_loading(3);
	 /****************************LEVEL3*******************************/

	 /*****************************************************************/
	 Reset_loading(4);
	 /****************************LEVEL4*******************************/

	 /*****************************************************************/
	 Reset_loading(5);
	 /****************************LEVEL5******************************/

	 /*****************************************************************/
	 Reset_loading(6);
	 /****************************LEVEL6*******************************/

	 /*****************************************************************/
	// Reset_loading(7);

/*************************************USED_HRS_RESET*************************/

	if(!factory_reset_flag)
	{
		UTILIZED.factory_used_hrs=UTILIZED.device_hrs_cnt;
		EEPROM_datawrite(EEPROM_FACTORY_HRS,UTILIZED.factory_used_hrs,2);
		factory_reset_flag=1;
		EEPROM_datawrite(EEPROM_FACTORY_RST,factory_reset_flag,1);
	}
	EEPROM_usedhrs_write(2);//EXCEPT FACTORY USED HRS RESET TO ZERO(PASSING VALUE 2)
/*****************************************************************************/
	 Reset_loading(7);

}

void Packing_reset_frame(void)
{
	uint8_t xpos=0;
		Rectangle(0,0,320,240,MAIN_FRAME_COL);
		Font_Select(ARIAL_BOLD_14);

		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"PACKING RESET");

		Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"Please wait...");
		CENTER_SCR(MAIN_FRAME_COL);  //center green
	//	Font_Select(ARIAL_14);
//		xpos=Letters_Alignments(0,320, (uint8_t*)"Reset Processing...",0);
//		TFT_String(xpos,170,(uint8_t *)"Reset Processing..." ,MAIN_FRAME_COL,WHITE);
		TFT_String_align(140,170,160,(uint8_t *)"Reset Processing...",0,MAIN_FRAME_COL,WHITE);
}

void Complete_reset_frame(void)
{
	uint8_t xpos=0;
	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	Font_Select(ARIAL_14);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"COMPLETE RESET");

	Frame_Footer_new(NEW_FTR_COL,HDR_TXT_COL,(uint8_t *)"Please wait...");
	CENTER_SCR(MAIN_FRAME_COL);  //center green
//	Font_Select(ARIAL_14);
//	xpos=Letters_Alignments(0,320, (uint8_t*)"Reset Processing...",0);
//	TFT_String(xpos,170,(uint8_t *)"Reset Processing..." ,MAIN_FRAME_COL,WHITE);
	TFT_String_align(0,170,320,(uint8_t *)"Reset Processing...",0,MAIN_FRAME_COL,WHITE);
}

void Reset_loading(uint8_t loading_level)
{
	uint8_t loading_cnt=0;
	uint8_t loop_count=0;
	loading_cnt=loading_level*20;
	if(loading_cnt>140)
	{
		loading_cnt=140;
	}
	for(loop_count=loading_cnt-20;loop_count<=loading_cnt;loop_count++)
	{
		Rectangle(20+(loop_count*2),120,0,30,WHITE);
		delay_us(20);
	}

}

void EEPROM_usedhrs_write(uint8_t default_write)
{
	if(default_write)
	{
		UTILIZED.device_sec_cnt=0;
		UTILIZED.device_min_cnt=0;
		UTILIZED.device_hrs_cnt=0;

		UTILIZED.battery_sec_cnt=0;
		UTILIZED.battery_min_cnt=0;
		UTILIZED.battery_hrs_cnt=0;

		UTILIZED.running_sec_cnt=0;
		UTILIZED.running_min_cnt=0;
		UTILIZED.running_hrs_cnt=0;

	if(default_write==1)
	{

		UTILIZED.factory_used_hrs=0;
		EEPROM_datawrite(EEPROM_FACTORY_HRS,UTILIZED.factory_used_hrs,2);

	}
	}

	EEPROM_datawrite(EEPROM_DEVICE_SEC,UTILIZED.device_sec_cnt,1);
	EEPROM_datawrite(EEPROM_DEVICE_MIN,UTILIZED.device_min_cnt,1);
	EEPROM_datawrite(EEPROM_DEVICE_HRS,UTILIZED.device_hrs_cnt,2);

	EEPROM_datawrite(EEPROM_BATTERY_SEC,UTILIZED.battery_sec_cnt,1);
	EEPROM_datawrite(EEPROM_BATTERY_MIN,UTILIZED.battery_min_cnt,1);
	EEPROM_datawrite(EEPROM_BATTERY_HRS,UTILIZED.battery_hrs_cnt,2);

	EEPROM_datawrite(EEPROM_RUNNING_SEC,UTILIZED.running_sec_cnt,1);
	EEPROM_datawrite(EEPROM_RUNNING_MIN,UTILIZED.running_min_cnt,1);
	EEPROM_datawrite(EEPROM_RUNNING_HRS,UTILIZED.running_hrs_cnt,2);

}

void EEPROM_usedhrs_read(void)
{
			UTILIZED.device_sec_cnt=EEPROM_dataread(EEPROM_DEVICE_SEC,1);
			UTILIZED.device_min_cnt=EEPROM_dataread(EEPROM_DEVICE_MIN,1);
			UTILIZED.device_hrs_cnt=EEPROM_dataread(EEPROM_DEVICE_HRS,2);
			if(UTILIZED.device_sec_cnt<1 || UTILIZED.device_min_cnt<1 ||
					UTILIZED.device_hrs_cnt<1 )
			{
				UTILIZED.device_sec_cnt=0;
				UTILIZED.device_min_cnt=0;
				UTILIZED.device_hrs_cnt=0;
			}

			UTILIZED.battery_sec_cnt=EEPROM_dataread(EEPROM_BATTERY_SEC,1);
			UTILIZED.battery_min_cnt=EEPROM_dataread(EEPROM_BATTERY_MIN,1);
			UTILIZED.battery_hrs_cnt=EEPROM_dataread(EEPROM_BATTERY_HRS,2);
			if(UTILIZED.battery_sec_cnt>60 || UTILIZED.battery_min_cnt>60 ||
							UTILIZED.battery_hrs_cnt>10000 )
			{
				UTILIZED.battery_sec_cnt=0;
				UTILIZED.battery_min_cnt=0;
				UTILIZED.battery_hrs_cnt=0;
			}

			UTILIZED.running_sec_cnt=EEPROM_dataread(EEPROM_RUNNING_SEC,1);
			UTILIZED.running_min_cnt=EEPROM_dataread(EEPROM_RUNNING_MIN,1);
			UTILIZED.running_hrs_cnt=EEPROM_dataread(EEPROM_RUNNING_HRS,2);

			if(UTILIZED.running_sec_cnt>60 || UTILIZED.running_min_cnt>60 ||
										UTILIZED.running_hrs_cnt>10000 )
			{
				UTILIZED.running_sec_cnt=0;
				UTILIZED.running_min_cnt=0;
				UTILIZED.running_hrs_cnt=0;
			}


			UTILIZED.factory_used_hrs=EEPROM_dataread(EEPROM_FACTORY_HRS,2);
			if(UTILIZED.factory_used_hrs>10000)
			{
				UTILIZED.factory_used_hrs=0;
			}
}

