#include "LPC55S16.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "Flowrate_Screen.h"
#include "Power_off.h"
#include "Password.h"
#include "Driver_Timer.h"
#include "Main_screen.h"

void passcode_header_footer_frame()
{
	 Rectangle(0,0,320,240,MAIN_FRAME_COL);
		//   Font_Select(BEBAS_43);
	     Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Passcode");
	     Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	     CENTER_SCR(MAIN_FRAME_COL);  //center green

	     right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	     	Font_Select(ARIAL_BOLD_14);
	     	TFT_String(195-30-5+10-7,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

}
void passcode_header_footer_popup_frame()
{
	 Rectangle(0,0,320,240,MAIN_FRAME_COL);

	     Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Passcode");

	 	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	 	//CENTRE GREEN
	 	CENTER_SCR(MAIN_FRAME_COL);  //center green
	    border_lines();
	 	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	 	Font_Select(ARIAL_BOLD_14);
	 	TFT_String(195-30-5+10-7,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
//	     TFT_ST7789V_lcd_drawcircle1(18,128,12,MAIN_FRAME_COL,MAIN_FRAME_COL);           //CIRCLE
//	     TFT_ST7789V_lcd_drawcircle1(297,127,12,MAIN_FRAME_COL,MAIN_FRAME_COL);
}
void Pc_split_frame()
{
	 uint8_t loop_count=0;


	 for(loop_count=0;loop_count<=4;loop_count++)
		 {
		     Rectangle(70+(45*loop_count),68,0,115,BLACK);//69///spliting
		 }
}
//void Left_Right_Indi_passcode(uint8_t cursor_pos)
//{
//	uint32_t Right_bg_color;
//	uint32_t left_bg_color;
//
//	if(cursor_pos==3 )
//	{
//    Right_bg_color	=	LIGHT_GRAY_3;
//    left_bg_color		=	WHITE;
//	}
//
//	else if(cursor_pos==0)
//	{
//     left_bg_color=		LIGHT_GRAY_3;
//     Right_bg_color=WHITE;
//	}
//
//	else
//	{
//	Right_bg_color	=WHITE;
//	left_bg_color	=WHITE;
//	}
//
//    TFT_ST7789V_lcd_drawcircle1(18,128,12,left_bg_color,left_bg_color);           					//	 Left Circle
//    TFT_ST7789V_lcd_drawcircle1(297,127,12,Right_bg_color,Right_bg_color);   					// Right Circle
//
//    TFT_ST7789V_left_Right_arrow_14_arial(121,13,0,left_bg_color,NEON_BLUE_1);        //	Left Arrow
//    TFT_ST7789V_left_Right_arrow_14_arial(120,295,1,Right_bg_color,NEON_BLUE_1);  //	Right Arrow
//
//}
void password_option_frame(uint8_t Cursor_no)
{

    uint8_t loop_count=0;
    uint32_t back_colour   = 0;
    uint32_t text_colour= 0;
    uint16_t ypos=0;
    char pass_buffer[2]={'\0','\0'};

    Font_Select(BEBAS_43);

	for(loop_count=0;loop_count<=3;loop_count++)
	{
		if(loop_count==Cursor_no)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = BLACK;
			TFT_ST7789V__rectangle_curve(71+(45*Cursor_no),64,125,43,0,back_colour);

							pass_buffer[0]=password.array_dig_2[Cursor_no]+48;
							ypos=Letters_Alignments(70+(45*(Cursor_no)),
									70+(45*(Cursor_no+1)), (uint8_t*)pass_buffer,0);
							TFT_Number(ypos,CALIB_EDT_YPOS,password.array_dig_2[Cursor_no],back_colour,text_colour);

            TFT_ST7789V_up_down_arrow_20_arial(79,84+(45*Cursor_no),0,back_colour,text_colour);  		//UP_ARROW
            TFT_ST7789V_up_down_arrow_20_arial(166,84+(45*Cursor_no),1,back_colour,text_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = WHITE;
			TFT_ST7789V_lcd_drawcircle1(92+(45*(loop_count)),125,12,WHITE,WHITE);


		}


		//CIRCLE
	}

}
void password_option_scroll(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
//	uint8_t ypos_3;
//	uint8_t space=0;
//	char letter_3[2]={'\0','\0'} ;
	uint16_t ypos=0;
	 char pass_buffer[2]={'\0','\0'};
	uint32_t back_colour   = 0;
	uint32_t text_colour= 0;
	uint8_t loop	= 0;
//	uint8_t loop_count	= 0;
	uint8_t Cursor_no   = 0;
	uint32_t arrow_fill_color = 0;
	uint32_t arrow_bg_color   = 0;


	for(loop = 0; loop<2; loop++)
	 	{
	 		if(!loop)
	 		{
	 			back_colour			= MAIN_FRAME_COL;
	 			text_colour 		= WHITE;
	 			Cursor_no			= prev_cursor_no;
                arrow_bg_color		= MAIN_FRAME_COL;
                arrow_fill_color	= MAIN_FRAME_COL;
                TFT_ST7789V__rectangle_curve(71+(45*Cursor_no),64,125,43,0,back_colour);



    			TFT_ST7789V_lcd_drawcircle1(92+(45*(Cursor_no)),125,12,WHITE,WHITE);



	 		}
	 		else
	 		{
	 			back_colour 		= LIGHT_GRAY;
	 			text_colour  		= BLACK;
	 			Cursor_no			= current_cur_no;
	 			arrow_bg_color		= LIGHT_GRAY;
				arrow_fill_color	= BLACK;

                TFT_ST7789V__rectangle_curve(71+(45*Cursor_no),64,125,43,0,back_colour);

                pass_buffer[0]=password.array_dig_2[Cursor_no]+48;
				ypos=Letters_Alignments(70+(45*(Cursor_no)),
						70+(45*(Cursor_no+1)), (uint8_t*)pass_buffer,0);
				TFT_Number(ypos,CALIB_EDT_YPOS,password.array_dig_2[Cursor_no],back_colour,text_colour);
             //    TFT_Number(75+(45*Cursor_no),98,password.array_dig_2[Cursor_no],back_colour,text_colour);


	 		}


	TFT_ST7789V_up_down_arrow_20_arial(79,84+(45*Cursor_no),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
	TFT_ST7789V_up_down_arrow_20_arial(166,84+(45*Cursor_no),1,arrow_bg_color,arrow_fill_color);
	}

}


