/*
 * patient_para_sett.c
 *
 *  Created on: Feb 6, 2025
 *      Author: Welcome
 */
#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "Main_Menu.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Brand.h"
#include "Occlusion.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include"Driver_I2C.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include  "sound.h"
#include "Dosage_mode.h"

uint8_t G_pat_para_opt_no=0;

const char* pat_para[3]={"General","BSA","Weight"};
extern uint8_t auto_lock_time;
extern uint8_t home_flag;
extern uint8_t ON_Flag;

uint8_t para_setting_screen()
{
	uint8_t page_no=0;
		uint8_t option_no=0;
		uint8_t prev_option = 0;
		uint8_t exit_flag=1;
		uint8_t edit_flag=0;
		uint8_t check_flag=0;
		uint8_t end_opt_no=2;


		//auto_lock_time=AUTO_EXIT_TIME;
	//  uint8_t time_interval=30;//common_bug //ZOOZ_1

		//home_flag=0;
		POW.power_status_flag=1;
	//  auto_lock_time=30;//common_bug //ZOOZ_1

		//while(KEY.value);
		while(exit_flag)
		{
			Power_off();

			if(!ON_Flag)
			{
				break;//26-03
			}

			else if(POW.power_status_flag)
			{
				POW.power_status_flag=0;//26-03
				pat_para_Frame();
				pat_para_opt_Frame(page_no,option_no,end_opt_no);
				//scroll_bar(page_no,option_no,total_option);  //23-03
				while(KEY.value);//28-03//ZOOZ_1
			}

//				if(KEY.value)//common_bug//ZOOZ_1
//				{
//				//	while(KEY.value);
//					auto_lock_time=AUTO_EXIT_TIME;
//				}
//
//				 if(!auto_lock_time)
//				{
//					break;
//				}//common_bug

			switch(KEY.value)
			{
			case DOWN_KEY :			//DOWN_KEY
						//	beep_sound();

						if(option_no<2)
						{
							option_no++;
						}
						else
						{
							option_no=0;
						}

						if(option_no!=prev_option)
						{
		//					if(option_no!=0)
		//					{
								pat_para_cursor(prev_option,option_no,page_no,pat_para);
						//	}
							prev_option = option_no;
						}

		//			  scroll_bar(page_no,option_no,2);
					while(KEY.value);
					break;

					case UP_KEY:


						if(option_no)
					{
						option_no--;
					}
					else
					{
						option_no=2;
					}
						if(option_no!=prev_option)
						{

							pat_para_cursor(prev_option,option_no,page_no,pat_para);

							prev_option = option_no;
						}

					while(KEY.value);
					break;

					case OK_KEY:
						//beep_sound();
						G_pat_para_opt_no=option_no;
						if(option_no==0)
						{
							check_flag=1;
							exit_flag=0;

						}
						else if(option_no==1)											//REW0
						{
							check_flag=pat_height_fun(option_no);
							if(!check_flag)
							{
								exit_flag=1;
								POW.power_status_flag=1;
							}
							else
							{
								exit_flag=0;
							}


						}
						else
						{
							check_flag=pat_weight_fun();
							if(!check_flag)
							{
								exit_flag=1;
								POW.power_status_flag=1;
							}
							else
							{
								exit_flag=0;
							}
						}


//						if(check_flag)
//						{
//
//							exit_flag=0;
//
//						//	POW.power_status_flag=1;
//						}
//						else
//						{
//							exit_flag=1;
//							if(auto_lock_time)
//							{
//							POW.power_status_flag=1;
//							}
//						}
//						Dosage_opt_Frame(page_no,option_no,3);
//						scroll_bar(page_no,option_no,5);//23-03
						//27-03

						while(KEY.value);
						break;

					case BACK_KEY:
					//	if(!G_bootup_flag)
					//	{
							//beep_sound();
							check_flag=0;
							exit_flag=0;
							while(KEY.value);
							break;

			}

		}
		return check_flag;

}



void pat_para_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page,const char* array[3]  )
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
					text_colour = LIGHT_GRAY;
					opt_no		= prev_option;//0
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
		//			Font_Select(ARIAL_14);
			Font_Select(ARIAL_BOLD_14);
			TFT_String(9,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)array[opt_no],back_colour,text_colour);



	}
}


void pat_para_layout()
{
	CENTER_SCR(MAIN_FRAME_COL);  //center green

	border_lines();

	separation_lines(1,3);
}

void pat_para_Frame(void)
{

	Frame_layout(1,3,(uint8_t*)"Patient Parameter",(uint8_t*)"OKAY",(uint8_t*)"to confirm");

}

/**************************************************************************
 *Function Name :void Main_Menu_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/



void pat_para_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

    uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;

			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)pat_para[loop_count],back_colour,text_colour);




	}

}
