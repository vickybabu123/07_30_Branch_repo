
#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Main_Menu.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "Power_off.h"
#include "Main_screen.h"
#include "Driver_I2C.h"

extern uint8_t ON_Flag;

extern uint8_t edit_flag;
//extern uint16_t xpos;

const char* bolus_limit_menu[7]={"2ml","5ml","10ml","20ml","30ml","50ml"};

uint16_t maxi_lmt[7]={5,30,40,50,50,50};
uint16_t min_limit=5;

uint16_t first_lmt[7]={0,5,30,40,50,50,50};
uint16_t second_lmt[7]={5,0,0,0,0,0};

//void bolus_layout()
//{
//
//	//Rectangle(3,36,300,169,BACK_GREEN2);
//	//TFT_ST7789V_lcd_shapes(38,38+163,306,300+15,LIGHT_GRAY_3);                                                                                             //SCROLLING BOX OUTSIDE LAYER
//	//TFT_ST7789V_lcd_shapes(97,97+57,306,303+12,BLACK);
////	Rectangle(10,55-8,300,20,BACK_GREEN2);//UP
////		Rectangle(10,170,300,18+5,BACK_GREEN2);//DOWN
//		//Rectangle(11,70-10-5-3,300-2,125-3+10+3,LIGHT_GRAY);//CENTRE BOX
//			Rectangle(12,72-10-5-3,297,116+2+10+3,MAIN_FRAME_COL);//CENTRE GREEN
//
////	Rectangle(7,34+(43*1),290,0, BLACK);
////	Rectangle(7,34+(43*2),290,0,BLACK);
////	Rectangle(7,34+(43*3),290,0,BLACK);
//			Rectangle(17,54+(32*1),277,0,LIGHT_GRAY);
//			Rectangle(17,54+(32*2),277,0,LIGHT_GRAY);
//			Rectangle(17,54+(32*3),277,0,LIGHT_GRAY);
//}



void bolus_limit_layout()
{
	CENTER_SCR(MAIN_FRAME_COL); //center green
	border_lines();
	separation_lines(1,3);

}
void bolus_limit()
{
	uint8_t bolus_limit_var=1;
	uint8_t option_no=0;
	uint8_t page_no=0;
	uint8_t exit_flag=0;
	uint8_t prev_option = 0;
	uint8_t end_opt_no=0;
	uint8_t auto_lock_time=0;
	uint16_t xpos=0;
	auto_lock_time=AUTO_EXIT_TIME;


	end_opt_no=3;

	POW.power_status_flag=1;
	while(KEY.value);
	while(bolus_limit_var)
	{
		Power_off();
				if(!ON_Flag)
				{
					break;//26-03
				}

				else if(POW.power_status_flag)
				   {
						POW.power_status_flag=0;//26-03
						bolus_limit_Frame();
						bolus_limit_Opt_Frame(page_no,option_no,end_opt_no);
						scroll_bar(page_no,option_no,6);//23-03
				   }

				if(KEY.value)//common_bug//ZOOZ_1
					{

						auto_lock_time=AUTO_EXIT_TIME;

					}
				else if(!auto_lock_time)
				{
					break;
				}//comm
		switch(KEY.value)
		{

		case DOWN_KEY :


		if(((page_no==1)&&(option_no<1))||((page_no==0)&&(option_no<3)))
		{
			option_no++;

			if(option_no!=prev_option)
			{
				bolus_limit_cursor(prev_option,option_no,page_no,bolus_limit_menu);
				prev_option = option_no;
			}
		}

		else
		{

			if(page_no<1)
			{
				option_no=0;
				end_opt_no=1;
				page_no++;
				prev_option=0;


			}
			else
			{
				option_no=0;
				end_opt_no=3;
				page_no=0;
				prev_option=0;

			}

			bolus_limit_layout();
			bolus_limit_Opt_Frame(page_no,option_no,end_opt_no);
		}
		scroll_bar(page_no,option_no,6);//23-03



		while(KEY.value);
		break;

		case UP_KEY:

		if(option_no)
		{
			option_no--;
		}

		else
		{

			if(page_no)
			{

					page_no--;
					option_no=3;
					end_opt_no=3;//22-03
                    prev_option=3;

			}
			else
			{
				page_no=1;
				option_no=1;
				prev_option=0;
				end_opt_no=1;
			}
			bolus_limit_layout();
			bolus_limit_Opt_Frame(page_no,option_no,end_opt_no);

		}

			if(option_no!=prev_option)
			{

				bolus_limit_cursor(prev_option,option_no,page_no,bolus_limit_menu);

				prev_option = option_no;
			}
			scroll_bar(page_no,option_no,6);//23-03

		while(KEY.value);
		break;

		case OK_KEY:

			bolus_limit_Opt_Frame(page_no,option_no,end_opt_no);
			exit_flag=Flow_rate_screen(&BOL.limit[(page_no*4)+option_no],0,maxi_lmt[(page_no*4)+option_no],1);//22-03    min_limit
			EEPROM_datawrite(EEPROM_G_bol_limit+0 ,BOL.limit[0],2);
			EEPROM_datawrite(EEPROM_G_bol_limit+2 ,BOL.limit[1],2);
			EEPROM_datawrite(EEPROM_G_bol_limit+4 ,BOL.limit[2],2);

			EEPROM_datawrite(EEPROM_G_bol_limit+(((page_no*4)+option_no)*2) ,BOL.limit[((page_no*4)+option_no)],2);

			POW.power_status_flag=1;//26-03


		while(KEY.value);
		break;


		case BACK_KEY:

		bolus_limit_var=0;

		while(KEY.value);
		break;

		}
	}
}



void bolus_limit_Frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Bolus limit",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}





void bolus_limit_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[7])
{

	uint8_t loop = 0;
		uint8_t opt_no  = 0;
		uint32_t back_colour = 0;
		uint32_t text_colour = 0;
		float value=0;
		char local_buffer[6];
		uint16_t xpos=0;



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

			value = BOL.limit[(page*4)+opt_no];


			Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

			Font_Select(ARIAL_BOLD_14);
//			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
//			(uint8_t*)array[(page*4)+opt_no],back_colour,text_colour);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)array[(page*4)+opt_no],back_colour,text_colour);

		//	TFT_String(100,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,(uint8_t*)":",back_colour,text_colour);

			sprintf(local_buffer,"%.1f",((float)value/10));    //store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,263);    //(uint8_t*)letter

			TFT_writenumber_float_1d(xpos-33,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,value,back_colour,text_colour);

			TFT_String(240,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,(uint8_t*)"ml",back_colour,text_colour);

			if(page==0)
			{
				for(int limit_count = 1; limit_count <=3 ; limit_count++)				//no of lines
					{

						Rectangle(8,44+(38*limit_count),289,0,LIGHT_GRAY);//LINE
					}
			}
			if(page==1)
			{
				for(int limit_count = 1; limit_count <3 ; limit_count++)				//no of lines
					{
						Rectangle(8,44+(38*limit_count),289,0,LIGHT_GRAY);//LINE
					}
			}



		}
}



void bolus_limit_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
	char local_buffer[6];



		float value = 0;
		uint32_t back_colour = 0;
		uint32_t text_colour = 0;
	    uint8_t loop_count=0;
	    uint16_t xpos=0;


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


			value =BOL.limit[(page*4)+loop_count];

			Font_Select(ARIAL_BOLD_14);

			TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)bolus_limit_menu[(page*4)+loop_count],back_colour,text_colour);


		//	TFT_String(100,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),(uint8_t*)":",back_colour,text_colour);



			sprintf(local_buffer,"%.1f",((float)value/10));    //store the integer variables in to string using sprintf

			xpos=Right_Alignments((uint8_t*)local_buffer,263);    //(uint8_t*)letter


			TFT_writenumber_float_1d(xpos-33,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,value,back_colour,text_colour);

			TFT_String(240,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)"ml",back_colour,text_colour);

		}
		if(page==0)
		{
			for(int limit_count = 1; limit_count <=3 ; limit_count++)				//no of lines
				{
					Rectangle(8,44+(38*limit_count),289,0,LIGHT_GRAY);//LINE
				}
		}
		if(page==1)
		{
			for(int limit_count = 1; limit_count <3 ; limit_count++)				//no of lines
				{
					Rectangle(8,44+(38*limit_count),289,0,LIGHT_GRAY);//LINE
				}
		}

}


