#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include"Keypad.h"
#include "TFT_Commands.h"
#include "Main_Menu.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Brand.h"
#include "Drug_List.h"
#include "Occlusion.h"
#include "Driver_I2C.h"
#include "SD_Calib.h"
#include "Main_screen.h"
#include "Driver_Timer.h"
extern unsigned long int keyvalue;


extern void Font_Select(uint8_t Font_Size);
extern uint16_t TFT_String(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t BG_COL,uint32_t TXT_COL);
extern void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
//extern void TFT_Number(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color);
extern void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *string);
extern uint16_t ADC_sdcalib(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples );
//extern uint32_t EEPROM_dataread(uint16_t addrs, uint16_t byte_val);
//extern uint32_t EEPROM_datawrite(uint16_t addrs, uint32_t data, uint16_t bytes_val);
uint16_t syringe_noise_filter(void);
void sd_cursor_sel(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]);
void SD_layout(void);
uint8_t SD_Calibration(void);
void sy_siz(unsigned int Pos);
void sy_size_setting(void);
void sd_baseval_check(void);
void pot_calibration(void);
unsigned char detect_syringe_size(unsigned int sybrand);
void ADC_read(uint8_t adc_input);
void syringe_selection_screen(void);
void frames_syringe_option(uint8_t page,uint8_t starting_count,uint8_t ending_count);
//uint8_t syr_place_screen(uint8_t* s_size1);
void syringe_adc_val(uint8_t *syringe_size);
void pop_up_screen(void);
void live_adc_calib(void);
void scroll_bar(uint8_t option_no,uint8_t page_no,uint8_t total_option);                                      //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
void calib_all_frame(uint8_t s_size);

extern char *Brand_Names[3][4];
extern uint16_t brand_select_no;
extern uint8_t brand;
extern uint8_t syringe_1;
extern char* syringe_settings[2][4];
extern char* syringe_min_max[2][4];
extern char* syringe[7];
extern uint8_t diff_val[9];
extern uint32_t G_loaded_adc_val[9];
extern uint32_t  G_minval[8];
extern uint32_t G_maxval[8];
//uint8_t s_size[2][4]={{_2ML_,_5ML_,_10ML_,_20ML_},{_30ML_,_50ML_}};
//uint8_t s_size[6]={{1,2,3,4,5,6}};
//extern uint8_t s_size;
//char* loaded_val_D[2][4]={{DISPOVAN_SD_2ML_DEFAULT_,DISPOVAN_SD_5ML_DEFAULT_,
//		DISPOVAN_SD_10ML_DEFAULT_,DISPOVAN_SD_20ML_DEFAULT_},{DISPOVAN_SD_30ML_DEFAULT_,DISPOVAN_SD_50ML_DEFAULT_}};

extern uint32_t G_Syr_size_val[2][4];





void SD_layout()
{

//	Rectangle(3,36,300,169,MAIN_FRAME_COL);
	//	TFT_ST7789V_lcd_shapes(38,38+163,306,300+15,LIGHT_GRAY_3);                                                                                             //SCROLLING BOX OUTSIDE LAYER
	//	TFT_ST7789V_lcd_shapes(97,97+57,306,303+12,BLACK);
	//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	Rectangle(7,34+(43*1),290,0, BLACK);
	Rectangle(7,34+(43*2),290,0,BLACK);
	Rectangle(7,34+(43*3),290,0,BLACK);
}

uint8_t syr_place_screen(uint8_t* s_size1)
{
	uint8_t s_size_loop=0;
    uint8_t check_flag=0;
    uint8_t s_size=*s_size1;
	Frame_Header(NEW_HDR_COL,DARK_TEAL,(uint8_t*)"Syringe size calibration");	// header


	CENTER_SCR(MAIN_FRAME_COL);  //center green
	 border_lines();


	TFT_ST7789V__rectangle_curve(10,60,117,300,0,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(112,90,(uint8_t*)"Place the ",WHITE,BLACK);
	TFT_String(93,130,(uint8_t*)syringe[s_size],WHITE,BLACK);
	TFT_String(142,130,(uint8_t*)" syringe ",WHITE,BLACK);


//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press        to confirm",(uint8_t*)"OK",2);
	s_size_loop=1;
	while(s_size_loop)
		{

			switch(KEY.value)
			{

			case OK_KEY:

         	check_flag=1;
         	s_size_loop=0;
         	while(KEY.value);
         	break;

			case BACK_KEY:

	        check_flag=0;
	        s_size_loop=0;
	        while(KEY.value);
         	break;

			}
		}
   return check_flag;
}
void min_max_val_header_footer()
{
	Font_Select(ARIAL_BOLD_14);
	//Frame_layout(1,3,(uint8_t*)"Syr    min    max    val",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	TFT_ST7789V__rectangle_curve(2,2,42,315,50,NEW_HDR_COL);

	TFT_String(21,11,(uint8_t*)"Syr             Min            Max",NEW_HDR_COL,HDR_TXT_COL);
	Frame_Footer(NEW_FTR_COL,WHITE,"OKAY","to confirm");
	CENTER_SCR(MAIN_FRAME_COL); 			//center green
	border_lines();
	separation_lines(1,3);
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

}
void syringe_adc_val(uint8_t *syringe_size)
{

	uint8_t loop_count = 0;
	uint8_t syr_size=0;
	// uint16_t xpos=0;

	syr_size=*syringe_size;
//	uint8_t page_no,option_no;
//	syringe_size=page_no*4+option_no;
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe size calibration");
	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	CENTER_SCR(MAIN_FRAME_COL);  //center green
	for(loop_count = 2; loop_count <3 ; loop_count++)
	{
		Rectangle(7,77+(43*loop_count),290,0, BLACK);
		TFT_String(160,84+(43*loop_count),(uint8_t*)":",MAIN_FRAME_COL,WHITE);

	}
	Font_Select(ARIAL_12);
	//TFT_String(85,131,(uint8_t*)"Brand",MAIN_FRAME_COL,WHITE);
	TFT_String(85,170,(uint8_t*)"Size",MAIN_FRAME_COL,WHITE);

	TFT_String(181,170,(uint8_t*)syringe[syr_size],MAIN_FRAME_COL,WHITE);

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(163,210,(uint8_t* )"to calibrate",ORANGE_DARK,WHITE);
	*syringe_size=syr_size;
}
void pop_up_screen(void)
{
    //uint8_t check_flag=0,s_size_loop=0;
	//Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Font_Select(ARIAL_BOLD_14);
	 TFT_ST7789V__rectangle_curve(35,65,105,255,0,WHITE);//70//80//120

	TFT_String(95+7,120-10,(uint8_t*)"Calibrated",WHITE,BLACK);


}
void pop_up_erase()
{


		Font_Select(ARIAL_BOLD_14);
		TFT_ST7789V__rectangle_curve(20,55,105,280,0,WHITE);//70//80//120

		TFT_String(95+5,120+10,(uint8_t*)"Calibrated",WHITE,BLACK);

}
void syringe_selection_screen(void)
{
	Frame_layout(1,3,(uint8_t*)"Syringe size calibration",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

void frames_syringe_option(uint8_t page,uint8_t starting_count,uint8_t ending_count)
{
	//	uint16_t data[4]={2,5,10,20};
	//	uint16_t data1[4]={30,50};

	//uint16_t value = 0;
	uint8_t loop_count=0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	//	Rectangle(0,0,320,240,MAIN_FRAME_COL);



	for(loop_count=0;loop_count<=ending_count;loop_count++)
	{
	//   if((page == 0 )|| (page == 1 && loop_count<3))//loop count minimum means here create the empty space for upcoming rows
	//	{
		if(loop_count == starting_count)
		{
			back_colour	= LIGHT_GRAY;
			text_colour= MAIN_FRAME_COL;
			Rectangle(9,48+(38*(loop_count)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		if((page==0)||(page==1 )||((page==2)&&(loop_count<3)))
		{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)syringe_settings[page][loop_count],back_colour,text_colour);
		if ((page == 0 && loop_count >= 0 && loop_count <= 3) ||
		    (page == 1 && loop_count >= 0 && loop_count <= 1))
		{
			TFT_Number(170,INITIAL_TEXT_POS+((loop_count)*VER_TEXT_SPACE)+6,
			G_loaded_adc_val[((page*4)+loop_count)+1],back_colour,text_colour);
		}



if(loop_count<=1)
{
	TFT_Number(170,INITIAL_TEXT_POS+((loop_count)*VER_TEXT_SPACE)+6,
			G_loaded_adc_val[((page*4)+loop_count)+1],back_colour,text_colour);
	}

//		TFT_String(150,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
//		(uint8_t*)":",back_colour,text_colour);
		}
    }
  }


void sd_cursor_sel(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4])
{

    uint32_t back_colour = 0;
	uint32_t text_colour	= 0;
	uint8_t loop		    = 0;
	uint8_t opt_no		    = 0;

	for(loop=0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour 	= LIGHT_GRAY;
			opt_no			= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour   = MAIN_FRAME_COL;
			opt_no		     = curr_option;
		}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
						(uint8_t*)array[page][opt_no],back_colour,text_colour);

    if((page == 0) ||(page == 1 && opt_no<=1))
        {
//		TFT_String(150,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
//				 (uint8_t*)":", back_colour,text_colour);

//	TFT_Number(170,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE),
//			(uint8_t*) G_Syr_size_val[page][opt_no],back_colour,text_colour);  //Right side data values
		TFT_Number(170,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
				G_loaded_adc_val[((page*4)+opt_no)+1],back_colour,text_colour);  //Right side data values

       }

  }
}

void pull_in_frame()
{
	CLRSCR(MAIN_FRAME_COL);


	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe size calibration");

	Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	CENTER_SCR(MAIN_FRAME_COL); 			//center green
	border_lines();
	Rectangle(9,48+(38*0),300,31,LIGHT_GRAY); //cursor

//	TFT_ST7789V__rectangle_curve(10,35,41,300,0,LIGHT_GRAY);
	Font_Select(ARIAL_BOLD_14);

	TFT_String(12,56,"Pull in",LIGHT_GRAY,MAIN_FRAME_COL);
	TFT_String(150,56,(uint8_t*)":",LIGHT_GRAY,MAIN_FRAME_COL);

}
void pullin_popup()
{
	Font_Select(ARIAL_BOLD_14);
	TFT_ST7789V__rectangle_curve(10,90,97,300,0,WHITE);//70//80//120
	TFT_String(120,96,(uint8_t*)"Caution",WHITE,RED);//LIGHT_GRAY_4
	Font_Select(ARIAL_12);
	TFT_String(30,120,(uint8_t*)"Ensure no syringe placed &",WHITE,BLACK);
//	TFT_String(43,138,(uint8_t*)"placed",WHITE,BLACK);
	TFT_String(30,148,(uint8_t*)"Place syringe holder inside",WHITE,BLACK);

}
void pull_out_frame()
{

	CLRSCR(MAIN_FRAME_COL);


	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe size calibration");

	Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	CENTER_SCR(MAIN_FRAME_COL); 			//center green
	border_lines();
	Rectangle(9,48+(38*0),300,31,LIGHT_GRAY); //cursor

	Rectangle(9,48+(38*0),300,31,LIGHT_GRAY); //cursor

//	TFT_ST7789V__rectangle_curve(10,35,41,300,0,LIGHT_GRAY);
	Font_Select(ARIAL_BOLD_14);

	TFT_String(12,56,"Pull out",LIGHT_GRAY,MAIN_FRAME_COL);
	TFT_String(150,56,(uint8_t*)":",LIGHT_GRAY,MAIN_FRAME_COL);


}
void pull_out_popup()
{
	Font_Select(ARIAL_BOLD_14);
		TFT_ST7789V__rectangle_curve(10,90,97,300,0,WHITE);//70//80//120
		TFT_String(120,96,(uint8_t*)"Caution",WHITE,RED);//LIGHT_GRAY_4
		Font_Select(ARIAL_12);

	TFT_String(30,126,(uint8_t*)"Pull out the syringe holder",WHITE,BLACK);
	TFT_String(65,156,(uint8_t*)"and twist it to lock",WHITE,BLACK);

}
void calib_all_frame(uint8_t s_size)
{
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe size calibration");
//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press        to confirm",(uint8_t*)"OK",2);
//	Rectangle(7,34+(43*1),290,0,BLACK);
//	TFT_ST7789V__rectangle_curve(10,35,41,300,0,LIGHT_GRAY);

	Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	CENTER_SCR(MAIN_FRAME_COL);  //center green
	Font_Select(ARIAL_12);
	Rectangle(7,34+(43*3)-10,290,0,BLACK);

	TFT_String(12,180-10,(uint8_t*)"Ensure",MAIN_FRAME_COL,WHITE);
	Font_Select(ARIAL_12);

//	TFT_String(77,180,(uint8_t*)syringe[s_size],MAIN_FRAME_COL,BLACK);
//	TFT_Number(180,51,s_size,MAIN_FRAME_COL,BLACK);

	TFT_String(139,180-10,(uint8_t*)"syringe is placed",MAIN_FRAME_COL,WHITE);



}
void calibrate_popup()
{
//	uint8_t page_no;
//	uint8_t option_no;
//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	Font_Select(ARIAL_BOLD_14);

	TFT_ST7789V__rectangle_curve(10,90,97,300,0,WHITE);//70//80//120
	TFT_String(70,117,(uint8_t*)"Calibrated value",WHITE,BLACK);
	TFT_String(95,147,(uint8_t*)"not in range.",WHITE,BLACK);

}
void calibrate_popup_erase()
{
	TFT_ST7789V__rectangle_curve(20,60,100,280,0,WHITE);//70//80//120
}

void minval_adc_option_screen(uint8_t page,uint8_t starting_count,uint8_t ending_count)
{
	uint8_t loop_count=0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=ending_count;loop_count++)
	{

			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;

	if((page == 0)|| (page = 1 && loop_count<=1))//loop count minimum means here create the empty space for upcoming rows
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)syringe_min_max[page][loop_count],back_colour,text_colour);


	if((page==0)||(page==1 && loop_count<=1))
	{

		TFT_Number(120,INITIAL_TEXT_POS+((loop_count)*VER_TEXT_SPACE)+6,
				G_minval[((page*4)+loop_count)+1],back_colour,text_colour);
		TFT_String(100,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
							(uint8_t*)":",back_colour,text_colour);
		TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
									(uint8_t*)":",back_colour,text_colour);
		TFT_Number(220,INITIAL_TEXT_POS+((loop_count)*VER_TEXT_SPACE)+6,
				G_maxval[((page*4)+loop_count)+1],back_colour,text_colour);

	}
    }
  }
}
