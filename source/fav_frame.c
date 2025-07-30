
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
#include "Main_screen.h"
#include "Driver_Timer.h"
#include "Dosage_mode.h"	//22-03

extern uint16_t fav_drug_list[11];
extern char Drug_Names[150][4][20];
extern uint16_t drug_new_no;	//22-03
void favourite_Frame_layout();


void favourite_Frame_layout(void)
{


//    Rectangle(11,47,298,145,MAIN_FRAME_COL); //center green
//	Rectangle(12,53,297,1,LIGHT_GRAY);
//	Rectangle(308,53,1,131,LIGHT_GRAY);
//	Rectangle(12,185,297,1,LIGHT_GRAY);
//	Rectangle(11,54,1,131,LIGHT_GRAY);



}


void favourite_Frame(void)
{
//
	Frame_layout(1,3,(uint8_t*)"Favourites",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

void favourite_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
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
			text_colour = MAIN_FRAME_COL;
			fav_star_colour=MAIN_FRAME_COL;//ZOOZ_2
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
		text_colour = LIGHT_GRAY;
			fav_star_colour=GOLD;//ZOOZ_2
		}

		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
		(uint8_t*)Drug_Names[fav_drug_list[((page*4)+loop_count)]/4][fav_drug_list[((page*4)+loop_count)]%4],back_colour,text_colour);

		// if( ((page*4)+loop_count) == dose.drug_select_no)
			if(dose.drug_select_no==fav_drug_list[((page*4)+loop_count)])//ZOOZ_2
			  {
				  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+7+(loop_count)*VER_TEXT_SPACE+0,250,0,back_colour,fav_star_colour);
			  }
		 separation_lines(1,3);
	}

}

void favourite_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)//,  char* array[5][4][20])
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

			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,


			(uint8_t*)Drug_Names[fav_drug_list[((page*4)+opt_no)]/4][fav_drug_list[((page*4)+opt_no)]%4],back_colour,text_colour);



			if(dose.drug_select_no == fav_drug_list[((page*4)+opt_no)])	//22-03


			{
				 TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+7+(opt_no)*VER_TEXT_SPACE+0,250,0,back_colour,GOLD);
			}
			separation_lines(1,3);


	}
}



void fav_edit_subsetting_frame(void)
{
	Font_Select(ARIAL_14);
	Rectangle(0,206,320,34,NEW_FTR_COL);
	TFT_String(65,210,(uint8_t *)" SEL",NEW_FTR_COL,HDR_TXT_COL);
	TFT_String(220,210,(uint8_t *)" DEL",NEW_FTR_COL,HDR_TXT_COL);
	TFT_ST7789V_lcd_drawcircle1(40,223,15,MAIN_FRAME_COL,MAIN_FRAME_COL);    //	 Left Circle
	TFT_ST7789V_lcd_drawcircle1(190,223,15,MAIN_FRAME_COL,MAIN_FRAME_COL);

	TFT_ST7789V_Mute_Symbol_12pt(218,182,0,MAIN_FRAME_COL,WHITE);
	Font_Select(ARIAL_9);

	TFT_String(30,218,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);

}


