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
#include"Driver_I2C.h"    //23-03
#include "FR_CALC.h"
#include "Main_screen.h"
#include "sound.h"
//#include "../Menu_Setting/Main_Menu.h"
//#include "../Menu_Setting/Main_Menu.h"
#include "Header.h"
#include "consumed_hrs.h"
#include "Power_off.h"
#include "factory.h"
#include"TFT_String.h"
uint16_t d_inf=1;//17.9.24

uint8_t G_infusion_mode_opt=0;
uint8_t G_bright_level=0;
uint8_t G_sound_level=0;
uint8_t G_device_info=0;
 uint8_t G_infusion_modes;
uint8_t G_drug_list=0;
uint8_t G_lock_time=0;
uint8_t G_last_data=0;

extern void device_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
extern uint8_t edit_flag ;
extern uint32_t back_colour ;
extern uint32_t text_colour;
extern volatile uint8_t waitingtime_1;
extern uint8_t ON_Flag;
extern uint8_t auto_lock_time;  //ZOOZ_1
extern uint8_t G_Menu_Brand_flag;//17.9.24
uint8_t G_pro_name;
uint8_t G_pro_model;
uint8_t G_sw_version;
uint8_t G_used_hrs;
uint8_t bright_ctrl[5]={1,3,5,7,0};
uint8_t brightness_mode=0;
uint16_t Auto_bright_level=0;

//const char* device_sett_menu[3]={"Brightness  Lvl","Sound  Lvl","Device Info"};

const char* device_sett_menu[2][4]={
		                             {"About(Help)","Brightness","Sound Level","Auto Lock"},
		                             {"Last data","Default modes","Drug List","Screen Saver"}
		                           };

const char* infusion_modes_options[2][5]={
											{"Flowrate","VTBI","Dose rate","Trapezia"},
											{"Ask every time","","",""}
										};

const char* bright_modes_options[2]={"Auto","Manual"};

const char* glow_values[2][6]={ {"OFF","ON","","","",""},
								{"1","2","3","4","5",""},
								};
const char* default_modes_options[2]={"Infusion modes","Syringe selection"};//17.9.24

const char* device_info_menu[5]={"Product","Model","S.no","SW Ver","Used hrs"};//25-03

const char* device_info_values[2][1]={{"Syringe Pump"},{"SP-1100"}};
									  //{"V-0.01"},{"00.00.00"}};//25-03
const char* default_values[1][2]={{"ON","OFF"}

};
const char* device_values[2][4][6]={
									  {
										  {                    },
										  {"1","2", "3","4","5"},//,"6","7","8","9","10"},
										  {"1","2","3","4","5"},
										  {"OFF","1 min","5 min","10 min",""}
									  },

									  {
										  {"OFF","ON","","",""},
										  {"F/R","Target","Dose","Ask",""},
										  {"No","Yes","",""},
										  {"ON","OFF","","",""},
									  }
 	 	 	 	 	 	 	 	 };

extern void device_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]);
extern uint16_t Passcode(uint16_t mode);//23-03
extern void scroll_bar(uint8_t page_no,uint8_t option_no,uint8_t total_option);     //23-03                                  //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
extern void infusion_sett_Screen();
extern void infusion_mode_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][5]);

extern void factory_setting();

extern void layout();

void about_fun(void);
void default_modes_Opt_Frame();
void default_sett_Screen();
void default_mode_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2]);
void default_mode_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void default_subsetting(uint8_t option_no, uint8_t INC_flag);//17.9.24
void Device_sett_EEPROM_write();
extern void device_sett_frame();


uint8_t G_drug_list;//23-03
uint8_t G_screen_saver;

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void device_sett_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t device_variable=1;
	uint8_t edit_flag=0;
	uint8_t end_opt_no=0;
//	uint8_t fact_longpress=0;
//	uint8_t check_flag=0;
	uint8_t total_option=8;
	uint8_t down_opt_arr[3]={3,3,3};
	uint8_t up_opt_arr[3][2]={	{3,3},
								{3,3},
								{3,3}
							 };
	POW.power_status_flag=1;
	auto_lock_time=AUTO_EXIT_TIME;

	while(device_variable)
	{
		Power_off();
//		Font_Select(ARIAL_14);
//	    TFT_Number(0,70,G_drug_list,YELLOW,RED);

		if(!ON_Flag)
		{
			break; //26-03
		}

		else if(POW.power_status_flag)
		{
			POW.power_status_flag=0;
			device_sett_frame();
			device_sett_Opt_Frame(page_no,option_no,3);//23-03
			scroll_bar(page_no,option_no,total_option); //23-03

			  if(!edit_flag)
				{
				   if((option_no==2)&&((page_no==0)||(page_no==1)))
				  {
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,195,0,LIGHT_GRAY,LIGHT_GRAY);
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,285,1,LIGHT_GRAY,LIGHT_GRAY);
				  }

				  else if((option_no==0)&&(page_no==1))
				  {
						    TFT_ST7789V_left_Right_arrow_14_arial(58,195,0,LIGHT_GRAY,LIGHT_GRAY);
							TFT_ST7789V_left_Right_arrow_14_arial(58,285,1,LIGHT_GRAY,LIGHT_GRAY);
				  }

				  else if((option_no==3)&&((page_no==0)||(page_no==1)))
				  {
						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,195,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,285,1,LIGHT_GRAY,LIGHT_GRAY);
				  }
					//edit_flag = 1;
				}

				else
				{
				  if((option_no==2)&&((page_no==0)||(page_no==1)))
				  {
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,195,0,LIGHT_GRAY,BLACK);
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,285,1,LIGHT_GRAY,BLACK);
				  }
				  else if((option_no==0)&&(page_no==1))
					  {
						TFT_ST7789V_left_Right_arrow_14_arial(58,195,0,LIGHT_GRAY,BLACK);
						TFT_ST7789V_left_Right_arrow_14_arial(58,285,1,LIGHT_GRAY,BLACK);
					  }
				  else if((option_no==3)&&((page_no==0)||(page_no==1)))
				  {

						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,195,0,LIGHT_GRAY,BLACK);
						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,285,1,LIGHT_GRAY,BLACK);
				  }
				}



		//	while(KEY.value);
		}

		if(KEY.value)
		{
			auto_lock_time=AUTO_EXIT_TIME;
		}

		if(!auto_lock_time)
		{
			device_variable=0;
			break;
		}

		switch(KEY.value)
		{
			case DOWN_KEY :   //DOWN_KEY

			if(!edit_flag)
			{
				if( (page_no==0 && option_no<3)  || (page_no==1 && option_no<3) )
				{
					option_no++;
				}
				else
				{
					page_no++;
//					if(page_no==1)
//					{
//						option_no=0;
//						prev_option=0;
//						end_opt_no=3;
//					}
//					else
//					{
//						option_no=0;
//						page_no=0;
//						prev_option=0;
//						end_opt_no=3;
//					}
					if(page_no>=2)
					{
						page_no=0;
					}
					option_no=0;
					prev_option=option_no;
					end_opt_no=down_opt_arr[page_no];  //REW1

					layout();
					device_sett_Opt_Frame(page_no,0,end_opt_no);
				}

				if(option_no!=prev_option)
				{
					device_sett_cursor(prev_option,option_no,page_no,device_sett_menu);
					prev_option = option_no;
				}
			}
				scroll_bar(page_no,option_no,total_option);//25-03

			while(KEY.value);
			break;

			case UP_KEY:           //UP_KEY


			if(!edit_flag)
			{
				if((page_no==0 && option_no>0)  || (page_no==1 && option_no>0))
				{
					option_no--;
				}

				else
				{
					//page_no++;

					if(!page_no)
					{
						page_no++;
//						option_no=2;
//						page_no=1;
//						end_opt_no=2;
//						prev_option=2;
					}

					else
					{
						page_no--;
//						prev_option=3;
//						option_no=3;
//						page_no=0;
//						end_opt_no=3;
					}

					option_no=up_opt_arr[page_no][0];
					end_opt_no=up_opt_arr[page_no][1];

					layout();
					device_sett_Opt_Frame(page_no,option_no,end_opt_no);
				}

				if(option_no!=prev_option)
				{
					device_sett_cursor(prev_option,option_no,page_no,device_sett_menu);
					prev_option = option_no;
				}
			}
				scroll_bar(page_no,option_no,total_option);//25-03

			while(KEY.value);
			break;

			case RIGHT_KEY:				 //LEFT_KEY:

//			//beep_sound();//23-03
			if(edit_flag)
			{
				device_subsetting( option_no,page_no,1);
			}
//			 if(!edit_flag)
//				{
//				  if((option_no==0)&&((page_no==1)))
//				  {
//						TFT_ST7789V_left_Right_arrow_14_arial((54+8),188,0,LIGHT_GRAY,BLACK);
//						TFT_ST7789V_left_Right_arrow_14_arial((54+8),269,1,LIGHT_GRAY,BLACK);
//				  }
//				  else if((option_no==2)&&((page_no==0)||(page_no==1)))
//				  {
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no),188,0,LIGHT_GRAY,BLACK);
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no),269,1,LIGHT_GRAY,BLACK);
//				  }
//				  else if((option_no==3)&&((page_no==0)||(page_no==1)))
//				  {
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no),158,0,LIGHT_GRAY,BLACK);
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no),269,1,LIGHT_GRAY,BLACK);
//				  }
//				}
			while(KEY.value);
			break;

			case LEFT_KEY:				 //RIGHT_KEY:
//			//beep_sound();
			if(edit_flag)
			{
				device_subsetting( option_no,page_no,0);
			}
//			 if(!edit_flag)
//				{
//				  if((option_no==0)&&((page_no==0)||(page_no==1)))
//				  {
//						TFT_ST7789V_left_Right_arrow_14_arial((62-5),188,0,LIGHT_GRAY,BLACK);
//						TFT_ST7789V_left_Right_arrow_14_arial((62-5),269,1,LIGHT_GRAY,BLACK);
//				  }
//				  else if((option_no==2)&&((page_no==0)||(page_no==1)))
//				  {
//					  TFT_ST7789V_left_Right_arrow_14_arial((63*option_no)+2,188,0,LIGHT_GRAY,BLACK);
//					  TFT_ST7789V_left_Right_arrow_14_arial((63*option_no)+2,269,1,LIGHT_GRAY,BLACK);
//				  }
//				  else if((option_no==3)&&((page_no==0)||(page_no==1)))
//				  {
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no)+4,188,0,LIGHT_GRAY,BLACK);
//					  TFT_ST7789V_left_Right_arrow_14_arial((53*option_no)+4,269,1,LIGHT_GRAY,BLACK);
//				  }
//				}
			while(KEY.value);
			break;

			case OK_KEY: 					 //OK_KEY

		  //beep_sound();//23-03
		  if(page_no==1 && option_no==1)
		  {
		 //  infusion_sett_Screen();
			  default_sett_Screen();
		   page_no=1;
		   end_opt_no=3;
		   POW.power_status_flag=1;
		   edit_flag=1;
		  }

		  if(page_no==0 && option_no==1)
			{
			   brightness_sett_Screen();
			   page_no=0;
			   end_opt_no=2;
			   POW.power_status_flag=1;
			   edit_flag=1;
			}

		  if(!edit_flag)
			{
//			  if((option_no==0)&&((page_no==0)||(page_no==1)))
//			  {
//					TFT_ST7789V_left_Right_arrow_14_arial((62-5),190,0,LIGHT_GRAY,BLACK);
//					TFT_ST7789V_left_Right_arrow_14_arial((62-5),271,1,LIGHT_GRAY,BLACK);
//			  }
			   if((option_no==2)&&((page_no==0)||(page_no==1)))
			  {
				  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,195,0,LIGHT_GRAY,BLACK);
				  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,285,1,LIGHT_GRAY,BLACK);
			  }

				  else if((option_no==0)&&(page_no==1))
				  {
	//				  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,190,0,LIGHT_GRAY,BLACK);
	//				  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,271,1,LIGHT_GRAY,BLACK);
								TFT_ST7789V_left_Right_arrow_14_arial(58,195,0,LIGHT_GRAY,BLACK);
							TFT_ST7789V_left_Right_arrow_14_arial(58,285,1,LIGHT_GRAY,BLACK);

				  }

			  else if((option_no==3)&&((page_no==0)||(page_no==1)))
			  {
//				  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,190,0,LIGHT_GRAY,BLACK);
//				  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,271,1,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,195,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,285,1,LIGHT_GRAY,BLACK);
			  }


				edit_flag = 1;
			}

			else
			{
				edit_flag = 0;
//				if(!edit_flag)
				{
//				  if((option_no==0))
//				  {
//						TFT_ST7789V_left_Right_arrow_14_arial((62-5),190,0,LIGHT_GRAY,LIGHT_GRAY);
//						TFT_ST7789V_left_Right_arrow_14_arial((62-5),271,1,LIGHT_GRAY,LIGHT_GRAY);
//				  }
				  if((option_no==2))
				  {
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,195,0,LIGHT_GRAY,LIGHT_GRAY);
					  TFT_ST7789V_left_Right_arrow_14_arial((64*option_no)+2,285,1,LIGHT_GRAY,LIGHT_GRAY);
				  }
				  else if((option_no==0)&&(page_no==1))
					  {
						TFT_ST7789V_left_Right_arrow_14_arial(58,195,0,LIGHT_GRAY,LIGHT_GRAY);
					    TFT_ST7789V_left_Right_arrow_14_arial(58,285,1,LIGHT_GRAY,LIGHT_GRAY);
					  }
				  else if((option_no==3))
				  {
//					  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,190,0,LIGHT_GRAY,LIGHT_GRAY);
//					  TFT_ST7789V_left_Right_arrow_14_arial((54*option_no)+4,271,1,LIGHT_GRAY,LIGHT_GRAY);

						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,195,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(56*option_no,285,1,LIGHT_GRAY,LIGHT_GRAY);
				  }
			}
			}
			if(option_no==0 && (page_no==0))
			{
				about_fun();//23-03
				edit_flag = 0;
				//POW.power_status_flag=1;
				if(auto_lock_time)
				{
				  POW.power_status_flag=1;
				}
			}



			Device_sett_EEPROM_write();


			while(KEY.value);
			break;

			case BACK_KEY: 					 //BACK_KEY
			device_variable=0;
		//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
			while(KEY.value);
			break;
		}
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void device_sett_frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Device settings",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

/**************************************************************************
 *Function Name :void sys_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void device_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	 uint8_t device_sett_data[2][4] = {
					 	 	 	 	 	 	 {0,0,G_sound_level,G_lock_time},
											 {G_last_data,G_infusion_mode_opt,G_drug_list,G_screen_saver}
									  };

	uint8_t loop_count=0;
	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)//0==0
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

		value = device_sett_data[page][loop_count];

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
		,(uint8_t*)device_sett_menu[page][loop_count],back_colour,text_colour);

		if ((page == 0 && (loop_count == 2 || loop_count == 3)) ||
		    ((page == 1 || page == 2) && (loop_count >= 0 )))




				{

//			TFT_String(200+15,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
//			(uint8_t*)device_values[page][loop_count][value],back_colour,text_colour);


			TFT_String_align(195,285,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)device_values[page][loop_count][value],0,back_colour,text_colour);

			if((page == 1 || page == 2) && (loop_count != 1 ))
			{
				TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
									,(uint8_t*)":",back_colour,text_colour);
			}


		}
	}

}

/**************************************************************************
 *Function Name :void sys_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void device_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4])
{
	const uint8_t device[2][4] = {
				 	 	 	 	 	 	 {0,0,G_sound_level,G_lock_time},
										 {G_last_data,G_infusion_mode_opt,G_drug_list,G_screen_saver}
								 };
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

		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
		(uint8_t*)array[page][opt_no],back_colour,text_colour);

		if ((page == 0 && (opt_no == 2 || opt_no == 3)) ||
		    ((page == 1 || page == 2) && (opt_no >= 0 )))
		    {

			TFT_String_align(195,285,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+7,
				(uint8_t*)device_values[page][opt_no][device[page][opt_no]],0,back_colour,text_colour);

		if((page == 1 || page == 2) && (opt_no != 1 ))
		{
			TFT_String(180,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6
						,(uint8_t*)":",back_colour,text_colour);
		}

			}
	}
}

/**************************************************************************
 *Function Name :void kvo_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
 *Arguments     :uint8_t option_no,uint8_t page_no, uint8_t INC_flag
 *Return        :void
 *Description	:In this function array arguments print as right side
***************************************************************************/

void device_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{
	uint8_t* device_data[2][4] = {
			 	 	 	 	 	 	 {0,0,&G_sound_level,&G_lock_time},
									 {&G_last_data,&G_infusion_mode_opt,&G_drug_list,&G_screen_saver}
								 };

	uint8_t max_value[2][4] = {
			                   {0,4,4,3},
							   {1,0,1,1}//max array elements
							  };
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	cur_value	= *device_data[page_no][option_no];//0
	pre_value	= cur_value;

	if(INC_flag)
	{

		if(cur_value < max_value[page_no][option_no])
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
			cur_value=max_value[page_no][option_no];

		}

	}


	if(pre_value != cur_value)
	{
		Font_Select(ARIAL_BOLD_14);
//		TFT_String(200+15,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
//		(uint8_t*)device_values[page_no][option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);
//
//		Font_Select(ARIAL_BOLD_14);
//		TFT_String(200+15,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
//		(uint8_t*)device_values[page_no][option_no][cur_value],LIGHT_GRAY,MAIN_FRAME_COL);



		TFT_String_align(195,285,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
			(uint8_t*)device_values[page_no][option_no][pre_value],0,LIGHT_GRAY,LIGHT_GRAY);

		TFT_String_align(195,285,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
		(uint8_t*)device_values[page_no][option_no][cur_value],0, LIGHT_GRAY,MAIN_FRAME_COL);

	}
	*device_data[page_no][option_no] = cur_value;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void device_info_frame(void)
{
	uint8_t info_count = 0;
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	//Rectangle(0,0,320,34,NEW_HDR_COL);

	Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Device info");	// header


	CENTER_SCR(MAIN_FRAME_COL);  //center green
   // border_lines();

    separation_lines(1,5);
}

/**************************************************************************
 *Function Name :void device_info_Opt_Frame()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to used device info option  frame
***************************************************************************/

void device_info_Opt_Frame()
{
	uint16_t device_info_data[5]={G_pro_name,G_pro_model,FACT.factory[3],VERSION,UTILIZED.device_hrs_cnt};


	uint16_t value = 0;
    uint8_t loop_count=0;

	for(loop_count=0;loop_count<=4;loop_count++)
	{
	back_colour	= MAIN_FRAME_COL;
	text_colour = LIGHT_GRAY;

	value = device_info_data[loop_count];

	Font_Select(ARIAL_BOLD_14);
	TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
	,(uint8_t*)device_info_menu[loop_count],back_colour,text_colour);

	TFT_String(115,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
	,(uint8_t*)":",back_colour,text_colour);

	if(loop_count<2)
	{
		TFT_String(125,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)device_info_values[loop_count][value],back_colour,text_colour);
	}

	else if((loop_count==2)||(loop_count==4))
	{
		TFT_Number(125,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		device_info_data[loop_count],back_colour,text_colour);
	}

	else if(loop_count==3)
		{
		TFT_writenumber_float(125,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			device_info_data[loop_count],back_colour,text_colour);
		}
	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void about_fun(void)
{
	uint8_t exit_flag=1;
	uint8_t fact_longpress=0;
	uint8_t check_flag=0;
	POW.power_status_flag=1;
	//while(KEY.value);
	while(exit_flag)
	{
		Power_off();//27-03


		if(!ON_Flag)
			{
				break;//26-03
			}

			else if(POW.power_status_flag)//27-03

		   {
				POW.power_status_flag=0;//27-03
				device_info_frame();
				device_info_Opt_Frame();

		   }

		if(!KEY.value)
			{
			auto_lock_time=AUTO_EXIT_TIME;
			}
		if(!auto_lock_time)
			{

				break;
			}
		switch(KEY.value)
		{
		case  BOLUS_KEY:                         //MUTE_KEY:
						//check_flag=0;
//			//beep_sound();
			if(fact_longpress==0)
			{
				waitingtime_1=2;
				fact_longpress=1;
				while(KEY.value==BOLUS_KEY && fact_longpress)
				{
					if(!waitingtime_1)
					{
						check_flag=Passcode(0);
						if(check_flag)
						{
							factory_setting();
						}
						if(auto_lock_time)
						{
							POW.power_status_flag=1;
						}
						fact_longpress=0;
					}
				}
			}
			if(fact_longpress==1)
			{
				fact_longpress=0;
			}
			break;

		case BACK_KEY:

			exit_flag=0;
			while(KEY.value);
			break;


		}
	}

}

/**************************************************************************
 *Function Name :void device_info_Opt_Frame()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to used device info option  frame
***************************************************************************/

void infusion_modes_Opt_Frame()
{
	Frame_layout(1,3,(uint8_t*)"Infusion modes",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

/**************************************************************************
 *Function Name :void sys_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void infusion_mode_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;
	uint32_t tick_colour=0;


	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			tick_colour=MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			tick_colour=GOLD;
		}

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
		,(uint8_t*)infusion_modes_options[page][loop_count],back_colour,text_colour);
		 if(G_infusion_mode_opt==((page*4)+loop_count))
		 {
		  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(loop_count)*VER_TEXT_SPACE+6+2,250,0,back_colour,tick_colour);
		 }

	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void infusion_sett_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t device_variable=1;
	uint8_t edit_flag=0;
	uint8_t end_opt_no=3;
	uint8_t total_option=5;
	uint8_t down_opt_arr[2]={3,0};
	uint8_t up_opt_arr[2][2]={	{3,3},
								{0,0}

								};
	POW.power_status_flag=1;//27-03

	while(KEY.value);
	while(device_variable)
	{
		Power_off();//27-03


	if(!ON_Flag)
		{
			break;//26-03
		}

		else if(POW.power_status_flag)//27-03

	   {
			POW.power_status_flag=0;//27-03
			infusion_modes_Opt_Frame();

			infusion_mode_sett_Opt_Frame(page_no,option_no,end_opt_no);
			scroll_bar(page_no,option_no,total_option);
			while(KEY.value);
	   }

		switch(KEY.value)
			{
				case DOWN_KEY :   //DOWN_KEY
					//beep_sound();
//					if(!edit_flag)
//				{
//					if( (page_no==0) && (option_no<3) )
//								{
//									option_no++;
//								}
//								else
//									{
//										option_no=0;
//										page_no=0;
//										//prev_option=0;
//										end_opt_no=4;
//									}
					if( (page_no==0 && option_no<3)  )
						{
							option_no++;
						}
						else
						{
							page_no++;

							if(page_no>1)
							{
								page_no=0;
							}
							option_no=0;
							prev_option=option_no;
							end_opt_no=down_opt_arr[page_no];
							 layout();
							infusion_mode_sett_Opt_Frame(page_no,option_no,end_opt_no);
							//POW.power_status_flag=1;



						}
					if(option_no!=prev_option)
						{
							infusion_mode_sett_cursor(prev_option,option_no,page_no,infusion_modes_options);
							prev_option = option_no;
						}
					scroll_bar(page_no,option_no,5);
				while(KEY.value);
				break;

				case UP_KEY:     //UP_KEY
//					//beep_sound();
//					if(!edit_flag)
//				{
//					if(page_no==0 && option_no>0)
//					{
//						option_no--;
//					}
//					else
//					{
//							option_no=3;
//							page_no=0;
//							end_opt_no=4;
//					}
					if((page_no==0 && option_no>0))
					{
						option_no--;
					}

					else
					{


						if(!page_no)
						{
							page_no++;

						}
						else
						{
							page_no--;

						}
					option_no=up_opt_arr[page_no][0];
					end_opt_no=up_opt_arr[page_no][1];
					 layout();
					infusion_mode_sett_Opt_Frame(page_no,option_no,end_opt_no);
					//POW.power_status_flag=1;

				}
				if(option_no!=prev_option)
				{
					infusion_mode_sett_cursor(prev_option,option_no,page_no,infusion_modes_options);
					prev_option = option_no;
				}
				scroll_bar(page_no,option_no,5);
				while(KEY.value);
				break;

				case OK_KEY: 					 //OK_KEY
				//beep_sound();
				G_infusion_mode_opt=(page_no*4)+option_no;;
				EEPROM_datawrite(EEPROM_G_infusion_modes,G_infusion_mode_opt,1);
				device_variable=0;
				while(KEY.value);
				break;

				case BACK_KEY: 	//23-03				 //BACK_KEY
				//beep_sound();
				device_variable=0;
			//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
				while(KEY.value);
				break;
	}
}
}

/**************************************************************************
 *Function Name :void sys_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void infusion_mode_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][5])
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint32_t tick_colour=0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			tick_colour=GOLD;
			opt_no		= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			tick_colour=MAIN_FRAME_COL;
			opt_no= curr_option;
		}
		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
		(uint8_t*)array[page][opt_no],back_colour,text_colour);
		if(G_infusion_mode_opt==((page*4)+opt_no))
		 {
		  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(opt_no)*VER_TEXT_SPACE+6+2,250,0,back_colour,tick_colour);
		 }
	}
}

void brightness_mode_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	uint8_t device_sett_data[2]={brightness_mode,G_bright_level};
	uint16_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;



	for(loop_count=0;loop_count<end_opt;loop_count++)
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

		value = device_sett_data[loop_count];

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7
		,(uint8_t*)bright_modes_options[loop_count],back_colour,text_colour);

		if((loop_count==0)||(loop_count==1))
		{

			TFT_String(200+15,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
			(uint8_t*)glow_values[loop_count][value],back_colour,text_colour);

		}
	}
}
void brightness_sett_Screen()
{

	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t device_variable=1;
	uint8_t edit_flag=0;
	uint8_t end_opt_no=0;
	POW.power_status_flag=1;//27-03


	ON_Flag=1;
	 while(KEY.value);
	while(device_variable)
	{
		Power_off();//27-03


	if(!ON_Flag)
		{
			break;//26-03
		}

		if(POW.power_status_flag)//27-03

	   {
			POW.power_status_flag=0;//27-03
			brightness_mode_Opt_Frame();
			brightness_mode_sett_Opt_Frame(0,option_no,2);
			//scroll_bar(page_no,option_no,2);
	   }


		switch(KEY.value)
			{
				case DOWN_KEY :   //DOWN_KEY
					//beep_sound();
					if((!edit_flag)&&(!brightness_mode))
				{
						if(option_no<1)

								{
									option_no++;
								}
								else
									{
										option_no=0;

									}

					if(option_no!=prev_option)
					{

						brightness_mode_sett_cursor(prev_option,option_no,page_no,bright_modes_options);

						prev_option = option_no;
					}

				}
					//scroll_bar(page_no,option_no,2);
//					EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);
//					EEPROM_datawrite(EEPROM_Auto_bright_level,Auto_bright_level,1);
				while(KEY.value);
				break;

				case UP_KEY:     //UP_KEY
					//beep_sound();
					if((!edit_flag)&&(!brightness_mode))
				{
					if(option_no>0)
					{
						option_no--;
					}
					else
					{
							option_no=1;

					}

					if(option_no!=prev_option)
					{

						 brightness_mode_sett_cursor(prev_option,option_no,page_no,bright_modes_options);

						prev_option = option_no;
					}
//					EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);
//					EEPROM_datawrite(EEPROM_Auto_bright_level,Auto_bright_level,1);
				}
					//scroll_bar(page_no,option_no,2);
				while(KEY.value);
				break;

				case RIGHT_KEY:				 //LEFT_KEY:
				if(edit_flag)
				{
				brightness_subsetting( option_no,page_no,1);
				edit_flag=1;
				}
//				if(!edit_flag)
//				{
//					if((option_no==0)&&(page_no==0))
//					  {
//							TFT_ST7789V_left_Right_arrow_14_arial(58,188,0,LIGHT_GRAY,BLACK);
//							TFT_ST7789V_left_Right_arrow_14_arial(58,269,1,LIGHT_GRAY,BLACK);
//					  }
//					else if((option_no==1)&&(page_no==0))
//					  {
//						TFT_ST7789V_left_Right_arrow_14_arial(62+32,188,0,LIGHT_GRAY,BLACK);
//						TFT_ST7789V_left_Right_arrow_14_arial(62+32,269,1,LIGHT_GRAY,BLACK);
//					  }
////					edit_flag=1;
//				}
				while(KEY.value);
				break;

				case LEFT_KEY:				 //RIGHT_KEY:
				if(edit_flag)
				{
					brightness_subsetting( option_no,page_no,0);
					edit_flag=1;
				}
//				if(!edit_flag)
//				{
//					if((option_no==0)&&(page_no==0))
//					  {
//							TFT_ST7789V_left_Right_arrow_14_arial(58,188,0,LIGHT_GRAY,BLACK);
//							TFT_ST7789V_left_Right_arrow_14_arial(58,269,1,LIGHT_GRAY,BLACK);
//					  }
//					else if((option_no==1)&&(page_no==0))
//					  {
//						TFT_ST7789V_left_Right_arrow_14_arial((62+32),188,0,LIGHT_GRAY,BLACK);
//						TFT_ST7789V_left_Right_arrow_14_arial((62+32),269,1,LIGHT_GRAY,BLACK);
//					  }
//
////					edit_flag=1;
//				}
				while(KEY.value);
				break;

				case OK_KEY: 					 //OK_KEY



					if(brightness_mode==1)
					 {
//						UART_req();

					 }

					 if(!edit_flag)
						{
						 if((option_no==0)&&(page_no==0))
						  {
							TFT_ST7789V_left_Right_arrow_14_arial(58,188,0,LIGHT_GRAY,BLACK);
							TFT_ST7789V_left_Right_arrow_14_arial(58,269,1,LIGHT_GRAY,BLACK);
						  }
						 else if((option_no==1)&&(page_no==0))
						  {
							TFT_ST7789V_left_Right_arrow_14_arial((62+32),188,0,LIGHT_GRAY,BLACK);
							TFT_ST7789V_left_Right_arrow_14_arial((62+32),269,1,LIGHT_GRAY,BLACK);
						  }
							edit_flag = 1;
							{

							if(option_no==1)
								{
								TFT_ST7789V__rectangle_curve(10,35+(43*(2)),35+5,290+5,0,LIGHT_GRAY);
								TFT_ST7789V__rectangle_curve(15,125,32,(56*(G_bright_level+1)+4),0,BLACK);
								}
							}

							}
							else
							{
								edit_flag = 0;

								if((option_no==0)&&(page_no==0))
								  {
									TFT_ST7789V_left_Right_arrow_14_arial(58,188,0,LIGHT_GRAY,LIGHT_GRAY);
									TFT_ST7789V_left_Right_arrow_14_arial(58,269,1,LIGHT_GRAY,LIGHT_GRAY);
								  }
								 else if((option_no==1)&&(page_no==0))
								  {
									TFT_ST7789V_left_Right_arrow_14_arial((62+32),188,0,LIGHT_GRAY,LIGHT_GRAY);
									TFT_ST7789V_left_Right_arrow_14_arial((62+32),269,1,LIGHT_GRAY,LIGHT_GRAY);
								  }

							}

//				 EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);
//				 EEPROM_datawrite(EEPROM_Auto_bright_level,Auto_bright_level,1);
				 while(KEY.value);
				 break;

				case BACK_KEY: 	//23-03				 //BACK_KEY
					//beep_sound();
//					EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);
//					EEPROM_datawrite(EEPROM_Auto_bright_level,Auto_bright_level,1);
					if(!brightness_mode)
					{
						Auto_bright_level=G_bright_level;
					}
				device_variable=0;
				while(KEY.value);
				break;
	}
}
}
/**************************************************************************
 *Function Name :void sys_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void brightness_mode_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2])
{
	const uint8_t device[2]={brightness_mode,G_bright_level};
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
		(uint8_t*)array[opt_no],back_colour,text_colour);

		if((opt_no<2)||(opt_no==0))
		{
		TFT_String(200+15,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)glow_values[opt_no][device[opt_no]],back_colour,text_colour);

		}
		if((opt_no==0))
			{
//				Rectangle((57*(curr_option+6)),125,56-8,34,MAIN_FRAME_COL);
//				TFT_ST7789V__rectangle_curve(3+17,121,42,(57*(curr_option+6)),0,MAIN_FRAME_COL);

				TFT_ST7789V__rectangle_curve(10,35+(43*(2)),35+5,290+5,0,MAIN_FRAME_COL);
				TFT_ST7789V__rectangle_curve(15,125,32,(56*(G_bright_level+1)+4),0,MAIN_FRAME_COL);
			}

	}
}

void brightness_mode_Opt_Frame()
{
	Frame_layout3(1,2,(uint8_t*)"Brightness",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}

void brightness_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{
	uint8_t* bright_data[2] = {&brightness_mode,&G_bright_level};
	uint8_t max_value[2] =  {1,4};

	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	uint16_t inc;
	cur_value	= *bright_data[option_no];
	pre_value	= cur_value;

	if(INC_flag)
	{

		if(cur_value < max_value[option_no])
		{
				 cur_value++;

		}
	}

	else
	{

		if(cur_value)
		{
			cur_value--;

		 }

}


	if(pre_value != cur_value)
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(200+15,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)glow_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);

		Font_Select(ARIAL_BOLD_14);
		TFT_String(200+15,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)glow_values[option_no][cur_value],LIGHT_GRAY,MAIN_FRAME_COL);

		if(option_no==1)
			{
			Auto_bright_level=cur_value;

			if(!INC_flag)
			{
				Rectangle((56*(cur_value+1)+27),125,56-8,34,LIGHT_GRAY);
				TFT_ST7789V__rectangle_curve((56*(cur_value+1)),125,32,64,0,LIGHT_GRAY);
			}
			TFT_ST7789V__rectangle_curve(15,125,32,(56*(cur_value+1)+4),0,BLACK);
			}

	}

	*bright_data[option_no] = cur_value;
	}


void default_sett_Screen()//17-09-2024
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t device_variable=1;
	uint8_t edit_flag=0;
	uint8_t end_opt_no=0;
	uint8_t temp_default=0;
	POW.power_status_flag=1;//27-03

      while(KEY.value);
	while(device_variable)
	{
		Power_off();//27-03
//		 Font_Select(ARIAL_14);
//		TFT_Number(220,100,d_inf,NAVY,WHITE);

	if(!ON_Flag)
		{
		//d_inf=temp_default;
			break;//26-03
		}

		else if(POW.power_status_flag)//27-03

	   {
			POW.power_status_flag=0;//27-03
			edit_flag=0;
			default_modes_Opt_Frame();
			default_mode_sett_Opt_Frame(0,option_no,2);
	   }
	if(KEY.value)
	{
	 //while(KEY.value);
	   auto_lock_time=AUTO_EXIT_TIME;
	}
	if(!auto_lock_time)
	{
		device_variable=0;
//			  break;
	}
	switch(KEY.value)
	{
		case DOWN_KEY :   //DOWN_KEY
	//		beep_sound();
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

//			if(option_no!=prev_option)
//			{
			//	if(option_no!=0)      //review 2
			//	{
				default_mode_sett_cursor(prev_option,option_no,page_no,default_modes_options);
			//	}
				prev_option = option_no;
			//}
		}
				while(KEY.value);
				break;

				case UP_KEY:     //UP
				//	beep_sound();
					if(!edit_flag)
				{
					if(option_no) //ZOOZ_1
					{
						option_no=0;
					}
					else
					{

							option_no=1;

					}

				//	if(option_no!=prev_option)     //review 2
//					{
					//	if(option_no<1)
					//	{
					     default_mode_sett_cursor(prev_option,option_no,page_no,default_modes_options);
					//	}
						prev_option = option_no;
					//}
				}

					while(KEY.value);
				    break;
							case RIGHT_KEY:				 //LEFT_KEY:
				//				beep_sound();
                          if(edit_flag)
							{
									default_subsetting( option_no,1);

							}
							while(KEY.value);
							break;

							case LEFT_KEY:				 //RIGHT_KEY:
						//		beep_sound();
								  if(edit_flag)
							{
									default_subsetting( option_no,0);

							}


				while(KEY.value);
				break;


				case OK_KEY: 					 //OK_KEY
		//			beep_sound();
								if(option_no==0)
								{

								   infusion_sett_Screen();
								   edit_flag=1;
								  // while(1);
								   POW.power_status_flag=1;

								}

								else if((option_no==1)&& (!edit_flag))
								{

									TFT_ST7789V_left_Right_arrow_14_arial(50+40+5,162+20+20+5+5+5,0,LIGHT_GRAY,BLACK);
									TFT_ST7789V_left_Right_arrow_14_arial(50+40+5,230+20+20+5+5+5,1,LIGHT_GRAY,BLACK);
									//edit_subsetting_frame();
									edit_flag=1;

								}

								else
								{
									edit_flag = 0;

									if(option_no==1)
									{
										TFT_ST7789V_left_Right_arrow_14_arial(50+40+5,162+20+20+5+5+5,0,LIGHT_GRAY,LIGHT_GRAY);
										TFT_ST7789V_left_Right_arrow_14_arial(50+40+5,230+20+20+5+5+5,1,LIGHT_GRAY,LIGHT_GRAY);
									}
								//	 Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);

								}
								//delay_secs(2);
//								 Font_Select(ARIAL_14);
//							    TFT_Number(0,140,d_inf,YELLOW,RED);
								if((!d_inf)&& (!edit_flag))//d_inf
								{
									G_Menu_Brand_flag=0;
									Brand_List(0);
									POW.power_status_flag=1;

								}
//								else
//								{
//									   edit_flag=0;
//								}
							//	EEPROM_datawrite(EEPROM_DEF_BRAND,d_inf,1);

				 // POW.power_status_flag=1;
				while(KEY.value);
				break;

				case BACK_KEY: 	//23-03	//BACK_KEY
		//			beep_sound();
					G_Menu_Brand_flag=1;
					if(edit_flag)
					{
					//d_inf=temp_default;
					}
					else
					{
					//	EEPROM_datawrite(EEPROM_DEF_BRAND,d_inf,1);
					}
				device_variable=0;

				while(KEY.value);
				break;
	}
}
}
void default_modes_Opt_Frame()
{
	Frame_layout3(1,2,(uint8_t*)"default modes",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}
void default_mode_sett_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	//uint8_t device_sett_data[4]={G_infusion_modes,G_bright_level,G_sound_level,0};
	//uint16_t kvo_data[3]={G_kvo,G_KVO_fr};
		uint16_t default_data[1] = {d_inf};

	uint8_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;



	for(loop_count=0;loop_count<end_opt;loop_count++)
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

		value = default_data[loop_count];

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7
		,(uint8_t*)default_modes_options[loop_count],back_colour,text_colour);

		if(loop_count==1)
		{
			TFT_String(180+40+5+5,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
			(uint8_t*)default_values[0][value],back_colour,text_colour);

			TFT_String(180+20+5+6,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7
			,(uint8_t*)":",back_colour,text_colour);
		}

	}

}

void default_mode_sett_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2])
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
//	uint16_t default_data[1] = {d_inf};

uint8_t value = 0;


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

		//value = default_data[loop];
		if(opt_no==1)
		{
			value = d_inf;
		}

		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,
		(uint8_t*)array[opt_no],back_colour,text_colour);

		if(opt_no)
		{
			TFT_String(180+45+5,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,
			(uint8_t*)default_values[0][value],back_colour,text_colour);


			TFT_String(180+20+5+6,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7
			,(uint8_t*)":",back_colour,text_colour);
		}

//		else
//		{
//			TFT_String(180+40,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE),
//						(uint8_t*)default_values[page][value],LIGHT_GRAY,BLACK);
//		}

	}
}

void default_subsetting(uint8_t option_no, uint8_t INC_flag)
{
//	uint32_t* kvo_data[3] = {&G_kvo,&G_KVO_fr};
	uint16_t* default_data[2] = {0,&d_inf};
	uint8_t max_value[2] = {0,1};  //max array elements
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;
	cur_value	= *default_data[option_no];//1
	pre_value	= cur_value;//1

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
		TFT_String(180+45+5,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
		(uint8_t*)default_values[0][pre_value],LIGHT_GRAY,LIGHT_GRAY);

		//Font_Select(ARIAL_14);
		TFT_String(180+45+5,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+7,
		(uint8_t*)default_values[0][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);
	}

	*default_data[option_no] = cur_value;
}

void Device_sett_EEPROM_write()
{
            EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);//23-03
			EEPROM_datawrite(EEPROM_G_sound_level,G_sound_level,1);
			EEPROM_datawrite(EEPROM_G_lock_time,G_lock_time,1);
			EEPROM_datawrite(EEPROM_LAST_DATA,G_last_data,1);
			EEPROM_datawrite(EEPROM_G_drug_list,G_drug_list,1);
			EEPROM_datawrite(EEPROM_SCREEN_SAVER,G_screen_saver,1);
}
