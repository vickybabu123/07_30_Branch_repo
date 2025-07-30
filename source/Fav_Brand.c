
#include "LPC55S16.h"
#include "stdio.h"
#include "stdint.h"
#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "fsl_clock.h"
#include "Driver_I2C.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "shapes.h"
#include "Drug_List.h"
#include "Dosage_mode.h"
#include "Main_screen.h"
#include "Driver_Timer.h"
#include "Power_off.h"
#include  "Brand.h"
#include  "sound.h"


extern uint8_t auto_lock_time;
extern uint8_t total_fav_Brand;
extern uint8_t ON_Flag;	//21-03-2024
extern uint8_t brand_select_no;
extern uint8_t total_fav;
extern uint8_t fav_Brand_check[50];
extern uint16_t fav_Brand_list[11];
extern char Brand_Names[6][4][15];
extern uint8_t G_Menu_Brand_flag;//17.09.24
uint16_t Brand_select_no=0;
uint8_t tick_flag=0;
uint8_t Brand_favourite_func(void)
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t exit_flag=1;
	uint8_t current_option=0;

	uint8_t  max_fav_page=0;
	uint8_t  max_fav_opt=0;
	uint8_t end_option_no=3;
	uint8_t check_flag=0;
    uint16_t brand_new_no=0;


	auto_lock_time=AUTO_EXIT_TIME;//common_bug//ZOOZ_1

	G_Menu_Brand_flag=1;

	if(!total_fav_Brand)
	{
		Brand_List_layout();//ZOOZ_2

		 TFT_ST7789V__rectangle_curve(30,65,110,255,0,LIGHT_GRAY);
		 Font_Select(ARIAL_14);
		 TFT_String(85,110,(uint8_t *)"No Brands",LIGHT_GRAY,BLACK);
		 delay_secs(2);
		 check_flag=1;
		 exit_flag=0;//ZOOZ_2

	}

	else
	{

		max_fav_page=(total_fav_Brand-1)/4;
		max_fav_opt=(total_fav_Brand-1)%4;




		if(page_no == max_fav_page)
		{
			end_option_no = max_fav_opt;
		}
		else
		{
			end_option_no = 3;
		}


	}
		POW.power_status_flag=1;//27-03

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
			//	favourite_Frame();//27-03
				Brand_List_layout();
				Brand_favourite_opt_Frame(page_no,option_no,end_option_no);//27-03


				if(max_fav_page)
				{
					scroll_bar(page_no,option_no,total_fav_Brand);//23-03
					while(KEY.value);//28-03
				}
			}

			if(KEY.value)//common_bug  //ZOOZ_1
			{
				auto_lock_time=AUTO_EXIT_TIME;
			}
			else if(!auto_lock_time)
			{
		//		auto_lock_time=45;
				break;
			}//common_bug

		switch(KEY.value)
		{


		case DOWN_KEY :   //DOWN_KEY

	//		beep_sound();

				if((option_no<3 && page_no<max_fav_page)||(option_no < max_fav_opt))				// page_no<max_fav_page) || (option_no <max_fav_opt))
				{

				option_no++;

				}


				else 		//if(max_fav_page)
				{

					if(page_no<max_fav_page)
					{

						page_no++;



					}
					else
					{
						page_no=0;

					}


					if(page_no == max_fav_page)
					{
						end_option_no = max_fav_opt;
					}
					else
					{
						end_option_no = 3;
					}

					option_no = 0;
					prev_option=option_no;

					brand__drug_split_screen(page_no);
					Brand_favourite_opt_Frame(page_no,option_no,end_option_no);
				}






				if(option_no!=prev_option)
				{
					{

						Brand_favourite_cursor(prev_option,option_no,page_no);//,fav_drug_list);
					}

					prev_option = option_no;

				}
				if(max_fav_page)
					{
						scroll_bar(page_no,option_no,total_fav_Brand);//23-03

					}
				while(KEY.value);
				break;



				case UP_KEY:

					if(option_no>0)
						{
						option_no--;


						}

						else
						{
							if(page_no)
							{
							page_no--;
							}
							else
							{
								page_no=max_fav_page;

							}

							if(page_no==max_fav_page)
							{
								end_option_no=max_fav_opt;
							}
							else
							{
								end_option_no=3;


							}
							option_no=end_option_no;
							prev_option=option_no;
							brand__drug_split_screen(page_no);
							Brand_favourite_opt_Frame(page_no,option_no,end_option_no);





							 }
					if(option_no!=prev_option)
						{
							 if(option_no<3)
							  {

								 Brand_favourite_cursor(prev_option,option_no,page_no);//,fav_drug_list);
							  }

							prev_option = option_no;//1

						}

					if(max_fav_page)
						{
							scroll_bar(page_no,option_no,total_fav_Brand);//23-03

						}
				while(KEY.value);
				break;

			case OK_KEY: 					 //OK_KEY
	//			beep_sound();

				G_Menu_Brand_flag=0;
				tick_flag=1;
				brand_new_no=brand_set(current_option,page_no,option_no);
				Brand_select_no=brand_new_no;
				{
			//	TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS+((Brand_select_no%4)*VER_TEXT_SPACE_1)),270,0,MAIN_FRAME_COL,MAIN_FRAME_COL);//ZOOZ_2
				}
				Brand_select_no=fav_Brand_list[((page_no*4)+option_no)];
				//TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((option_no)*VER_TEXT_SPACE_1),270,0,LIGHT_GRAY,GOLD);//ZOOZ_2
				recently_used_fun(Brand_select_no);
				exit_flag=0;
				check_flag=1;
//				Font_Select(ARIAL_14);
//
//				TFT_Number(10,120,Brand_select_no,NAVY,YELLOW);
//				TFT_Number(10,140,page_no,RED,YELLOW);
//				TFT_Number(10,160,option_no,RED,YELLOW);

			//	delay_secs(2);


			while(KEY.value);
			break;


			case BACK_KEY:
		//		beep_sound();
					exit_flag=0;
					check_flag=0;



			while(KEY.value);
			break;

		}
	   }
	return check_flag;//ZOOZ_2
}

void Brand_favourite_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


    uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;
   	uint32_t fav_star_colour=0;//ZOOZ_2

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = BLACK;
			fav_star_colour=MAIN_FRAME_COL;//ZOOZ_2
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = WHITE;
			fav_star_colour=GOLD;//ZOOZ_2
		}

		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)Brand_Names[fav_Brand_list[((page*4)+loop_count)]/4][fav_Brand_list[((page*4)+loop_count)]%4],back_colour,text_colour);

//		Font_Select(ARIAL_BOLD_14);
//	    TFT_String(10,140,dose.drug_select_no,RED,YELLOW);


		 //if( ((page*4)+loop_count) == (dose.drug_select_no%4))
			if(Brand_select_no==fav_Brand_list[((page*4)+loop_count)])//ZOOZ_2
		  {
			  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(loop_count)*VER_TEXT_SPACE+6,250,0,back_colour,fav_star_colour);
		  }


	}
//	Font_Select(ARIAL_12);
//	TFT_Number(10,140,Brand_select_no,RED,YELLOW);//drug_select_no //brand_select
	 delay_secs(1);

}

void Brand_favourite_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)//  char* array[5][4][20])
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
			text_colour = WHITE;
			opt_no		= prev_option;
		}

		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = BLACK;
			opt_no= curr_option;
		}

		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);


			Font_Select(ARIAL_BOLD_14);

			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,


			(uint8_t*)Brand_Names[fav_Brand_list[((page*4)+opt_no)]/4][fav_Brand_list[((page*4)+opt_no)]%4],back_colour,text_colour);



			if(Brand_select_no == fav_Brand_list[((page*4)+opt_no)])	//22-03
			{
				 TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(opt_no)*VER_TEXT_SPACE+6,250,0,back_colour,GOLD);
			}



	}
}
//
//
//void fav_eeprom_write(void)//29-03
//{
//	uint8_t loop_count=0;
//	EEPROM_datawrite(EEPROM_TOT_BRAND_FAV,total_fav_Brand,1);
//	for(loop_count=0;loop_count<total_fav_Brand;loop_count++)
//	{
//		EEPROM_datawrite(EEPROM_FAV_BRAND_LIST+(loop_count*2),fav_Brand_list[loop_count],2);
//	}
//
//}
//
//void fav_eeprom_read(void)//29-03
//{
//	uint8_t loop_count=0;
//
//	total_fav=EEPROM_dataread(EEPROM_TOT_BRAND_FAV,1);
//	if(total_fav>10)
//	{
//		total_fav=0;
//	}
//
//	for(loop_count=0;loop_count<total_fav;loop_count++)
//		{
//		 fav_Brand_list[loop_count]=EEPROM_dataread(EEPROM_FAV_BRAND_LIST+(loop_count*2),2);
//			if(fav_Brand_list[loop_count]>600)
//			{
//				fav_Brand_list[loop_count]=0;
//				total_fav=0;
//                break;
//
//			}
//		}
//	fav_drug_fb_load();
//}
//void fav_drug_fb_load()
//{
//	uint8_t loop_count=0;
//
//
//	for(loop_count=0;loop_count<total_fav;loop_count++)
//	{
//		fav_Brand_check[fav_Brand_list[loop_count]]=1;
//	}
//
//}
