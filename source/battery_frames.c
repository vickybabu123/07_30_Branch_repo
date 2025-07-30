#include <FR_CALC.h>
#include <Boot_up.h>
#include <STEPPER.h>
#include "LPC55S16.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Keypad.h"
#include "GPIO.h"
#include "Flowrate_Screen.h"
#include "Driver_Timer.h"
#include "fsl_clock.h"
#include "PWM.h"
#include "Main_screen.h"
#include "Occlusion.h"
#include "Main_menu.h"
#include "Driver_ADC.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include "Driver_PINT.h"
#include "Driver_I2C.h"
#include "Dosage_mode.h"
#include "Alarm.h"
#include "battery.h"
#include "math.h"

extern uint8_t _1sec_delay;
extern uint8_t home_flag;
extern uint8_t home_return_flag;
extern uint8_t G_Brand_back_flag;
extern uint16_t Dosage_Alignments(uint8_t *asciichar);
extern uint8_t war_status[5];

uint8_t temp_batt_level=0;
uint8_t B_xpos=0;
uint8_t prev_B_xpos=0;

uint16_t prev_battery_per=0;
uint16_t prev_battery_lvl=0;

void Battery_symbol(void)
{

	Rectangle(270,12,30,1,DARK_TEAL);			//Battery top line
	Rectangle(271,29,30,1,DARK_TEAL);           //Battery Bottom Line
	Rectangle(270,12,2,18,DARK_TEAL);           //First Left side Line
	Rectangle(300,12,2,18,DARK_TEAL);           //RIGHT side Line

	Rectangle(273,14,26,14,WHITE);                  //INNER FILL

  //Rectangle(267,14,temp_batt_level,14,LIGHT_GREEN);

	Rectangle(292+5+6,6+11,3,8,DARK_TEAL);            //Battery outer rect
	Font_Select(ARIAL_12);

//	if(BATT.percent)
//	{
//		TFT_Number(218,15,BATT.percent,LIGHT_TURQUI,DARK_TEAL_1);
//		prev_battery_per=BATT.percent;
//	}
//	else
//	{
//		TFT_Number(218,15,prev_battery_per,LIGHT_TURQUI,LIGHT_TURQUI);
//		TFT_Number(225,15,BATT.percent,LIGHT_TURQUI,DARK_TEAL_1);
//
//	}
//	Font_Select(ARIAL_12);
//	TFT_String(210+6+2+24,9+4+2,(uint8_t* )"%",LIGHT_TURQUI,DARK_TEAL_1);
//	battery_level(14,2);

}

void battery_charge_lvl(uint16_t xpos,uint8_t batt_lv1)
{

	    char BR_buffer[6];

//	    Font_Select(ARIAL_14);
//	    TFT_Number(170,70,batt_lv1,NAVY,RED);
//	    TFT_Number(170,90,prev_battery_lvl,NAVY,RED);
//      TFT_Number(0,50,21,NAVY,RED);

	    if(batt_lv1>=prev_battery_lvl)
	    {
	    	if(BATT.percent<=10)
			{
				Rectangle(273,14,(batt_lv1),14,RED);             //INNER FILL
			}
	    	else
	    	{
		    	Rectangle(273,14,(batt_lv1),14,LIGHT_GREEN);     //INNER FILL
	    	}
	    }

	    else if(batt_lv1<=prev_battery_lvl)
	    {
	    	if(BATT.percent<=10)
			{
				Rectangle(273,14,(batt_lv1),14,RED);                  //INNER FILL
			}
	    	Rectangle(273+batt_lv1,14,(26-batt_lv1),14,WHITE);                  //INNER FILL
	    }

	    prev_battery_lvl = batt_lv1;
	    TFT_String(215+prev_B_xpos-3,15,(uint8_t* )"%",LIGHT_TURQUI,LIGHT_TURQUI);

		sprintf(BR_buffer,"%.d",BATT.percent);         //27-03
		B_xpos=Dosage_Alignments((uint8_t*)BR_buffer);
		prev_B_xpos=B_xpos;

		if(BATT.percent)
		{
			TFT_String(240,15,(uint8_t* )"%",LIGHT_TURQUI,LIGHT_TURQUI);
			Font_Select(ARIAL_12);
			Rectangle(220,15,25,15,LIGHT_TURQUI);
			TFT_Number(215,15,BATT.percent,LIGHT_TURQUI,DARK_TEAL_1);   //Printing all the numbers except zero
			prev_battery_per=BATT.percent;
			TFT_String(215+B_xpos,15,(uint8_t* )"%",LIGHT_TURQUI,DARK_TEAL_1);
		}

		else
		{
			//Rectangle(212,15,25,15,LIGHT_TURQUI);
			TFT_Number(218,15,prev_battery_per,LIGHT_TURQUI,LIGHT_TURQUI);//Printing only zero
			TFT_Number(225,15,BATT.percent,LIGHT_TURQUI,DARK_TEAL_1);
			Font_Select(ARIAL_12);
			TFT_String(240,15,(uint8_t* )"%",LIGHT_TURQUI,DARK_TEAL_1);
		}

//	        Font_Select(ARIAL_14);
//			TFT_Number(170,70,batt_lv1,NAVY,RED);
//
//			Font_Select(ARIAL_14);
//			TFT_Number(170,90,prev_battery_lvl,NAVY,RED);
//
//			Font_Select(ARIAL_14);
//			TFT_Number(170,110,BATT.percent,NAVY,RED);

}

//void battery_level(uint16_t xpos,uint8_t batt_lv1)
//{
//
//	uint32_t color[4][3]={
//							{RED,	MAIN_FRAME_COL,	MAIN_FRAME_COL},
//							{ORANGE,MAIN_FRAME_COL,	MAIN_FRAME_COL},
//							{ORANGE,ORANGE,	MAIN_FRAME_COL},
//							{LIGHT_GREEN,LIGHT_GREEN,LIGHT_GREEN}
//						 };
//
//
//		Rectangle(267,14,26,14,WHITE);              //INNER FILL
//		Rectangle(267,14,batt_lv1,14,RED);
//
////  TFT_Number(150,70,batt_lv1,YELLOW,BLACK);
////	Rectangle(264,2+xpos,4,12,GREEN);			    //First  Rectangle Box
////	Rectangle(273,2+xpos,4,12,GREEN);			    //Second Rectangle Box
////	Rectangle(282,2+xpos,4,12,GREEN);               //Third Rectangle Box
//
//}

void battery_warning(uint8_t batt_lv1)
{

	if((!BATT.mode)&&(home_flag))
	{
			if((BATT.percent>=15) && (BATT.percent<25))
			{
				if(!BATT.low)
				{
					 BATT.low=1;
					 war_status[5]=1;
					 WARN_SOUND(3,1,3);
					 delay_secs(3);
					 Warning_Scr_Erase(0);
				}
			}

			else if((BATT.percent>=10) && (BATT.percent<15))
			{
				if(!BATT.critical)
				{
					BATT.low=0;
					BATT.critical=1;
					WARN_SOUND(3,1,3);
				}
			}
			else
			{
				 war_status[5]=0;
				 BATT.low=0;
			}

		if(!BATT.mode)
		{
				if(BATT.percent<10)
				{
					if(!BATT.empty)
					{
						BATT.low=0;
						BATT.empty=1;
		//				Alarm(BAT_CRITICAL);
					}
				}
		}

	}
}

void Battery_indicate(void) //23-04
{
	uint8_t batt_lv1=0;

	batt_lv1=BATT.percent/4;
	batt_lv1=batt_lv1+2;
	temp_batt_level=batt_lv1;

	if(batt_lv1==2)
	{
		batt_lv1=0;
	}

	battery_warning(batt_lv1);
	Font_Select(ARIAL_14);

	if(home_flag)
	{
		if(!BATT.mode)
		{
			if(!_1sec_delay || (G_Brand_back_flag))
			{
				battery_charge_lvl(BATTERY_POS,batt_lv1);
				_1sec_delay=1;
			}

		}
	}
}
