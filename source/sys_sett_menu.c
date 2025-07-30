
#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "Main_Menu.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Brand.h"
#include "Drug_List.h"
#include "Occlusion.h"
#include "Main_Menu.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "SD_calib.h"
#include "Syringe_calib_header.h"
#include "Main_Screen.h"

extern uint8_t auto_lock_time;//common_bug
extern uint32_t back_colour;
extern uint32_t text_colour;
uint8_t G_press_calib=0;
uint8_t G_SD_calib=0;
uint8_t G_SL_calib=0;
//uint8_t G_acc_calib=0;
uint8_t G_calib=0;
uint8_t G_RTC=0;
uint8_t G_Vref=0;
uint8_t brand_power_flag=0;
extern uint8_t brand_select_no;
extern uint8_t menu_brand_flag;

const char* sys_sett_menu[4]={"Calibration","Plunger Tuning","Date & time","Accelerometer"};
const char* calib_setting_menu[3]={"SD Calibration","SL Calibration","Pressure Calibration"};


extern uint8_t ON_Flag;

extern void RTC_screen(void);
extern void Pressure_calib_setting(void);
extern void SD_Calibration();
extern void accu_calib_setting();
extern void SD_Calibration_entry(void);
extern void vref_info_setting(void);
//extern uint8_t ON_Flag;


void calib_setting();
extern void plunger_adc_live_main_screen();
extern void Pressure_EEPROM_read();

/**************************************************************************
 *Function Name :void sys_sett_Screen()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to enter the KVO option
***************************************************************************/


void sys_sett_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t sys_variable=1;

	//uint8_t time_interval=120;//common_bug  //ZOOZ_1


	sys_variable=Passcode(1);//23-03


//	POW.power_status_flag=1;//27-03



	if(sys_variable)//23-03
	{
		layout();
		sys_sett_frame();
		sys_sett_Opt_Frame(0,option_no,3);//27-03//Need to change the password power off functionality
	}

    while(KEY.value)
    {

    }

	while(sys_variable)
	{
		Power_off();//27-03


			if(!ON_Flag)
				{
					break;//26-03
				}

				else if(POW.power_status_flag)//27-03

			   {
					POW.power_status_flag=0;//27-03
					sys_sett_frame();
					sys_sett_Opt_Frame(0,option_no,3);
					//scroll_bar(page_no,option_no,4);
				}



		switch(KEY.value)
		{
			case DOWN_KEY :
			if(option_no<3)
			{
				option_no++;
			}

			else
			{
				option_no=0;
			}

			if(option_no!=prev_option)
			{

				sys_sett_cursor(prev_option,option_no,page_no);//,sys_sett_menu);
				prev_option = option_no;

			}
		//	scroll_bar(page_no,option_no,4);

			while(KEY.value);
			break;

			case UP_KEY:
			if(option_no>0)
				{
					option_no--;
				}
			else
				{
				option_no=3;
				}

				if(option_no!=prev_option)
				{

					sys_sett_cursor(prev_option,option_no,page_no);//,sys_sett_menu);
					prev_option = option_no;
				}
			//	scroll_bar(page_no,option_no,4);

			while(KEY.value);
			break;


			case OK_KEY:
				if(option_no<=2)
				{
			sys_sett_fun(option_no);
				}
			POW.power_status_flag=1;//27-03
			while(KEY.value);
			break;

			case BACK_KEY:
			sys_variable=0;
			while(KEY.value);
			break;
		}
	}

}

/**************************************************************************
 *Function Name :void sys_sett_frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for KVO main frame
***************************************************************************/

void sys_sett_frame(void)
{
	Frame_layout3(1,3,(uint8_t*)"System settings",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);


}

/**************************************************************************
 *Function Name :void sys_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void sys_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	 uint8_t loop_count=0;

		for(loop_count=0;loop_count<=end_opt;loop_count++)
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

			Font_Select(ARIAL_BOLD_14);
			//TFT_String(21,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),(uint8_t*)main_menu[page][loop_count],back_colour,text_colour);
			TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)sys_sett_menu[loop_count],back_colour,text_colour);
			//value = sys_sett_data[(page*4)+loop_count];


		}
}

/**************************************************************************
 *Function Name :void sys_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void sys_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)//, const char* array[4])
{
	//const uint8_t sys_sett_print[3]={G_press_calib,G_acc_calib,G_RTC};
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
				opt_no		= prev_option;
			}
			else
			{
				back_colour = LIGHT_GRAY;
				text_colour = MAIN_FRAME_COL;
				opt_no= curr_option;
			}
			Rectangle(9,48+(38*(opt_no)),300,31,back_colour);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)sys_sett_menu[opt_no],back_colour,text_colour);

		}
}
/**************************************************************************
 *Function Name :void sys_sett_fun(uint8_t page_no,uint8_t option_no)
 *Arguments     :uint8_t option_no
 *Return        :void
 *Description	:In this function for sub system setting
 ***************************************************************************/
void sys_sett_fun(uint8_t option_no)
{


	void (*fun_ar[3])()={calib_setting,plunger_adc_live_main_screen,RTC_screen};


	fun_ar[option_no]();

}

void calib_sett_frame(void)
{
	Frame_layout3(1,3,(uint8_t*)"Calibration",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);

//	uint8_t calib_count = 0;
//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
//
//	//Rectangle(0,0,320,34,NEW_HDR_COL);
//
//	Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Calibration");	// header
//
//
//	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
//	//CENTRE GREEN
//	CENTER_SCR(MAIN_FRAME_COL);  //center green
//	border_lines();
//	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
//	Font_Select(ARIAL_BOLD_14);
//	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
//	separation_lines(1,3);


}
void calib_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
    uint8_t loop_count=0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
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



		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)calib_setting_menu[loop_count],back_colour,text_colour);
		separation_lines(1,3);
	}

}
void calib_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
{
	//const uint8_t sys_sett_print[3]={G_press_calib,G_acc_calib,G_RTC};
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
			opt_no		= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}
		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);


				//TFT_ST7789V__rectangle_curve(3,35+(43*(opt_no)),41,308,0,back_colour);


				Font_Select(ARIAL_BOLD_14);

				TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
				(uint8_t*)array[opt_no],back_colour,text_colour);
				separation_lines(1,3);
	}
}
void calib_sett_fun(uint8_t option_no)
{


	void (*fun_ar[3])()={SD_Calibration,accu_calib_setting,Pressure_calib_setting};


	fun_ar[option_no]();

}



void calib_setting()
{
		uint8_t page_no=0;
		uint8_t option_no=0;
		uint8_t prev_option = 0;
		uint8_t calib_sett_variable=1;
		uint8_t exit_flag = 1;

		uint8_t temp_brand_no=0;

//		calib_sett_frame();
//		calib_sett_Opt_Frame(0,0,2);

		temp_brand_no=brand_select_no;
		brand_power_flag=1;
		menu_brand_flag=0;

		while(exit_flag)
		{
		calib_sett_variable=Brand_List(1);

		if(!calib_sett_variable)
		{
			exit_flag = 0;
			break;
		}
		else
		{
			POW.power_status_flag=1;
		}

		while(calib_sett_variable)
		{
			 Power_off();

					 if(!ON_Flag)
					   {
						  break; //26-03
					   }

					 else if(POW.power_status_flag)
					     {
						   POW.power_status_flag=0;
						   calib_sett_frame();
						   calib_sett_Opt_Frame(0,option_no,2);
					//	   scroll_bar(page_no,option_no,3);//26-03
						   while(KEY.value);
					     }


			switch(KEY.value)
			{
				case DOWN_KEY :
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

					calib_sett_cursor(prev_option,option_no,page_no,calib_setting_menu);
					prev_option = option_no;

				}

			//	scroll_bar(page_no,option_no,3);
				while(KEY.value);
				break;

				case UP_KEY:
				if(option_no>0)
					{
						option_no--;
					}
				else
					{
					option_no=2;
					}

					if(option_no!=prev_option)
					{

						calib_sett_cursor(prev_option,option_no,page_no,calib_setting_menu);
						prev_option = option_no;
					}
				//	scroll_bar(page_no,option_no,3);
				while(KEY.value);
				break;


				case OK_KEY:
				calib_sett_fun(option_no);
//				calib_sett_frame();
//				calib_sett_Opt_Frame(0,option_no,2);
				 POW.power_status_flag=1;
				while(KEY.value);
				break;

				case BACK_KEY:

					if(brand_select_no!=temp_brand_no)
					{
						brand_select_no=temp_brand_no;
						EEPROM_SD_READ();
						SL_EEPROM_read();
						Pressure_EEPROM_read();
					}
				calib_sett_variable=0;
		//	    calib_sett_variable=Brand_List(1);
				POW.power_status_flag=1;

			//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
				while(KEY.value);
				break;
			}
		}
	}
}
