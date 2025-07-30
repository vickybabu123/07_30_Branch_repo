/*
 * standby_fun.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Welcome
 */


#include "LPC55S16.h"
#include "Shapes.h"
#include "Boot_up.h"
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
#include "Calibration_Screen.h"


uint8_t standby_time_screen(void);
uint8_t  standby_mode_function(void);
void standby_frame();
void standby_popup();
void standby_logo();
void Standby_Time_disp(uint16_t xpos, uint16_t ypos, uint32_t standby_min,uint32_t standby_sec, uint32_t border_color, uint32_t fill_color);
extern uint16_t  TFT_String_align(uint16_t x_start,uint16_t x_end,uint16_t YPOS,uint8_t *popup_message,uint8_t center,uint32_t back_col,uint32_t txt_col);

const char* std_menu[3]={"Standby mode"};
//const char* std_values[2]={"OFF","ON"};
const char* std_values[2][2]={{"OFF","ON"},//22-03
							  {0}
							 };

uint8_t G_standby=0;
uint8_t Stand_by_con;
uint16_t G_stand_time;
uint16_t G_stand_min;
uint16_t G_stand_sec;
uint16_t standby_min=0;
uint8_t cnfrm_flag=0;

uint8_t G_std_pwr_flag=0;

extern uint8_t standby_key;
extern uint8_t auto_lock_time;
extern uint8_t ON_Flag;
extern uint8_t home_flag;
uint16_t std_hr_time;
uint16_t std_min;
uint8_t last_value;
extern uint16_t std_sec_count;
extern uint8_t ON_Flag;


void standby_fun(void)
{

	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t std_variable=1;
	uint8_t end_op_no=3;
	uint8_t edit_flag=0;
	uint8_t standby_sts=0;

	auto_lock_time=AUTO_EXIT_TIME;
	standby_sts=G_standby;
	POW.power_status_flag=1;
	while(KEY.value);

	while(std_variable)
		{
			Power_off();

			 if(!ON_Flag)
			       	{
				G_standby=standby_sts;

			       	break;//23-03
			       	}
			       else if(POW.power_status_flag)
				   {
					   POW.power_status_flag=0;//26-03
					   standby_Frame();
					   standby_Opt_Frame(page_no,option_no,end_op_no);//22-03

					  	if(edit_flag)
						{
							TFT_ST7789V_left_Right_arrow_14_arial(55,195,0,LIGHT_GRAY,BLACK);
							TFT_ST7789V_left_Right_arrow_14_arial(55,269,1,LIGHT_GRAY,BLACK);

							edit_flag = 1;
						}
						else
						{
							edit_flag = 0;
							if(option_no==0)
							{
							TFT_ST7789V_left_Right_arrow_14_arial(55,195,0,LIGHT_GRAY,LIGHT_GRAY);
							TFT_ST7789V_left_Right_arrow_14_arial(55,269,1,LIGHT_GRAY,LIGHT_GRAY);
							}



						}

				   }
				   	if(KEY.value)
					{
				      auto_lock_time=AUTO_EXIT_TIME;
					}
					else if(!auto_lock_time)
					{
						G_standby=standby_sts;

						break;

					}


			switch(KEY.value)
			{


				case RIGHT_KEY:
//					beep_sound();
					if(edit_flag)
					{
					standby_subsetting( option_no,page_no,1);
					}

				while(KEY.value);
				break;

				case LEFT_KEY:
//					beep_sound();
					if(edit_flag)
					{
					standby_subsetting( option_no,page_no,0);
					}

				while(KEY.value);
				break;

				case OK_KEY: 					 //OK_KEY
//				beep_sound();
				if(!edit_flag && option_no==0)
				{
					TFT_ST7789V_left_Right_arrow_14_arial(55,195,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(55,269,1,LIGHT_GRAY,BLACK);

					edit_flag = 1;
				}
				else
				{
					cnfrm_flag=1;
					edit_flag = 0;

					if(option_no==0)
					{
						TFT_ST7789V_left_Right_arrow_14_arial(55,195,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(55,269,1,LIGHT_GRAY,LIGHT_GRAY);

					}
				//	POW.power_status_flag=1;//27-03

				}
				//POW.power_status_flag=1;//27-03

				EEPROM_datawrite(EEPROM_STANDBY,G_standby,1);

				while(KEY.value);
				break;


				case BACK_KEY:
//				beep_sound();
					//Stand_by_con=0;
				if(edit_flag)
				{
					G_standby=standby_sts;
				}
				else
				{
					EEPROM_datawrite(EEPROM_STANDBY,G_standby,1);
				}
				std_variable=0;
				//Rectangle(3,36,300,169,MAIN_FRAME_COL);
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

void standby_Frame(void)
{

	Frame_layout3(1,1,(uint8_t*)"Standby mode",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}

void standby_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;


	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),300,31,LIGHT_GRAY);
		}


		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,(uint8_t*)std_menu[loop_count],back_colour,text_colour);

//		TFT_String(180,INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);

		TFT_String(220-3,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)std_values[loop_count][G_standby],back_colour,text_colour);

	}
}

void standby_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{
	uint8_t* std_data[2] = {&G_standby};
	uint8_t max_value[2] = {1,0};  //max array elements
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	cur_value	= *std_data[option_no];
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
		TFT_String(220-3,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)std_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);

		TFT_String(220-3,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)std_values[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
	}

	*std_data[option_no] = cur_value;
}


uint8_t  standby_mode_function(void)
{
	uint8_t check_flag=0;
	uint8_t pd_loop=1;
	uint8_t exit_flag=0;
	uint8_t cursor=0;




	POW.power_status_flag=1;
//	standby_frame();
//	standby_cursor(cursor,0);
//while(KEY.value);

	while(pd_loop)
	{
		 if(!ON_Flag)
	     {
		   break; //26-03
	     }

		  if(POW.power_status_flag)
		 {
			 POW.power_status_flag=0;
			 standby_frame();
			 standby_cursor(cursor,0);
//			 standby_logo();
		 }

		 switch(KEY.value)
				{

				case LEFT_KEY:

					{
//						beep_sound();
						cursor=0;
						standby_cursor(cursor,0);
						standby_logo();
					}


					while(KEY.value);
					 break;


				case RIGHT_KEY:

					{
//						beep_sound();
						cursor=1;

						standby_cursor(cursor,0);
						standby_logo();
					}
					//check_flag=1;
					//pd_loop=0;
					while(KEY.value);
					 break;

				case OK_KEY:
					if(!cursor)
					{
						//exit_cnt++;
						//if(exit_cnt>5)
						{
							//exit_cnt=0;
							//delay_secs(2);
							shutting_down();
							Rectangle(0,0,320,240,BLACK);
					    	home_flag=0;
					    	ON_Flag=0;
							pd_loop=0;


						}
					}
					else
					{
						exit_flag=edit_screen_Time(&G_stand_time, TIME_HHMM,2400);

						std_hr_time=G_stand_time/100;
						std_min=G_stand_time %100;
						G_stand_min=((std_hr_time*60)+std_min);



						if((!exit_flag)&&(G_stand_time))
						{

							check_flag=standby_time_screen();
							if(check_flag)
							{
								pd_loop=0;
							}
						}
						else
						{
//							standby_frame();
							POW.power_status_flag=1;
						}

					}
					last_value=G_standby;
					//pd_loop=0;
					//check_flag=1;

					while(KEY.value);
					break;
				case BACK_KEY:
					cursor=0;
					//standby_cursor(cursor,0);
					last_value=0;
					check_flag=1;
					pd_loop=0;
					while(KEY.value);
					break;

				}
	}
	return check_flag;
}


void standby_frame()
{

	CLRSCR(MAIN_FRAME_COL);


	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Standby mode");

	Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");

	CENTER_SCR(MAIN_FRAME_COL); 			//center green

	border_lines();

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

//	TFT_ST7789V__rectangle_curve(52,92-10,64,64,0,WHITE);

	standby_logo();

//	TFT_String(38,161,(uint8_t* )"Turn off",MAIN_FRAME_COL,LIGHT_GRAY);


}

void standby_cursor(uint8_t cur_value,uint8_t edit_flag)
{
//	uint8_t row=0;
//	uint8_t	col=0;


//	if(last_value !=cur_value)
	{
		if(cur_value==0)
		{
			TFT_ST7789V__rectangle_curve(202,92-10,64,64,0,MAIN_FRAME_COL); //erasing cur=1
			TFT_String(190,161,(uint8_t* )"Standby",MAIN_FRAME_COL,MAIN_FRAME_COL);

			TFT_ST7789V__rectangle_curve(52,92-10,64,64,0,WHITE);

			standby_logo();

			TFT_String(38,161,(uint8_t* )"Turn off",MAIN_FRAME_COL,LIGHT_GRAY);


		}
		else if(cur_value==1)
		{
			TFT_ST7789V__rectangle_curve(52,92-10,64,64,0,MAIN_FRAME_COL); //erasing cur=0
			TFT_String(38,161,(uint8_t* )"Turn off",MAIN_FRAME_COL,MAIN_FRAME_COL);

			TFT_ST7789V__rectangle_curve(202,92-10,64,64,0,WHITE);
			standby_logo();

			TFT_String(190,161,(uint8_t* )"Standby",MAIN_FRAME_COL,LIGHT_GRAY);

		}


//		last_value =cur_value;
		}
}

void standby_logo()
{
	TFT_ST7789V__rectangle_curve(55-1,94-10,60,60,0,Lionheart);
	TFT_ST7789V_power_28pt_arial(100+2-10,50+14,0,Lionheart,WHITE);


	TFT_ST7789V__rectangle_curve(204,94-10,60,60,0,ORANGE);
	TFT_ST7789V_standby_28pt_arial(100+2-10,210,0,ORANGE,WHITE);

}
uint8_t standby_time_screen(void)
{

	uint8_t exit_flag=1;
	uint8_t check_flag=0;
	uint8_t disp_flag=0;
	static uint16_t prev_std_min=1;  //,prev_std_hr=1;

	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Standby Mode");
	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	CENTER_SCR(MAIN_FRAME_COL);  //center green

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,210,(uint8_t* )"to retrieve",ORANGE_DARK,WHITE);

//	Standby_Time_disp(70, 95, G_stand_min,G_stand_sec, MAIN_FRAME_COL, WHITE);

	G_stand_sec=0;



	while(KEY.value);

	while(exit_flag)
	{
		if(!ON_Flag)
		{
			break;
		}

		if(G_stand_sec!=prev_std_min)
			{
//				Standby_Time_disp(70, 95, G_stand_min,G_stand_sec, MAIN_FRAME_COL, MAIN_FRAME_COL);

			if(G_stand_min||G_stand_sec)
			{
				  Rectangle(160,80,20+40+30+20,80,MAIN_FRAME_COL);

				Standby_Time_disp(70, 95, G_stand_min,G_stand_sec, MAIN_FRAME_COL, WHITE);
			}
				prev_std_min=G_stand_sec;

			}

		if(G_stand_min)
		{
			if(!G_stand_sec)
			{
				G_stand_min--;
				G_stand_sec=59;
			}

		}
		else if(!G_stand_sec && !disp_flag)
		{

			standby_popup();
			disp_flag=1;
		//	exit_flag=0;
		EEPROM_datawrite(EEPROM_STANDBY,G_standby,1);

		}


//		Font_Select(ARIAL_14);
//		TFT_Number(30,70,G_stand_min,YELLOW,GREEN);



//		if(!G_stand_sec && !G_stand_min)
//		{
//
//			ON_Flag=0;
//
//			break;
//		}

			if(KEY.value==OK_KEY)
			{

					//Rectangle(0,0,320,240,MAIN_FRAME_COL);
					G_stand_time=0;
					check_flag=1;
					last_value=0;
					exit_flag=0;
					while(KEY.value);
					break;
			}

	}
	return check_flag;
}

void standby_popup()
{
	Rectangle(11,47,298,145,MAIN_FRAME_COL);//CENTRE GREEN
	Font_Select(ARIAL_BOLD_14);
	TFT_ST7789V__rectangle_curve(20,80-20,90+20,280,0,WHITE);//70//80//120

	Font_Select(ARIAL_BOLD_14);
	//TFT_String(135,90,(uint8_t*)"Info",WHITE,RED);//LIGHT_GRAY_4
	//TFT_String(36,125,(uint8_t*)"Standby time completed",WHITE,BLACK);
	//TFT_String(36,125,(uint8_t*)"Standby time completed",WHITE,BLACK);
    TFT_String_align(20,300,80,(uint8_t *)"Info",0,WHITE,RED);
    TFT_String_align(20,300,110,(uint8_t*)"Standby",0,WHITE,BLACK);
    TFT_String_align(20,300,140,(uint8_t*)"time completed",0,WHITE,BLACK);


}


void Standby_Time_disp(uint16_t xpos, uint16_t ypos, uint32_t standby_min,uint32_t standby_sec, uint32_t border_color, uint32_t fill_color)
{

char local_buffer[10];
    uint32_t hours = standby_min / 60;
    uint32_t minutes = standby_min % 60;
   // uint32_t seconds = number % 100;

    border_lines();

    Font_Select(BEBAS_60);

    sprintf(local_buffer, "%02d", hours);
    xpos = TFT_String(xpos,ypos,(uint8_t*)local_buffer, border_color, fill_color);

    xpos = TFT_String(xpos, ypos, (uint8_t *)":", border_color, fill_color);

    sprintf(local_buffer, "%02d", minutes);
    xpos = TFT_String(xpos, ypos, (uint8_t*)local_buffer, border_color, fill_color);

    xpos = TFT_String(xpos, ypos, (uint8_t *)":", border_color, fill_color);

    sprintf(local_buffer, "%02d", standby_sec);
    TFT_String(xpos, ypos,(uint8_t*) local_buffer, border_color, fill_color);


}
