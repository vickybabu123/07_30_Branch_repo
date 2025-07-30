/*
 * standby_fun.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Welcome
 */


#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include"Driver_I2C.h"
#include"Driver_Timer.h"
#include "Power_off.h"
#include "sound.h"
#include "Main_Menu.h"
#include "Driver_ADC.h"
#include "Alarm.h"

const char* near_empty_menu[1]={"Near empty"};
//const char* std_values[2]={"OFF","ON"};
const char* near_empty_value[1][4]={"3 mins ","5 mins ","7 mins ","10 mins"};

uint8_t G_near_empty;



extern uint8_t auto_lock_time;
extern uint8_t ON_Flag;
extern uint8_t home_flag;


void near_empty(void);

void near_empty(void)
{

	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t near_empty_variable=1;
	uint8_t end_op_no=3;
	uint8_t edit_flag=0;
	uint8_t near_empty_sts=0;

	auto_lock_time=AUTO_EXIT_TIME;
	POW.power_status_flag=1;
	while(KEY.value);

	while(near_empty_variable)
		{
			Power_off();

			 if(!ON_Flag)
			       	{
				G_near_empty=near_empty_sts;

			       	break;//23-03
			       	}
			       else if(POW.power_status_flag)
				   {
					   POW.power_status_flag=0;//26-03
					   near_empty_Frame();
					   near_empty_Opt_Frame(page_no,option_no,end_op_no);//22-03
					  	if(edit_flag)
						{
							TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,BLACK);
							TFT_ST7789V_left_Right_arrow_14_arial(58,290,1,LIGHT_GRAY,BLACK);

							edit_flag = 1;
						}
						else
						{
							edit_flag = 0;
							if(option_no==0)
							{
							TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,LIGHT_GRAY);
							TFT_ST7789V_left_Right_arrow_14_arial(58,290,1,LIGHT_GRAY,LIGHT_GRAY);
							}


						}

				   }
				   	if(KEY.value)
					{
				      auto_lock_time=AUTO_EXIT_TIME;
					}
					else if(!auto_lock_time)
					{
						G_near_empty=near_empty_sts;

						break;

					}


			switch(KEY.value)
			{


				case RIGHT_KEY:
//					beep_sound();
					if(edit_flag)
					{
						near_empty_subsetting( option_no,page_no,1);
					}

				while(KEY.value);
				break;

				case LEFT_KEY:
//					beep_sound();
					if(edit_flag)
					{
						near_empty_subsetting( option_no,page_no,0);
					}

				while(KEY.value);
				break;

				case OK_KEY: 					 //OK_KEY
//				beep_sound();
				if(!edit_flag)
				{
					TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(58,290,1,LIGHT_GRAY,BLACK);

					edit_flag = 1;
				}
				else
				{
					edit_flag = 0;


						TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(58,290,1,LIGHT_GRAY,LIGHT_GRAY);


				}
				//EEPROM_datawrite(EEPROM_STANDBY,G_near_empty,1);

				while(KEY.value);
				break;


				case BACK_KEY:
//				beep_sound();
				if(edit_flag)
				{
					G_near_empty=near_empty_sts;
				}
				else
				{
					//EEPROM_datawrite(EEPROM_STANDBY,G_near_empty,1);
				}
				near_empty_variable=0;

				while(KEY.value);
				break;

			}
		}


}


/**************************************************************************
 *Function Name :void KVO_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for KVO main frame
***************************************************************************/

void near_empty_Frame(void)
{
	Frame_layout3(1,1,(uint8_t*)"Near empty time",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);


}

void near_empty_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

//	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;


	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),300,31,back_colour);
		}



		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,(uint8_t*)near_empty_menu[loop_count],back_colour,text_colour);

//		TFT_String(180,INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);

		TFT_String(220-16,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)near_empty_value[0][G_near_empty],back_colour,text_colour);

	}
}

void near_empty_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{
	uint8_t* near_empty_data[2] = {&G_near_empty};
	uint8_t max_value[2] = {3,0};  //max array elements
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	cur_value	= *near_empty_data[option_no];
	pre_value	= cur_value;

	if(INC_flag)
	{       //0            1
		if(cur_value < max_value[option_no])
		{
			cur_value++;
		}

		else
		{
			cur_value=0;
		}
	}
	else
	{
		if(cur_value)
		{
			cur_value--;
		}

		else
		{
			cur_value=max_value[option_no];
		}
	}

	if(pre_value != cur_value)
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(220-16,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)near_empty_value[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);

		TFT_String(220-16,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)near_empty_value[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
	}
	else
	{
//		Font_Select(ARIAL_BOLD_14);
//				TFT_String(220-16,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
//				(uint8_t*)near_empty_value[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);
//
//				TFT_String(220-16,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
//				(uint8_t*)near_empty_value[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
	}

	*near_empty_data[option_no] = cur_value;
}
