/*
 * kvo_menu.c
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
#include "FR_CALC.h"
#include "Alarm.h"
#include "Power_off.h"
#include "sound.h"
#include "Driver_Timer.h"
#include <Boot_up.h>
#include "Linear_Header.h"

uint32_t G_kvo=1;//22-03
uint8_t G_kvo_FR=0;
uint32_t G_KVO_fr=1;
uint16_t G_KVO_set_fr=0;
uint8_t G_KVO_run_flag=0;
extern uint8_t auto_lock_time;//common_bug
extern uint8_t disc;
extern uint8_t release;
extern volatile uint8_t normal_run;


const char* kvo_menu[3]={"KVO","KVO Rate"};
const char* kvo_values[2][2]={{"OFF","ON"},//22-03
							  {0}
							 };
extern uint8_t ON_Flag;
extern uint8_t G_Bolus_target_flag;

void KVOmodeRun(void);//22-03
void KVO_run(void);


/**************************************************************************
 *Function Name :void KVO_Screen()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to enter the KVO option
***************************************************************************/

void KVO_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t kvo_variable=1;
	uint8_t end_op_no=3;//22-03
//  uint8_t kvo_ent_flg=0;
	uint8_t exit_flag=0;
	uint8_t edit_flag=0;

	uint8_t kvo_fr=0;//common_bug
	uint8_t kvo_sts=0;//common_bug

//	uint8_t time_interval=45;//common_bug
	auto_lock_time=AUTO_EXIT_TIME;//common_bug

	kvo_sts=G_kvo;//common_bug
	kvo_fr=G_KVO_fr;//common_bug
	POW.power_status_flag=1;
	while(KEY.value);

	while(kvo_variable)
	{
		Power_off();

		if(!ON_Flag) 	//MAIN.main_power_status_flag == 1 )
		{
			G_kvo=kvo_sts;//common_bug
			G_KVO_fr=kvo_fr;//common_bug
			break;//23-03
		}
		       else if(POW.power_status_flag)
			   {
				   POW.power_status_flag=0;//26-03
				   KVO_Frame();
				   KVO_Opt_Frame(page_no,option_no,end_op_no);//22-03

				  	if(edit_flag)
					{
						TFT_ST7789V_left_Right_arrow_14_arial(56,162,0,LIGHT_GRAY,BLACK);
						TFT_ST7789V_left_Right_arrow_14_arial(56,230,1,LIGHT_GRAY,BLACK);

						edit_flag = 1;
					}
					else
					{
						edit_flag = 0;
						if(option_no==0)
						{
						TFT_ST7789V_left_Right_arrow_14_arial(56,162,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(56,230,1,LIGHT_GRAY,LIGHT_GRAY);
						}
					//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);
	//					TFT_ST7789V_lcd_drawcircle1(140,223,15,MAIN_FRAME_COL,MAIN_FRAME_COL);
	//
	//					Font_Select(ARIAL_9);
	//					TFT_String(130,218,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);

					}

			   }
			   	if(KEY.value)  //ZOOZ_1
				{
				//	while(KEY.value);
			      auto_lock_time=AUTO_EXIT_TIME;					//time_interval;//common_bug
				}
				else if(!auto_lock_time)
				{
				//	auto_lock_time=45;

					G_kvo=kvo_sts;//common_bug
					G_KVO_fr=kvo_fr;//common_bug
					break;
				 //   return;
				//	occl_variable=0;
				}//common_bug
			   
			   
		switch(KEY.value)
		{
			case DOWN_KEY :   //DOWN_KEY
				//beep_sound();
				if(!edit_flag)
			{
				if((!option_no)&&(G_kvo==1))
				{
					option_no=1;
				}
				else
				{
					option_no=0;
				}

				if(option_no!=prev_option)
				{
				//	if(option_no!=0)
				//	{
						KVO_cursor(prev_option,option_no,page_no,kvo_menu);
				//	}
					prev_option = option_no;
				}
			}

			while(KEY.value);
			break;

			case UP_KEY:     //UP
				//beep_sound();
				if(!edit_flag)
			{
				if((option_no)&&(G_kvo==1)) //ZOOZ_1
				{
					option_no=0;
				}
				else
				{
					if(G_kvo==1)
					{
						option_no=1;
					}
				}

				if(option_no!=prev_option)
				{
				//	if(option_no<1)
				//	{
						KVO_cursor(prev_option,option_no,page_no,kvo_menu);
				//	}
					prev_option = option_no;
				}
			}

			while(KEY.value);
			break;

			case RIGHT_KEY:				 //LEFT_KEY:

				if(edit_flag)
			{
				kvo_subsetting( option_no,page_no,1);

			}
			while(KEY.value);
			break;

			case LEFT_KEY:				 //RIGHT_KEY:

				if(edit_flag)
			{
				kvo_subsetting( option_no,page_no,0);

			}
			while(KEY.value);
			break;

			case OK_KEY: 					 //OK_KEY
				//beep_sound();
				if(!edit_flag&&option_no==0)
				{
					TFT_ST7789V_left_Right_arrow_14_arial(56,162,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(56,230,1,LIGHT_GRAY,BLACK);

					edit_flag = 1;
				}
				else
				{
					edit_flag = 0;
					if(option_no==0)
					{
						TFT_ST7789V_left_Right_arrow_14_arial(56,162,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(56,230,1,LIGHT_GRAY,LIGHT_GRAY);
					}
						//Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);
				}

				if(option_no==1)
				{
					exit_flag=Flow_rate_screen(&G_KVO_fr,0,50,1);
					if(auto_lock_time)
									{
										POW.power_status_flag=1;
									}
					{
							page_no=1;//22-03
							option_no=1;
							end_op_no=3;
							POW.power_status_flag=1;//27-03

					}
				}

				EEPROM_datawrite(EEPROM_G_kvo,G_kvo,1);
				EEPROM_datawrite(EEPROM_G_KVO_fr,G_KVO_fr,2);//22-03

			while(KEY.value);
			break;

			case BACK_KEY: 					 //BACK_KEY
				//beep_sound();//22-03

			if(edit_flag)
			{
				G_kvo=kvo_sts;//common_bug //ZOOZ_1
				G_KVO_fr=kvo_fr;//common_bug
			}
			else
			{
				EEPROM_datawrite(EEPROM_G_kvo,G_kvo,1);
				EEPROM_datawrite(EEPROM_G_KVO_fr,G_KVO_fr,2);//22-03

			}
			kvo_variable=0;
		//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
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

void KVO_Frame()
{
	Frame_layout3(1,2,(uint8_t*)"KVO",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);

}

/**************************************************************************
 *Function Name :void KVO_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void KVO_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
	uint16_t kvo_data[3]={G_kvo,G_KVO_fr};
	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;

	for(loop_count=0;loop_count<=1;loop_count++)
	{
		if(loop_count == start_opt)//0==0
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

		value = kvo_data[loop_count];

//		Font_Select(ARIAL_14);
//		TFT_String(INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),
//		9,(uint8_t*)kvo_menu[loop_count],back_colour,text_colour);
//		TFT_String(INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),
//		150,(uint8_t*)":",back_colour,text_colour);
//		TFT_String(INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),170,
//		(uint8_t*)kvo_values[loop_count][value],back_colour,text_colour);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)kvo_menu[loop_count],back_colour,text_colour);

//		TFT_String(150,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);

		if(loop_count==0)
			{


			TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)kvo_values[loop_count][value],back_colour,text_colour);
			  Font_Select(ARIAL_14);

			}
		else
			{
			if(!G_kvo)
			{
				TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
				value,MAIN_FRAME_COL,THIN_ICE);
				TFT_String(255-20,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,THIN_ICE);

				}
			else
			{
			TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
			value,back_colour,text_colour);
			TFT_String(255-20,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",back_colour,text_colour);

			}
	}

}

}
/**************************************************************************
 *Function Name :void KVO_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void KVO_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
{
	//const uint16_t kvo_print[3]={G_kvo,G_KVO_fr};
	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint8_t value=0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour =  MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option; //1
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}
		if(opt_no==0)
		{
			value = G_kvo;
		}
		else
		{
			value=G_KVO_fr;
		}

		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);


		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
		(uint8_t*)array[opt_no],back_colour,text_colour);

		TFT_String(150,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);




		if(opt_no==0)
		{
			TFT_String(180,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
			(uint8_t*)kvo_values[0][value],back_colour,text_colour);
		}
		else
		{
//			TFT_Number(INITIAL_TEXT_POS+(1*VER_TEXT_SPACE),200,
//						                value,back_colour,text_colour);
			TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
					value,back_colour,text_colour);
			TFT_String(255-20,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",back_colour,text_colour);


		}



		//		TFT_Number(INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE),200,
		//				(uint8_t*)G_bol_fr,back_colour,text_colour);
	}
}

/**************************************************************************
 *Function Name :void kvo_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
 *Arguments     :uint8_t option_no,uint8_t page_no, uint8_t INC_flag
 *Return        :void
 *Description	:In this function array arguments print as right side
***************************************************************************/


void kvo_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{
	uint32_t* kvo_data[3] = {&G_kvo,&G_KVO_fr};
	uint8_t max_value[2] = {1,0};  //max array elements
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	cur_value	= *kvo_data[option_no];
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
			cur_value=1;
		}
	}

	if(pre_value != cur_value)
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(180,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)kvo_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);
		//Font_Select(ARIAL_14);
		TFT_String(180,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)kvo_values[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
	}

	if(G_kvo)
	{
		TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
		G_KVO_fr,MAIN_FRAME_COL,THIN_ICE);
		TFT_String(255-20,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,THIN_ICE);
	}

	else
	{
		TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
		G_KVO_fr,MAIN_FRAME_COL,LIGHT_GRAY);
		TFT_String(255-20,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,LIGHT_GRAY);
	}

	*kvo_data[option_no] = cur_value;
}

/**************************************************************************
 *Function Name :void edit_subsetting_frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function to change frame footer string
***************************************************************************/
//
void edit_subsetting_frame(void)
{
//	Font_Select(ARIAL_14);
////	Rectangle(0,206,320,34,NEW_FTR_COL);
//	TFT_ST7789V__rectangle_curve(10,190-20,40+10+10,300,50,NEW_FTR_COL);
//	TFT_String(90,210-10,(uint8_t *)"change",NEW_FTR_COL,HDR_TXT_COL);
//	TFT_String(220,210-10,(uint8_t *)"confirm",NEW_FTR_COL,HDR_TXT_COL);
//
//	//Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"                 change                    confirm");
//	TFT_ST7789V_lcd_drawcircle1(20+10,223-10,15,MAIN_FRAME_COL,MAIN_FRAME_COL);    //	 Left Circle
//	TFT_ST7789V_lcd_drawcircle1(60+10,223-10,15,MAIN_FRAME_COL,MAIN_FRAME_COL);    // Right Circle
//	TFT_ST7789V_left_Right_arrow_14_arial(215-10,15+10,0,MAIN_FRAME_COL,WHITE);    //	Left Arrow
//	TFT_ST7789V_left_Right_arrow_14_arial(215-10,59+10,1,MAIN_FRAME_COL,WHITE);    //	Right Arrow
//
//	TFT_ST7789V_lcd_drawcircle1(190+10,223-10,15,MAIN_FRAME_COL,MAIN_FRAME_COL);
//	Font_Select(ARIAL_9);
//	TFT_String(182+10,218-10,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);
}

/************************************************************
  Function Name : KVOmodeRun(void)
  Description   :checks for Target vol,time to run kvo mode
  Input         : Void
  Output        : Void
*************************************************************/

void KVOmodeRun(void)
{
  if((G_kvo) && (G_KVO_fr))// ||(!BOL.run_mode))
  {
	if(G_KVO_fr > MOT.flowrate)
	{
		if( MOT.flowrate <= 50)//12-06-2024
			{
			G_KVO_set_fr = 1;
			}
		else if( MOT.flowrate > 50 &&  MOT.flowrate <= 100)
		{
			G_KVO_set_fr = 5;
		}
		else
		{
			G_KVO_set_fr = 10;
		}
	}
	else G_KVO_set_fr=G_KVO_fr;

	G_KVO_run_flag=1;
	MR_Value_calculate(G_KVO_set_fr);
  }
  if(!G_KVO_run_flag)
  	{
  		Alarm(TAR_REACH);
  	}

  	else
  	{
  		KVO_run();
  	}



  			G_Bolus_target_flag=0;

  }

void KVO_run(void)
{
	uint8_t kvo_var=1;

	Alarm_war_screen(0);

	while(kvo_var)
	{



		  if(POW.power_status_flag)
		 {
           break;
		 }


		 plunger_alarm_check();

		if((release) && (normal_run))
		{
			Alarm(PLUNGER_DIS);
		}
		else if((disc) && (normal_run))
		{
			Alarm(NOT_ENGAGE);
		}
		if((LIN_POT.syr_end_flag==1) && (normal_run) )
		{
			Alarm(SYR_END);
		}
		if(normal_run==1)
		{
			syringe_disp_status(0);
		}


	  Alarm_Running(0);

	  switch(KEY.value)
	  {

		case START_STOP_KEY :

			if(G_KVO_run_flag)
			{
				G_KVO_run_flag=0;
				Timer_disable();
				kvo_var=0;
				normal_run=0;
				POW.power_status_flag=1;
			}

			while(KEY.value);
			break;

		case MUTE_KEY :

//			pwm_sta( alarm_sound.period,0);
//			mute_count=20;

			while(KEY.value);

	}

}
}
