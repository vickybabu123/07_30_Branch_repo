#include "LPC55S16.h"
#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "TFT_Commands.h"
#include "Driver_Timer.h"
#include "TFT_String.h"


/**********************************************************************************
 *                         global_variable
***********************************************************************************/

char Brand_Names[6][4][15] ={
										{"DISPO VAN",		"BD",		 "BBraun",    "Nipro"},
										{"Baxter","Teleflex", "Romson",   "Terumo"}
//								{"BBRAUN",		"BD",		 "JMS",    "JNS"},
//								{"Polymed",  "Romson", "Safeti",   "Termuo"}
                                };

//char *brand_list[17]={"BBRAUN","BD", "JMS", "JNS",
//					  "Polymed",   "Romson",  "Safeti",  "Termuo"
//					   };
//
//char *brand_1[4][4]={	{"Dispo",	"BD",		 "BBRAUN",    "Nipro"},
//						{"JMS",      "Romson","Baxter", "Got Bul"},
//						{"MX Rad",    "Esthl",   "Termu","Telef"}
//					};

extern uint8_t brand_select_no;//22-03

extern uint8_t fav_Brand_check[50];//17.09.2024
extern uint8_t G_Menu_Brand_flag;
extern uint16_t Brand_select_no;
extern uint8_t tick_flag;
/**********************************************************************************
 *                         extern_variable
***********************************************************************************/

void Brand_List_layout ();
void  Brand_List_option(uint8_t page,uint8_t starting_count,uint8_t Final_count);
void Brand_list_option_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page, const char array[6][4][15]);



/**************************************************************************
                            extern Functions Call
***************************************************************************/

extern void TFT_ST7789V_14arial_writestr_New(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,
		uint32_t boarder_color,uint32_t fill_color);
extern void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
extern void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *string);

extern void TFT_ST7789V__rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,
		uint16_t radius,uint32_t colour);
extern void Font_Select(uint8_t Font_Size);
extern uint16_t TFT_String(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t BG_COL,uint32_t TXT_COL);
extern void Rectangle(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height,uint32_t Color);
extern uint8_t TFT_ST7789V_tick_symbol_14pt(uint16_t xpos, uint16_t ypos,    //22-03
	       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);//ZOOZ_2

/**************************************************************************
                               Function
***************************************************************************/



/*----------------------------------------------------------------
 * Function_name:Brandlistlayout
 * Arguments    :void
 * return       :void
 * Description  :Able to provide frame for the entire brand screen
------------------------------------------------------------------- */

void Brand_List_layout ()
{
	Frame_layout(1,3,(uint8_t*)"Brand List",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

/*----------------------------------------------------
 * Function_name:Brandlistoption
 * Arguments    :pageno,starting count,finalcount
 * return       :void
 * Description  :Able to provide option based on counts
-------------------------------------------------------- */

void  Brand_List_option(uint8_t page,uint8_t starting_count,uint8_t Final_count)
{
	 uint8_t loop_count=0;
		    uint32_t back_colour = 0;
			uint32_t text_colour = 0;
			uint32_t tick_color = 0;
		uint32_t fav_star_colour1 = 0;//17.09.2024

	      for(loop_count=0;loop_count<=Final_count;loop_count++)
			{
		    	if(loop_count == starting_count)
		    	{
		    		back_colour	= LIGHT_GRAY;
					text_colour=  MAIN_FRAME_COL;
					tick_color = MAIN_FRAME_COL;
						fav_star_colour1=MAIN_FRAME_COL;//17.09.2024
					//TFT_ST7789V__rectangle_curve(3,35+(43*(starting_count)),41,290,0,back_colour);
					Rectangle(9,48+(38*(starting_count)),288,30,back_colour);
		    	}
		    	else
		    	{
		    		tick_color = GOLD;
					back_colour	= MAIN_FRAME_COL;
				text_colour =LIGHT_GRAY;
				fav_star_colour1=GOLD;//17.09.2024
		    	}

		    	  Font_Select(ARIAL_BOLD_14);

			  TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
			  (uint8_t*)Brand_Names[page][loop_count],back_colour,text_colour);


             if(tick_flag==1)
             {
            	 brand_select_no=Brand_select_no;
             }

			  if((((page*4)+loop_count )== brand_select_no)&&((G_Menu_Brand_flag))) //17.09.2024
			  {
				  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((loop_count)*VER_TEXT_SPACE)+6,250,0,back_colour,tick_color);
				  tick_flag=0;
			  }


			  if((fav_Brand_check[((page*4))+loop_count]==1)&&(!G_Menu_Brand_flag))//17.09.2024
			  {
				 // fav_check[(page*4)+loop_count]=1;
				  TFT_ST7789V_star_14((INITIAL_TEXT_POS)+((loop_count)*VER_TEXT_SPACE)+6,270,1,back_colour,fav_star_colour1);

			  }

//			   Font_Select(ARIAL_BOLD_14);
//			   TFT_Number(0,70,G_Menu_Brand_flag,YELLOW,RED);
//			  if(page_no==2)
//			  {
//				  TFT_String(10,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE_1),
//				   (uint8_t*)add_data1[0],back_colour,text_colour);
//			  }
}
		  separation_lines(1,3);


		//	TFT_String(INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE_1),10,(uint8_t*)Brand_Names[page][loop_count],back_colour,text_colour);
		}

/*---------------------------------------------
 * Function_name:Brand_list_option_scroll
 * Arguments    :prev option,curr_option,page
 * return       :void
 * Description  :Able to scroll the option
----------------------------------------------- */

void Brand_list_option_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page, const char array[6][4][15])
 {

	uint32_t back_colour = 0;
    uint32_t text_colour = 0;
    uint32_t text_colour_1 = 0;
	uint8_t loop		 = 0;
	uint8_t opt_no		 = 0;
	uint32_t tick_color = 0;

	for(loop = 0; loop<3; loop++)
	{
		if(!loop)
		{
			back_colour     = MAIN_FRAME_COL;
			text_colour 	= LIGHT_GRAY;
			text_colour_1 = GOLD;
			opt_no			= prev_option;
			tick_color		=GOLD;
		}
		else
		{
			back_colour   = LIGHT_GRAY;
			text_colour   = MAIN_FRAME_COL;
			text_colour_1 = MAIN_FRAME_COL;
			opt_no		  = curr_option;
			tick_color	  =MAIN_FRAME_COL;
    	}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

	    Font_Select(ARIAL_BOLD_14);
	    TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
							(uint8_t*)array[page][opt_no],back_colour,text_colour);
		if(((page*4)+opt_no == brand_select_no)&&((G_Menu_Brand_flag)))//17.09.2024
		{
			TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,250,0,back_colour,tick_color);
		}

		if((fav_Brand_check[((page*4))+opt_no]==1 )&&(!G_Menu_Brand_flag))//17.09.2024
		{

			TFT_ST7789V_star_14((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,270,1,back_colour,text_colour_1);

			  }
			  		separation_lines(1,3);
 }
}
