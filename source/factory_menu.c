
#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Main_Menu.h"
//#include "../Menu_Setting/Main_Menu.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "factory.h"
#include"Driver_I2C.h"
#include "Consumed_hrs.h"
#include "Power_off.h"
#include "sound.h"




extern uint8_t edit_flag;
//extern uint16_t xpos;
extern volatile uint8_t waitingtime_1;
extern uint8_t ON_Flag;
extern uint8_t password_one_flag;
extern uint8_t password_two_flag;
extern uint8_t password_three_flag;
extern uint8_t password_four_flag;
factorys FACT;
 uint16_t reset_year=0;
extern uint16_t G_year;
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} DateTime;
extern DateTime currentDateTime;
extern DateTime getRTCDateTime();

uint8_t G_qc_track=0;
uint8_t G_qc_year=0;
uint8_t packing_variable=0;
uint16_t G_Test_code;
uint32_t serialno;

const char* factory_menu[12]={"Mfg. year","Model","SW version","Serial.no","Reset","QC Track","Used Hours","Disconnection track",
                              "Motor reverse count","EEPROM cycle count","Error log"};

const char* qc_track[27]={"A","B","C","D","E","F","G","H","I",
						 "J","K","L","M","N","O","P","Q","R",
						 "S","T","U","V","W","X","Y","Z"};

const char* used_hrs_info_menu[5]={"Device ON","Battery ON","Battery Run","Factory Test"};//25-03

extern uint8_t ON_Flag;
extern uint8_t month_yr_edit_screen(uint16_t *Edit_value,uint8_t screen,uint16_t max_value);
extern uint8_t edit_screen(uint16_t *Edit_value,uint8_t screen,uint16_t max_value);
extern uint16_t serialno_screen(uint16_t *Value_1, uint32_t Limit);

extern void Packing_reset();
extern void Complete_Reset();
extern void scroll_bar(uint8_t page_no,uint8_t option_no,uint8_t total_option);     //23-03                                  //(uint8_t prev_option, uint8_t curr_option, uint8_t page)


void factory_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag);
void factory_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void factory_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[9]);
void factory_Frame(void);
void factory_setting();

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void factory_layout()
{
	uint8_t loop=0;
//	Rectangle(3,36,300,169,MAIN_FRAME_COL);

	for(loop=1;loop<=3;loop++)
	{
		Rectangle(17,54+(32*loop),277,0,LIGHT_GRAY);//LINE

//		Rectangle(7,34+(43*2),290,0,BLACK);
//		Rectangle(7,34+(43*3),290,0,BLACK);
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void factory_setting()
{
	uint8_t factory_var=1;
	uint8_t option_no=0;
	uint8_t page_no=0;
	uint8_t exit_flag=0;
	uint8_t prev_option = 0;
	uint8_t end_opt_no=0;
	uint8_t total_option=11;
	serialno=12345;
	uint8_t edit_flag=0;
//	uint8_t Reset_variable=0;
	DateTime currentDateTime = getRTCDateTime();
	 G_year =  2000 +currentDateTime.year;
	end_opt_no=3;
	POW.power_status_flag=1;//27-03
	while(factory_var)
	{
		Power_off();//27-03


		if(!ON_Flag)
		{
			break;//26-03
		}

		else if(POW.power_status_flag)//27-03
		{
			POW.power_status_flag=0;//27-03
			factory_Frame();
			factory_Opt_Frame(page_no,option_no,end_opt_no);
			scroll_bar(page_no,option_no,total_option);
		}

		switch(KEY.value)
		{

			case RIGHT_KEY:
			//beep_sound();
			if(edit_flag)
			{
				factory_subsetting( 1,page_no,1);
			}
			while(KEY.value);
			break;


			case LEFT_KEY:
			//beep_sound();
			if(edit_flag)
			{
				factory_subsetting( 1,page_no,0);
			}
			while(KEY.value);
			break;

			case DOWN_KEY :
			//beep_sound();
			if(!edit_flag)
			{

				if(((page_no==1)&&(option_no<3))||((page_no==0)&&(option_no<3))||((page_no==2)&&(option_no<2)))
				{
					option_no++;
					if(option_no!=prev_option)
					{
						factory_cursor(prev_option,option_no,page_no,factory_menu);

						prev_option = option_no;
					}
				}

				else
				{
					page_no++;
				if(page_no==1)
				{

					option_no=0;
					prev_option=0;
					end_opt_no=3;

				}
				else if(page_no==2)
				{

					option_no=0;
					prev_option=0;
					end_opt_no=2;

				}
				else
				{
					page_no=0;
					option_no=0;
					prev_option=0;
					end_opt_no=3;

				}

				layout();
				factory_Opt_Frame(page_no,option_no,end_opt_no);

				}

			}
			scroll_bar(page_no,option_no,total_option);

			while(KEY.value);
			break;

			case UP_KEY:
			//beep_sound();
			if(!edit_flag)
			{
				if(option_no)
				{
					option_no--;
				}

				else
				{

					if(page_no==1)
					{

						page_no=0;
//						Rectangle(3,36,300,169,MAIN_FRAME_COL);
						option_no=3;
						prev_option=option_no;
						end_opt_no=3;//23-03
//						factory_layout();
//						factory_Opt_Frame(page_no,option_no,end_opt_no);
					}
					else if(page_no==2)
					{
						page_no=1;
						option_no=3;
						prev_option=option_no;
						end_opt_no=3;

					}
					else
					{

						page_no=2;
//						Rectangle(3,36,300,169,MAIN_FRAME_COL);
						option_no=2;
						prev_option=option_no;
						end_opt_no=2;//23-03

					}
					layout();
					factory_Opt_Frame(page_no,option_no,end_opt_no);

				}
				if(option_no!=prev_option)
				{

					factory_cursor(prev_option,option_no,page_no,factory_menu);
					prev_option = option_no;
				}

			}
			scroll_bar(page_no,option_no,total_option);

			while(KEY.value);
			break;

			case OK_KEY:
			//beep_sound();
			if(!page_no)
			{
				if(option_no==0)
				{
					exit_flag=edit_screen(&FACT.factory[(page_no*4)+option_no],COLON_ON,3112);

				}
				else if(option_no==3)
				{
					exit_flag=serialno_screen(&FACT.factory[(page_no*4)+option_no],99999);
				}
				if(option_no==0||option_no==3)
				{
					if(exit_flag)
					{
						POW.power_status_flag=1;//2-04
					}

						else
						{


						}
				}
			}

			else
			{
				if((page_no==1 )&& (option_no == 1))
				{
					if(!edit_flag)
					{
						edit_flag = 1;
						TFT_ST7789V_left_Right_arrow_14_arial(95,190,0,LIGHT_GRAY,BLACK);
						TFT_ST7789V_left_Right_arrow_14_arial(95,260,1,LIGHT_GRAY,BLACK);
					}

					else
					{
						edit_flag = 0;
						TFT_ST7789V_left_Right_arrow_14_arial(95,190,0,LIGHT_GRAY,LIGHT_GRAY);
						TFT_ST7789V_left_Right_arrow_14_arial(95,260,1,LIGHT_GRAY,LIGHT_GRAY);

					}


				}

				else if(page_no==1 && option_no==2)
				{
					used_hrs();
					POW.power_status_flag=1;//2-04

				}
				else if(page_no==1 && !option_no)
				{
				      Passcode(0);

					if(password_one_flag)
					{
						reset_year=G_year;
						Packing_reset();
						password_one_flag=0;
					}

					else if(password_two_flag)
					{
						Complete_Reset();
						password_two_flag=0;
					}

					else
					{
						POW.power_status_flag=1;//2-04
					}

				}
			}
			EEPROM_datawrite(EEPROM_RESET_YEAR,reset_year,2);
			POW.power_status_flag=1;//2-04

			EEPROM_datawrite(EEPROM_QC_TRACK,G_qc_track,1);
			EEPROM_datawrite(EEPROM_SERIAL_NO,FACT.factory[3],3);
			EEPROM_datawrite(EEPROM_MFG_MON_YEAR,FACT.factory[0],2);

			while(KEY.value);
			break;


			case BACK_KEY:
			//beep_sound();
			factory_var=0;


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

void factory_Frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Factory settings",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void factory_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[9])
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint32_t value=0;


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

		value = FACT.factory[(page*4)+opt_no];

		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)array[(page*4)+opt_no],back_colour,text_colour);

		separation_lines(1,3);

		 if( ((page==0) && ((opt_no<=3)))||((page==1) && ((opt_no==1)||(opt_no==2))))
		{
			 TFT_String(170,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);
		}

		else
		{
		 // TFT_String(170,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);
		}



//		if(!page)
		{
			Font_Select(ARIAL_BOLD_14);

			if((opt_no==0)&&(page==0))
			{
				TFT_Number(200,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(value/100),back_colour,text_colour);

				if(value<1000)
				{

					TFT_String(215,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(uint8_t*)"/",back_colour,text_colour);

					TFT_Number(225,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(value%100),back_colour,text_colour);

				}
				else
				{

					TFT_String(225,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(uint8_t*)"/",back_colour,text_colour);

					TFT_Number(235,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(value%100),back_colour,text_colour);

				}

			}
			else if((page==0)&& (opt_no==1))
			{
				TFT_String(190,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)"SP-1100",back_colour,text_colour);
			}

			else if((page==0)&& (opt_no==2))
			{
				//TFT_String(190,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)VERSION,back_colour,text_colour);
				TFT_writenumber_float(190,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
						VERSION,back_colour,text_colour);

			}

			else if((page==0)&& (opt_no==3))
			{
				TFT_Number(200,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,value,back_colour,text_colour);
			}
		}

//		else
		{
			if((page==1)&&(opt_no==1))
			{
				Font_Select(ARIAL_BOLD_14);
				TFT_String(220,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)qc_track[G_qc_track],back_colour,text_colour);
			}
		}


	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void factory_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


	uint32_t value = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
    uint8_t loop_count=0;


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

		value = FACT.factory[(page*4)+loop_count];

		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)factory_menu[(page*4)+loop_count],back_colour,text_colour);

		separation_lines(1,3);

		 if( ((page==0) && ((loop_count<=3)))||((page==1) && ((loop_count==1)||(loop_count==2))))
		{
			 TFT_String(170,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);
		}

		else
		{
		 // TFT_String(170,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)":",back_colour,text_colour);
		}

//		if(page==0)
		{
			Font_Select(ARIAL_BOLD_14);
			if((page==0)&&(loop_count==0))
				{
					TFT_Number(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(value/100),back_colour,text_colour);

					if(value<1000)
					{

						TFT_String(215,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						(uint8_t*)"/",back_colour,text_colour);

						TFT_Number(225,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						(value%100),back_colour,text_colour);

					}
					else
					{

						TFT_String(225,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						(uint8_t*)"/",back_colour,text_colour);

						TFT_Number(235,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						(value%100),back_colour,text_colour);

					}
				}

			else if((page==0)&&(loop_count==1))
			{

				TFT_String(190,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)"SP-1100",back_colour,text_colour);

			}
			else if((page==0)&&(loop_count==2))
			{

				//TFT_String(190,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)VERSION,back_colour,text_colour);

				TFT_writenumber_float(190,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						VERSION,back_colour,text_colour);
			}

			else if((page==0)&&(loop_count==3))
			{
				TFT_Number(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,value,back_colour,text_colour);
			}
		}
//		else
		{

			if((page==1)&&(loop_count==1))
			{
				Font_Select(ARIAL_BOLD_14);
				TFT_String(220,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)qc_track[G_qc_track],back_colour,text_colour);

			}
		}

	}


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void factory_subsetting(uint8_t option_no,uint8_t page_no ,uint8_t INC_flag)
{

	uint8_t* factory_data[2] = {0,&G_qc_track};
	uint8_t max_value[2] =  {0,25};
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;

	cur_value	= *factory_data[option_no];
	pre_value	= cur_value;

	Font_Select(ARIAL_BOLD_14);

	if(INC_flag)
	{
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
			cur_value=25;
		}
	}

	Font_Select(ARIAL_BOLD_14);
	TFT_String(220,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,(uint8_t*)qc_track[pre_value],LIGHT_GRAY,LIGHT_GRAY);

	TFT_String(220,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,(uint8_t*)qc_track[cur_value], LIGHT_GRAY,MAIN_FRAME_COL);

	TFT_ST7789V_left_Right_arrow_14_arial(95,190,0,LIGHT_GRAY,BLACK);
	TFT_ST7789V_left_Right_arrow_14_arial(95,260,1,LIGHT_GRAY,BLACK);

	*factory_data[option_no]=cur_value;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void used_hrs(void)
{
	uint8_t exit_flag=1;
	uint8_t fact_longpress=0;
	//uint8_t check_flag=0;
	POW.power_status_flag=1;//26-03

	while(exit_flag)
	{
		Power_off();

		if(!ON_Flag) 	//MAIN.main_power_status_flag == 1 )
		{
		  break;//23-03
		}

		else if(POW.power_status_flag)
		{
			POW.power_status_flag=0;//26-03
			used_hrs_info_frame();
			used_hrs_info_Opt_Frame();
		}
		if(!KEY.value)
		{
			fact_longpress=0;
		}

		switch(KEY.value)
		{

		case BACK_KEY:

			exit_flag=0;

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

void used_hrs_info_frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Used hours",(uint8_t*)"OKAY",(uint8_t*)"to confirm");

}

/**************************************************************************
 *Function Name :void device_info_Opt_Frame()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to used device info option  frame
***************************************************************************/


void used_hrs_info_Opt_Frame()
{
    uint8_t loop_count=0;
    uint16_t used_hrs_info_data[5]={UTILIZED.device_hrs_cnt,UTILIZED.battery_hrs_cnt,UTILIZED.running_hrs_cnt,UTILIZED.factory_used_hrs};
	uint16_t value = 0;
    uint32_t back_colour=0;
    uint32_t text_colour=0;

	for(loop_count=0;loop_count<4;loop_count++)
	{
		back_colour	= MAIN_FRAME_COL;
		text_colour = WHITE;

		value = used_hrs_info_data[loop_count];
		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
		,(uint8_t*)used_hrs_info_menu[loop_count],back_colour,text_colour);

		TFT_String(160,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6
		,(uint8_t*)":",back_colour,text_colour);
		TFT_Number(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		value,back_colour,text_colour);
	}

}
