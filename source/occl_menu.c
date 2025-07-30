/*
 * occl_menu.c
 *
 *  Created on: 02-Mar-2024
 *      Author: 7303
 */

#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Brand.h"
#include "Drug_List.h"
#include "Occlusion.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include "Main_Menu.h"
//#include "../Menu_Setting/Main_Menu.h"
#include"Driver_I2C.h"
#include"Driver_Timer.h"
#include "Power_off.h"
#include "sound.h"
#include"TFT_String.h"

/**************************************************************************
                          Global Variables
***************************************************************************/

uint8_t G_occl_unit=0;
//uint8_t G_occl_unit1=0;

uint8_t G_occl_level=0;
extern uint16_t  TFT_String_align(uint16_t x_start,uint16_t x_end,uint16_t YPOS,uint8_t *popup_message,uint8_t center,uint32_t back_col,uint32_t txt_col) ;
extern uint8_t auto_lock_time;  //ZOOZ_1

extern uint8_t auto_exit_mode;//common_bug   
const char* occl_menu[3]={"Pressure Unit","Occlusion Lvl"};
const char* occl_values[3][10]={{"mmHg",  "PSI",  "kPa",  "BAR"},
								{"1","2","3","4","5","6","7","8","9","10"}
							   };

const char* unit_value[4][10]={{"100","200","300","400","500","600","700","800","900","1000"},
							   {"2","4","6","8","10","12","14","16","18","20"},
							   {"13.3","26.6","39.9","53.3","66.6","79.9","93.3","106.6","119.9","132.2"},
							   {"0.13","0.26","0.39","0.5","0.6","0.7","0.9","1.0","1.1","1.3"}
							  };


extern uint8_t ON_Flag;

/**************************************************************************
 *Function Name :void Occl_Screen()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to enter the Bolus option
***************************************************************************/

void Occl_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t occl_variable=1;
	uint8_t edit_flag;
//	G_occl_level=5;
//  uint8_t time_interval=45;//common_bug
	uint8_t occl_level=0;//common_bug
	uint8_t occl_unit=0;//common_bug

  //uint8_t occl_ent_flg=0;
	edit_flag=0;
	occl_level=G_occl_level;//common_bug
	occl_unit=G_occl_unit;//common_bug
  //uint8_t occl_ent_flg=0;
	edit_flag=0;

	 POW.power_status_flag=1;
	// auto_lock_time=30;//common_bug  //ZOOZ_1
while(KEY.value)
{

}

	while(occl_variable)
	{

		Power_off();
		if(!ON_Flag) 	//MAIN.main_power_status_flag == 1 )
		{
			G_occl_level=occl_level;//common_bug
			G_occl_unit=occl_unit;//common_bug
			break;//23-03
		}

	   else if(POW.power_status_flag)
	   {
		   POW.power_status_flag=0;//26-03
		   Occl_Frame();
		   Occl_Opt_Frame(page_no,option_no,2);
		   //scroll_bar(page_no,option_no,2);
		   if(edit_flag)
		   {
			 if(option_no==0)
			 {
					TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(58,280,1,LIGHT_GRAY,BLACK);
			 }
			 else if(option_no==1)
			 {
				TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,190,0,LIGHT_GRAY,BLACK);
				TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,280,1,LIGHT_GRAY,BLACK);
			 }


		   }

	   }
		if(KEY.value)//common_bug  //ZOOZ_1
		{
		//	while(KEY.value);
			auto_lock_time=AUTO_EXIT_TIME;
		}
		else if(!auto_lock_time)
		{
		//	auto_lock_time=45;
			G_occl_level=occl_level;//common_bug
			G_occl_unit=occl_unit;	//common_bug
			break;

		}							//common_bug


		switch(KEY.value)
		{
			case DOWN_KEY :			//DOWN_KEY
			//	beep_sound();
			if(!edit_flag)
			{
				if(!option_no)
				{
					option_no=1;
				}
				else
				{
					option_no=0;
				}

				if(option_no!=prev_option)
				{
//					if(option_no!=0)
//					{
						Occl_cursor(prev_option,option_no,page_no,occl_menu);
				//	}
					prev_option = option_no;
				}
			}
//			  scroll_bar(page_no,option_no,2);
			while(KEY.value);
			break;

			case UP_KEY:			//UP_KEY
			//	beep_sound();
			if(!edit_flag)
			{
				if(option_no)
			{
				option_no=0;
			}
			else
			{
				option_no=1;
			}
				if(option_no!=prev_option)
				{

						Occl_cursor(prev_option,option_no,page_no,occl_menu);

					prev_option = option_no;
				}
			}
//			  scroll_bar(page_no,option_no,2);
			while(KEY.value);
			break;

			case LEFT_KEY:				 //LEFT_KEY:
		//		beep_sound();
				if(edit_flag)
			{
				occl_subsetting( option_no,0);
			}

			while(KEY.value);
			break;

			case RIGHT_KEY:				 //RIGHT_KEY:
//				beep_sound();
				if(edit_flag)
			{
				occl_subsetting( option_no,1);
			}


			while(KEY.value);
			break;

			case OK_KEY: 					 //OK_KEY
			//	beep_sound();
			if(!edit_flag)
			{
				if(option_no==0)
				 {
					TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(58,280,1,LIGHT_GRAY,BLACK);
				 }
				else if(option_no==1)
				 {
					TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,190,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,280,1,LIGHT_GRAY,BLACK);
//					TFT_ST7789V_left_Right_arrow_14_arial(163,75,0,LIGHT_GRAY,BLACK);
//					TFT_ST7789V_left_Right_arrow_14_arial(163,225,1,LIGHT_GRAY,BLACK);
				 }

				edit_flag = 1;
			}
			else
			{
				edit_flag = 0;

					if(option_no==0)
					{
						TFT_ST7789V_left_Right_arrow_14_arial(58,190,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(58,280,1,LIGHT_GRAY,LIGHT_GRAY);

//						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3,190,0,MAIN_FRAME_COL,MAIN_FRAME_COL);
//						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3,270,1,MAIN_FRAME_COL,MAIN_FRAME_COL);

//						TFT_ST7789V_left_Right_arrow_14_arial(163,75,0,MAIN_FRAME_COL,MAIN_FRAME_COL);
//						TFT_ST7789V_left_Right_arrow_14_arial(163,225,1,MAIN_FRAME_COL,MAIN_FRAME_COL);
					}
					else if(option_no==1)
					{
//						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,190,0,MAIN_FRAME_COL,MAIN_FRAME_COL);
//						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,270,1,MAIN_FRAME_COL,MAIN_FRAME_COL);

						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,190,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(50+10+3+30,280,1,LIGHT_GRAY,LIGHT_GRAY);
//						TFT_ST7789V_left_Right_arrow_14_arial(163,75,0,LIGHT_GRAY,LIGHT_GRAY);
//						TFT_ST7789V_left_Right_arrow_14_arial(163,225,1,LIGHT_GRAY,LIGHT_GRAY);
					}

//					Occl.PSI_level=(G_occl_level+1)*2;
//					Occl.dynamic_ratio=54/Occl.PSI_level;

			//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);


//				Font_Select(ARIAL_14);
//								TFT_Number(100,120,G_occl_unit,YELLOW,RED);
//
//
//delay_secs(3);

				EEPROM_datawrite(EEPROM_G_occl_unit ,G_occl_unit,1);//22-03
				EEPROM_datawrite(EEPROM_G_occl_level ,G_occl_level,1);
				occl_level=G_occl_level;//common_bug
				occl_unit=G_occl_unit;//common_bug

			}


			while(KEY.value);
			break;

			case BACK_KEY:
				//BACK_KEY
//				beep_sound();

				if(edit_flag)
				{
					G_occl_level=occl_level;//common_bug  //ZOOZ_1
					G_occl_unit=occl_unit;//common_bug
					//edit_flag=0;
				}

				{
					EEPROM_datawrite(EEPROM_G_occl_unit ,G_occl_unit,1);//22-03
					EEPROM_datawrite(EEPROM_G_occl_level ,G_occl_level,1);
				}
				//if(!edit_flag)
				{
					occl_variable=0;
				}
//			Rectangle(3,36,300,169,MAIN_FRAME_COL);
			while(KEY.value);
			break;
		}
	}
//return 0;
}
/**************************************************************************
 *Function Name :void Occl_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for Occlusion main frame
***************************************************************************/

void Occl_Frame(void)
{
	Frame_layout3(1,2,(uint8_t*)"Occlusion setting",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}

/**************************************************************************
 *Function Name :void Occl_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/
void Occl_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
	uint16_t occl[3]={G_occl_unit,G_occl_level};
	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint8_t loop_count=0;

	for(loop_count=0;loop_count<=1;loop_count++)
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;

			Rectangle(9,48+(38*(start_opt)),300,31,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		value = occl[loop_count];

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
		,(uint8_t*)occl_menu[loop_count],back_colour,text_colour);

//		TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
//		,(uint8_t*)":",back_colour,text_colour);
		if(loop_count==0)
			TFT_String(180+25,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)occl_values[loop_count][value],back_colour,text_colour);
		else
		{
			TFT_String(180+25+25,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)occl_values[loop_count][value],back_colour,text_colour);
		}
//else
//{
//		TFT_String(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
//		(uint8_t*)occl_values[1][0],back_colour,text_colour);
//}
		if(start_opt==1)
		{
			Font_Select(ARIAL_BOLD_14);
			TFT_ST7789V__rectangle_curve(60+20,130,50,170-20,0,LIGHT_GRAY);

	//		TFT_String(94,160,(uint8_t*)unit_value[G_occl_unit][G_occl_level],back_colour, text_colour);//
	//		TFT_String(150,160,(uint8_t*)occl_values[0][G_occl_unit],back_colour, text_colour);//


			TFT_String(94-4,155-10,(uint8_t*)unit_value[G_occl_unit][G_occl_level],back_colour, text_colour);//
			TFT_String(155-4,155-10,(uint8_t*)occl_values[0][G_occl_unit],back_colour, text_colour);//
		}
	}

}

/**************************************************************************
 *Function Name :void Occl_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/
void Occl_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
{
	 uint8_t occlusion[3]={G_occl_unit,G_occl_level};
	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint32_t back_colour1 = 0;
	uint32_t text_colour1 = 0;


	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option;
		}

		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}

		//TFT_ST7789V__rectangle_curve(135,50,200,60,0,back_colour);


		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);
		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
		(uint8_t*)array[opt_no],back_colour,text_colour);

		if(opt_no==0)
				TFT_String(180+25,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
				(uint8_t*)occl_values[opt_no][occlusion[opt_no]],back_colour,text_colour);
		else
		{
			TFT_String(180+25+25,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
				(uint8_t*)occl_values[opt_no][occlusion[opt_no]],back_colour,text_colour);
		}
//		if(loop_count==0)
//				TFT_String(180+25,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
//				(uint8_t*)occl_values[loop_count][value],back_colour,text_colour);
//		else
//		{
//				TFT_String(180+25+25,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
//				(uint8_t*)occl_values[loop_count][value],back_colour,text_colour);
//		}
//		TFT_String(180,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)
//		,(uint8_t*)":",back_colour,text_colour);
	}


	if(!opt_no)
		{
			back_colour1 = LIGHT_GRAY;
			text_colour1 = LIGHT_GRAY;
		}
		else
		{
			back_colour1 = LIGHT_GRAY;
			text_colour1 = MAIN_FRAME_COL;
		}

	{
		if(opt_no==1)
		{
		Font_Select(ARIAL_BOLD_14);
	//	TFT_ST7789V__rectangle_curve(60,145,50,190,0,back_colour1);
		TFT_ST7789V__rectangle_curve(60+20,130,50,170-20,0,LIGHT_GRAY);

		TFT_String(94-4,155-10,(uint8_t*)unit_value[G_occl_unit][G_occl_level],back_colour1, text_colour1);//
		TFT_String(155-4,155-10,(uint8_t*)occl_values[0][G_occl_unit],back_colour1, text_colour1);//
		}
		else
		{
			TFT_ST7789V__rectangle_curve(60+20,130,50,170-20,0,MAIN_FRAME_COL);
		}
	}

}



void occl_subsetting(uint8_t option_no, uint8_t INC_flag)
{

	uint8_t* occl_data[3] = {&G_occl_unit,&G_occl_level};
	uint8_t max_value[2] =  {3,9};
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;

	cur_value	= *occl_data[option_no];

	pre_value	= cur_value;

	if(INC_flag)
	{
		if(cur_value < max_value[option_no])
		{
			cur_value++;
		}
		else
		{
			if(option_no==0)
		{
			cur_value=0;
		}
		else
		{
			cur_value=0;
		}
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
		if(option_no==0)
		{
			cur_value=3;
		}
		else
		{
			cur_value=9;
		}
	}
	}
	if(pre_value != cur_value)
	{
		if(option_no==0)
		{
		Font_Select(ARIAL_BOLD_14);
//		TFT_String(180+25,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
//		(uint8_t*)occl_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);
//
//		Font_Select(ARIAL_BOLD_14);
//		TFT_String(180+25,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
//		(uint8_t*)occl_values[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);


		TFT_String_align(190,280,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
			(uint8_t*)occl_values[option_no][pre_value],0,LIGHT_GRAY,LIGHT_GRAY);

		TFT_String_align(190,280,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)occl_values[option_no][cur_value],0, LIGHT_GRAY,MAIN_FRAME_COL);

//		Font_Select(ARIAL_BOLD_14);
//
//
//		 TFT_String_align(190,280,80,(uint8_t *)"Info",0,WHITE,RED);



		}
		else
		{
			Font_Select(ARIAL_BOLD_14);
			TFT_String(180+25+25,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
			(uint8_t*)occl_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(180+25+25,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
			(uint8_t*)occl_values[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
		}
	}

	if(option_no)
	{
//		TFT_ST7789V__rectangle_curve(135,50,200,60,0,LIGHT_GRAY);
//		TFT_String(160,150,(uint8_t *)"mmHg",LIGHT_GRAY,BLACK);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(94-4,155-10,(uint8_t*)unit_value[G_occl_unit][pre_value],LIGHT_GRAY,LIGHT_GRAY);
		TFT_String(94-4,155-10,(uint8_t*)unit_value[G_occl_unit][cur_value],LIGHT_GRAY,MAIN_FRAME_COL);
	}

	*occl_data[option_no] = cur_value;

}
