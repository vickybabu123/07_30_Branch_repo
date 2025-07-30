/**************************************************************************
 *File Name			:RTC.c
 *Description		:In this file have RTC screen(date and month)
 *Developed By		:7303
 *Version			:V1
***************************************************************************/

#include "LPC55S16.h"
#include "Color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Keypad.h"
#include "stdbool.h"
#include "Shapes.h"
#include "Driver_Timer.h"
#include "Power_off.h"
#include "fsl_rtc.h"				//22-03
//#include "menu.h"
#include"Driver_I2C.h"
#include "string.h"
#include "Power_off.h"
#include "Main_menu.h"
#include "sound.h"
#include "RTC.h"
#include "RTC_driver.h"

//#include "Calibration_Screen.h"


/**************************************************************************
                          Global Variables
***************************************************************************/
extern uint16_t reset_year;
uint16_t G_date_mnth=0;
 uint16_t G_year=0;
 uint16_t G_time;
 uint16_t G_secs=0;
 uint16_t G_time_format;
 uint16_t year1;
  uint8_t month1;
  uint8_t day1;
  uint8_t hour1;
  uint8_t  minute1;
  uint8_t second1;
extern char letter1[2];
void setRTC();
const char* RTC_menu[4]={"Date & month","Year","Time","Format"};
const char* format_values[2]={"12-Hrs","24-Hrs"};

extern uint32_t RTC_dataread(uint16_t addrs, uint16_t byte_val);
extern uint8_t array_dig2[6];
extern uint8_t edit_flag;
extern uint8_t G_loop_count;
extern uint8_t ON_Flag;
extern uint32_t RTC_datawrite(uint8_t addrs1, uint8_t data, uint16_t bytes_val);
extern uint8_t edit_screen(uint16_t *Edit_value,uint8_t screen,uint16_t max_value);

/**************************************************************************
                            Functions Declaration
***************************************************************************/

void RTC_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void RTC_frame(void);

uint16_t decimal_screen(uint16_t *Edit_value,uint16_t Min_Limit,uint32_t Max_Limit);

extern rtc_datetime_t date;	//22-03
extern uint32_t currSeconds;
uint16_t date_limit[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

uint16_t date_month_limit=0;

/**************************************************************************
                            Functions Definations
***************************************************************************/

/**************************************************************************
 *Function Name :void RTC_screen()
 *Arguments     :void
 *Return        :void
 *Description	:In this function for RTC screen
***************************************************************************/
// Struct for DateTime



DateTime getRTCDateTime()
{
    DateTime rtcDateTime;

    rtcDateTime.day    = RTC_dataread(0x04, 1);
    delay_ms(100);

    rtcDateTime.month  = RTC_dataread(0x05, 1);
    delay_ms(100);
    rtcDateTime.second = RTC_dataread(0x00, 1);
    delay_ms(100);
    rtcDateTime.minute = RTC_dataread(0x01, 1);
    delay_ms(100);
    rtcDateTime.hour   = RTC_dataread(0x02, 1);
    delay_ms(100);
    rtcDateTime.year   = RTC_dataread(0x06, 1);



    return rtcDateTime;
}


void RTC_screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t RTC_variable=1;
	uint8_t temp_time_format=0;



//	RTC_dataread(0x00, 1);
//	G_date_mnth=    RTC_dataread(0x04, 1) *100+ RTC_dataread(0x05, 1);	// 22-03
//	G_year=RTC_dataread(0x06, 1);
//	G_time =  RTC_dataread(0x02, 1)*100 +  RTC_dataread(0x01, 1);


	 DateTime currentDateTime = getRTCDateTime();


	 G_date_mnth =(currentDateTime.day * 100 + currentDateTime.month); // Format: DDMM
	 G_time = currentDateTime.hour * 100 + currentDateTime.minute;    // Format: HHMM
	 G_year =  2000 +currentDateTime.year;
	 G_secs=currentDateTime.second;

	 RTC_dataread(0x00, 1);
	POW.power_status_flag=1;

    while(KEY.value)
    {

    }
	while(RTC_variable)
	{

		 Power_off();

			 if(!ON_Flag)
			   {
				  break; //26-03
			   }

			 else if(POW.power_status_flag)
				 {
				   POW.power_status_flag=0;
				   RTC_frame();
				   RTC_Opt_Frame(0,option_no,3);
				   if(option_no==3 && edit_flag)
				   {
					   TFT_ST7789V_left_Right_arrow_14_arial(163+4,190,0,LIGHT_GRAY,BLACK);
					   TFT_ST7789V_left_Right_arrow_14_arial(163+4,276,1,LIGHT_GRAY,BLACK);

				   }

				 }

		switch(KEY.value)
		{
			case DOWN_KEY :
				//beep_sound();
			if(!edit_flag)
			{

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

						RTC_cursor(prev_option,option_no,page_no);//,RTC_menu);
						prev_option = option_no;
				}
			}

			while(KEY.value);
			break;

			case UP_KEY:
				//beep_sound();
			if(!edit_flag)
			{
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
					RTC_cursor(prev_option,option_no,page_no);//,RTC_menu);
					prev_option = option_no;
				}
			}
			while(KEY.value);
			break;

			case RIGHT_KEY:
				//beep_sound();
			if(edit_flag)
			{
				format_subsetting( option_no,1);
				TFT_ST7789V_left_Right_arrow_14_arial(163+4,190,0,LIGHT_GRAY,BLACK);
				TFT_ST7789V_left_Right_arrow_14_arial(163+4,276,1,LIGHT_GRAY,BLACK);

			}

			while(KEY.value);
			break;

			case LEFT_KEY:
				//beep_sound();
			if(edit_flag)
			{
				format_subsetting( option_no,0);
				TFT_ST7789V_left_Right_arrow_14_arial(163+4,190,0,LIGHT_GRAY,BLACK);
				TFT_ST7789V_left_Right_arrow_14_arial(163+4,276,1,LIGHT_GRAY,BLACK);
			}

			while(KEY.value);
			break;

			case OK_KEY:
				//beep_sound();
			if(!edit_flag)
			{

				if(option_no==0)
				{

					date_month_limit=date_limit[currentDateTime.month-1]*100+12;

					edit_screen(&G_date_mnth,COLON_ON,date_month_limit);

					month1  = G_date_mnth%100U;  //G_date_mnth
				    day1    = G_date_mnth/100U;
				    RTC_datawrite(0x04, day1, 1);  // DATE
				    delay_ms(100);
				    RTC_datawrite(0x05, month1, 1);   // MONTH
				    delay_ms(100);

				}
				else if(option_no==1)
				{

					decimal_screen(&G_year,reset_year,2099);
					year1   = 2000+G_year;
				    RTC_datawrite(0x06, year1, 1);  // YEAR
				    delay_ms(100);

				}
				else if(option_no==2)
				{

					edit_screen(&G_time,TIME_HHMM,2359);
				    hour1   = G_time/100;
					minute1 = G_time%100;
				    RTC_datawrite(0x01, minute1, 1);  // MIN
				    delay_ms(100);
				    RTC_datawrite(0x02, hour1, 1);  // HOUR
				    delay_ms(100);
				}
				else if(option_no==3)
				{
				//	edit_flag=1;
//					TFT_ST7789V_left_Right_arrow_14_arial(163+4,190,0,LIGHT_GRAY,BLACK);
//					TFT_ST7789V_left_Right_arrow_14_arial(163+4,276,1,LIGHT_GRAY,BLACK);
//					edit_subsetting_frame();
				}

				//POW.power_status_flag=1;

			//	year1   = 2000+G_year;//2025;		//22-03


//
//				month1  = G_date_mnth%100U;  //G_date_mnth
//			    day1    = G_date_mnth/100U;
//			    hour1   = G_time/100;
//				minute1 = G_time%100;
				second1 = 0;
				currSeconds = 00;


//			    RTC_datawrite(0x04, day1, 1);  // DATE
//			    delay_ms(100);
//			    RTC_datawrite(0x05, month1, 1);   // MONTH
//			    delay_ms(100);
			    RTC_datawrite(0x00, second1, 1);  // SEC
			    delay_ms(100);
//			    RTC_datawrite(0x01, minute1, 1);  // MIN
//			    delay_ms(100);
//			    RTC_datawrite(0x02, hour1, 1);  // HOUR
//			    delay_ms(100);
			  //  RTC_datawrite(0x06, year1, 1);  // YEAR






			}
			else
			{
				edit_flag = 0;


//				if(option_no==3)
//				{
//					TFT_ST7789V_left_Right_arrow_14_arial(163+4,190,0,LIGHT_GRAY,LIGHT_GRAY);
//					TFT_ST7789V_left_Right_arrow_14_arial(163+4,276,1,LIGHT_GRAY,LIGHT_GRAY);
//
//				}

			}
			temp_time_format=G_time_format;
//	    	EEPROM_datawrite(EEPROM_G_time_format,G_time_format,1);
	    	while(KEY.value);
			break;

			case BACK_KEY:
		    //beep_sound();
		    if(edit_flag)
			{
		    	 G_time_format=temp_time_format;
			}

		    else
		    {
//		    	EEPROM_datawrite(EEPROM_G_time_format,G_time_format ,1);
		    }
		    RTC_variable=0;
			option_no=0;
			page_no=0;
			edit_flag = 0;

//			Rectangle(3,36,300,169,MAIN_FRAME_COL);
			while(KEY.value);
			break;
		}
	}
}

void setRTC()
{
    RTC_datawrite(0x00, 0, 1);        // SEC
    RTC_datawrite(0x01, minute1, 1);  // MIN
    RTC_datawrite(0x02, hour1, 1);    // HOUR
    RTC_datawrite(0x04, day1, 1);     // DATE
    RTC_datawrite(0x05, month1, 1);   // MONTH
    RTC_datawrite(0x06, 24, 1);       // YEAR
}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void option_footer_frame()
{
//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
	Frame_Footer_new(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"OKAY");
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
	CENTER_SCR(MAIN_FRAME_COL);  //center green
}

/**************************************************************************
 *Function Name :void RTC_frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for RTC frame
***************************************************************************/

void RTC_frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Date & Month",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	separation_lines2(1,3);
}

/**************************************************************************
 *Function Name :void RTC_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void RTC_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)

{
	uint8_t loop_count=0,xpos=0;
		uint16_t RTC_data[4]={G_date_mnth,G_year,G_time,G_time_format};
		uint16_t value;
		uint32_t back_colour = 0;
		uint32_t text_colour = 0;
		char local_buffer[6];
		for(loop_count=0;loop_count<=3;loop_count++)  //local var
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

			value = RTC_data[loop_count];

			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)RTC_menu[loop_count],back_colour,text_colour);

			TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			(uint8_t*)":",back_colour,text_colour);

			if(loop_count==0)
			{
				TFT_Number(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						(value/100),back_colour,text_colour);

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

			if(loop_count==1)
			{
				TFT_Number(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						value,back_colour,text_colour);
			}
			if(loop_count==2)
			{

				sprintf(local_buffer,"%04d",value);    //store the integer variables in to string using sprintf
				xpos=Right_Alignments((uint8_t*)local_buffer,263);
				TFT_Time_disp(xpos-14,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
						value,back_colour,text_colour);
			}
			if(loop_count==3)
			{
				TFT_String(200,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)format_values[1],back_colour,text_colour);
			}
		}
									 Font_Select(ARIAL_14);

									 TFT_Number(10,40,G_secs,RED,YELLOW);
}

/**************************************************************************
 *Function Name :void RTC_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/

void RTC_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)// const char* array[4])
{
	const uint16_t RTC_print[4]={G_date_mnth,G_year,G_time,G_time_format};
		uint8_t loop = 0;
		uint8_t opt_no = 0,xpos=0;
		char local_buffer[6];
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
			(uint8_t*)RTC_menu[opt_no],back_colour,text_colour);


			TFT_String(180,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)":",back_colour,text_colour);

			if(opt_no==0)
			{
				TFT_Number(200,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
						(RTC_print[opt_no]/100),back_colour,text_colour);

				if(RTC_print[opt_no]<1000)
				{

					TFT_String(215,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(uint8_t*)"/",back_colour,text_colour);

					TFT_Number(225,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(RTC_print[opt_no]%100),back_colour,text_colour);

				}
				else
				{

					TFT_String(225,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(uint8_t*)"/",back_colour,text_colour);

					TFT_Number(235,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
					(RTC_print[opt_no]%100),back_colour,text_colour);

				}
			}

			if(opt_no==1)
			{
				TFT_Number(200,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
				RTC_print[opt_no],back_colour,text_colour);
			}

			if(opt_no==2)
			{
				sprintf(local_buffer,"%04d",RTC_print[opt_no]);    //store the integer variables in to string using sprintf
				xpos=Right_Alignments((uint8_t*)local_buffer,263);
				TFT_Time_disp(xpos-14,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
						RTC_print[opt_no],back_colour,text_colour);
			  }
			if(opt_no==3)
			{
				TFT_String(200,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
				(uint8_t*)format_values[1],back_colour,text_colour);
//		Font_Select(ARIAL_14);
//		TFT_Number(80,40,G_date_mnth,RED,YELLOW);
//
//		Font_Select(ARIAL_14);
//		TFT_Number(80,80,G_time,RED,YELLOW);
//
//		Font_Select(ARIAL_14);
//			TFT_Number(80,135,G_secs,RED,YELLOW);
//
//			Font_Select(ARIAL_14);
//					TFT_Number(80,160,G_year,RED,YELLOW);
			}
		}
}

/**************************************************************************
 *Function Name :void format_subsetting(uint8_t option_no, uint8_t INC_flag)
 *Arguments     :uint8_t option_no, uint8_t INC_flag
 *Return        :void
 *Description	:In this function array arguments print as right side
***************************************************************************/

void format_subsetting(uint8_t option_no, uint8_t INC_flag)
{
	uint16_t* format_data[4] = {0,0,0,&G_time_format};
		uint8_t max_value[4] = {0,0,0,1};
		uint8_t cur_value = 0;
		uint8_t pre_value = 0;
		cur_value	= *format_data[option_no];
		pre_value	= cur_value;

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
				cur_value=1;
			}
		}
	//	Rectangle(11,47,298,145,MAIN_FRAME_COL);//CENTRE GREENB
		if(pre_value != cur_value)
		{

				Font_Select(ARIAL_BOLD_14);
			TFT_String(200,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
			(uint8_t*)format_values[1],LIGHT_GRAY,MAIN_FRAME_COL);

			 TFT_String(200,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
			 (uint8_t*)format_values[1],LIGHT_GRAY,MAIN_FRAME_COL);

		}
		*format_data[option_no] = cur_value;
}
