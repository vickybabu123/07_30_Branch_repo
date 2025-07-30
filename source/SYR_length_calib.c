/*
 * Syringe_Length_calibration_setting.c
 *
 *  Created on: 30-Oct-2023
 *      Author: Software4
 */
#include "Linear_Header.h"
#include "Syringe_calib_header.h"
#include "Driver_ADC.h"
#include "color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Keypad.h"
#include "Shapes.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "Power_off.h"
#include "Driver_I2C.h"
#include "SD_Calib.h"
#include "Flowrate_Screen.h"
#include "Main_screen.h"

extern uint8_t ON_Flag;

extern uint16_t timer_count;
extern float rem_volume;
//extern uint8_t ssize;

float __1_ml_ADC;
extern uint16_t adc_rd;
float __0_ml_ADC;
//uint8_t syr_size_detect();
void system_setting();
void calibration_setting();
void syringe_calibration();
//void syr_length_edit();
//void syr_length_adc_editing();

void frame_footer_ok_conform();
extern void frame_syringe_size_glow(uint8_t state,uint8_t screen);
void frame_footer_press_ok_to_conform();
void footer_press_ok_to_calirate();
void frame_footer_press_ok_to_edit();

//void plunger_Pout_ADC_calib_main_screen();
//void plunger_Pin_ADC_calib_main_screen();
void syringe_length_and_collar_main_screen(uint8_t cursor,uint8_t screen);
void warning_screen_ADC(uint8_t);
uint16_t  syringe_length_edit(uint16_t,uint8_t choose);
extern uint16_t Letters_Alignments(uint16_t X_Start,uint16_t X_End,uint8_t *asciichar,uint8_t Type);
//void syr_Calibration_screen();
void split_frame();
void length_option_scroll(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE);
void Digit_Spiliting_L(uint32_t Value);
//extern void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *circle_word,uint8_t circle_pos);
extern void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
//extern void TFT_Number(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color);
extern uint8_t TFT_ST7789V_left_Right_arrow_14_arial(uint16_t xpos, uint16_t ypos,
        uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
void length_option_scroll(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE);
void Left_Right_Indi_L(uint8_t cursor_pos);
void edit_split_frame2(void);
void frame_syr_len_and_plunger_adc_main(uint8_t cursor,uint8_t screen);
void Digit_Spiliting2(uint32_t Value,uint8_t cur_no,uint8_t prev_no);
void edit_option_scroll2(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE);
void accu_calib_setting();
 uint8_t syringe_len_collar_len_main_screen();
void plunger_adc_live_main_screen();
void plunger_adc_screen(uint8_t cursor,uint8_t screen);

uint16_t edit_screen_1(uint16_t *addval,uint8_t cursor,uint16_t limit);
void warning_calibrated_screen( uint8_t cursor);
void syringe_end_check();
void plunger_based_o_ml_max_ml_calculate();

extern void swap_sysize_glow(uint8_t currentOption,uint8_t prev_sysize);
void acc_len_NAV_section();
void SL_EEPROM_read();
volatile float Syringe_length_ADC_differnece;

volatile uint16_t linear_temp_adc=0;
uint16_t SD_temp_adc=0;

uint16_t temp_adc=0;
uint8_t condition_L=1;
uint8_t  array_dig_L[5];
//extern uint16_t xpos;
char letter_L[2]={'\0','\0'} ;
uint8_t L_position_1=0;
extern uint16_t array_dig2[6];
extern char letter1[2];

uint16_t value_l[8]={0,330,380,510,630,0,750}; //Syringe Length
uint16_t value_c[8]={0,140,185,182,185,0,226}; //Collar Length

uint16_t sl_pullout_ADC;
uint16_t sl_pullin_ADC;
extern uint8_t prev_val[8];
// uint8_t prev_sysize;
 extern uint8_t ADC_delay;

 extern uint8_t brand_select_no;
void display_GLOW_SECOND_Options(uint8_t currentOption)
{
	    uint8_t row=0,col=0;
		uint8_t sysize=0;

		uint8_t acutual_size=currentOption;            //detect_syringe_size(2);

		uint32_t   color[2]={GRAY,WHITE};

		uint8_t sel=0;

		char* mat[7]={"789"," 2 mL"," 5 mL","10 mL","20 mL","30 mL","50 mL"};


		  Font_Select(ARIAL_14);

		  sysize=1;
		for( row =0; row<=1; row++ )
		{
			for(col =0; col<=2; col++ )
			{
				if(acutual_size==sysize)
				{
					sel=1;
				}
				else
				{
					sel=0;
				}
			  /* printf("prev_val [%d]=%d",sysize,prev_val[sysize]);
			   printf("prev_val [%d]=%d",sysize,prev_val[sysize]);*/

				  if( prev_val[sysize] !=sel)
				  {
					  TFT_ST7789V__rectangle_curve(25+(95*col),62+(68*row),50,80,0,color[sel]);

					  TFT_String(40+(row*-5)+(95*col),75+(row*70),(uint8_t *)mat[sysize],color[sel],BLACK);
				  }

			  prev_val[sysize]=sel;
			  sysize++;
			}

		}
}
/****************************************************************************
* Function Name : void accu_calib_setting()
* Arguments     : void
* Return type   : void
* Description   : these function controll the sysringe size detect and glow funtions.(ACCURACY CALIBRATION-MAIN FUNCTION)
*****************************************************************************/
void accu_calib_setting()
{
	uint8_t calib_loop=1;
	   uint8_t cursor=0;
	   uint8_t ssize=0;
	   POW.power_status_flag=1;//27-03

	   while(KEY.value);

	   while(calib_loop)  //REW_UP(Switch case)
	   {
		   Power_off();//27-03
		   if(!ON_Flag)
		   {
			   break;//26-03
		   }

		   else if(POW.power_status_flag)//27-03
		   {
			   POW.power_status_flag=0;//27-03
			   frame_syr_len_and_plunger_adc_main(cursor,1);
		   }
		   ssize =detect_syringe_size(1); //REW_UP

		   switch(KEY.value)
		 {
		   case UP_KEY:
	//	   if( (KEY.value== UP_KEY) || (KEY.value==DOWN_KEY) )
	//	   {
			   //   cursor^=1;
			   frame_syr_len_and_plunger_adc_main(cursor,0);
			   while(KEY.value);
			   break;

		   case DOWN_KEY:
		   //	   if( (KEY.value== UP_KEY) || (KEY.value==DOWN_KEY) )
		   //	   {
		   		   //   cursor^=1;
		   		   frame_syr_len_and_plunger_adc_main(cursor,0);
		   		   while(KEY.value);
		   		   break;
		  // }
	//	   else if(KEY.value == BACK_KEY)
	//	   {
		    case BACK_KEY:
//			   beep_sound();
			   calib_loop=0;
			   while(KEY.value);
			   break;

		 //  }

	//	   else if(KEY.value == OK_KEY)
	//	   {
		    case OK_KEY:
			   if(!cursor)
			   {
				   //ssize =6;
				   acc_len_NAV_section();
			   }
			   POW.power_status_flag=1;
			   while(KEY.value);
			   break;

		 //  }
	   }
	}
}

/****************************************************************************
* Function Name : void acc_len_NAV_section()
* Arguments     : void
* Return type   : void
* Description   : these function controll the sringe size detect and glow funtions.
*****************************************************************************/
void acc_len_NAV_section()
{
	uint8_t sy_current_val,nav_loop;
	uint8_t prev_sysize=0;
	//ssize=2;
		POW.power_status_flag=1;//27-03

//	frame_syringe_size_glow(1,1);
//	 while(KEY.value);
	 nav_loop=1;

		while(nav_loop)
		{
			  Power_off();//27-03


				   			if(!ON_Flag)
				   				{
				   					break;//26-03
				   				}

				   				else if(POW.power_status_flag)//27-03

				   			   {

				   					POW.power_status_flag=0;//27-03
				   					frame_syringe_size_glow(1,1);

				   			   }
			  sy_current_val=detect_syringe_size(1);

			if(prev_sysize != sy_current_val)
			{
				swap_sysize_glow(sy_current_val,prev_sysize);
				prev_sysize=sy_current_val;
			}

			if(KEY.value==OK_KEY  && sy_current_val!=0)//26-04
			{
				nav_loop = syringe_len_collar_len_main_screen();
			//  break;
				nav_loop=1;
				frame_syringe_size_glow(1,1);
			//  break;
			}
			else if(KEY.value == BACK_KEY)
			{
				nav_loop=0;
			}
			/*else if(KEY.value == BOLUS_KEY)
			{
				ssize=sy_current_val++;
				if(sy_current_val>6)sy_current_val=0;
				while(KEY.value);
			}*/
		}
}
/****************************************************************************
* Function Name : uint8_t syringe_len_collar_len_main_screen()
* Arguments     : void
* Return type   : void
* Description   : these function controll the syringe length and collar main screen functions.
*****************************************************************************/
uint8_t syringe_len_collar_len_main_screen()
{
   uint8_t back_or_ok=0;
   uint8_t calib_loop=1;
   uint8_t cursor=0;
   uint8_t ssize=0;

   ssize=detect_syringe_size(1);
   syringe_length_and_collar_main_screen(0,1);
   while(KEY.value);
   while(calib_loop)
   {

	   if( (KEY.value== UP_KEY) || (KEY.value==DOWN_KEY) )
	   {
		   cursor^=1;

		   syringe_length_and_collar_main_screen(cursor,0);
		   while(KEY.value);
	   }
	   else if(KEY.value == BACK_KEY)
	   {
		   calib_loop=0;
		   back_or_ok=1;
		   while(KEY.value);
	   }
	   else if(KEY.value == OK_KEY)
	   {

		   if(!cursor)
		   {
			   edit_screen_1(&value_l[ssize],0,1200);
			//   printf("\n ssize = %d ,length edit= %d",ssize,value_l[ssize]);
			   EEPROM_datawrite(EEPROM_SL_LEN_VAL+(brand_select_no)*12+((ssize-1)*2),value_l[ssize], 2);
		   }

		   else
		   {
			   edit_screen_1(&value_c[ssize],1,300);
			//   printf("\n ssize = %d ,collar edit= %d",ssize,value_c[ssize]);
			   EEPROM_datawrite(EEPROM_SL_COL_VAL+(brand_select_no)*12+((ssize-1)*2),value_c[ssize], 2);
		   }

		   Rectangle(0,36,320,204-36,MAIN_FRAME_COL);
		   frame_footer_press_ok_to_edit();

		   syringe_length_and_collar_main_screen(cursor,1);
		   while(KEY.value);
	   }
   }
   return back_or_ok;
}
/****************************************************************************
* Function Name : void plunger_adc_live_main_screen()
* Arguments     : void
* Return type   : void
* Description   : this function display live adc value , pull in and pull out values.

*****************************************************************************/
void plunger_adc_live_main_screen()
{
	   uint8_t calib_loop=1;
	   uint8_t cursor=0;
	   uint8_t plunger_flag=0;
	   uint32_t temp_adc1=1;
//	   plunger_adc_screen(0,1);
	   Font_Select(BEBAS_43);
	   POW.power_status_flag=1;
	   //while(KEY.value);
	   while(calib_loop)
	   {
		   Power_off();//27-03


		   			if(!ON_Flag)
		   				{
		   					break;//26-03
		   				}

		   				else if(POW.power_status_flag)//27-03

		   			   {
		   					POW.power_status_flag=0;//27-03
		   				    plunger_adc_screen(0,1);
		   			   }


		   if(!ADC_delay)
		  		   {
		  			 ADC_delay=1;
		  			 linear_temp_adc = ADC_read(ADC_CH_PLUNGER_POT);
		  		   }

		   if(!plunger_flag)
		   {
		  		   if(temp_adc1 != linear_temp_adc)
		  			 {
		  			   Font_Select(BEBAS_43);
		  			   TFT_Number(85,48,temp_adc1,MAIN_FRAME_COL, MAIN_FRAME_COL);
		  			   TFT_Number(85,48,linear_temp_adc,MAIN_FRAME_COL, WHITE);
		  			   temp_adc1 = linear_temp_adc;
		  			 }
		   }


		  switch(KEY.value)
		  {
		  case UP_KEY:

			// cursor^=1;
			  if(cursor>0)
			   {
				  cursor--;
			   }
			   else
			   {
				  cursor=1;
			   }

			  plunger_adc_screen(cursor,0);

			  while(KEY.value);
			  break;

		  case DOWN_KEY:

			  //cursor^=1;
			  if(cursor<1)
			  {
				  cursor++;
			  }
			  else
			  {
				  cursor=0;
			  }
			  plunger_adc_screen(cursor,0);

			  while(KEY.value);
			  break;

		  case OK_KEY:

			  if(!plunger_flag)
			  {
			  warning_screen_ADC(cursor);
			  plunger_flag=1;
			  }

			  else if (plunger_flag==1)
			  {

					  plunger_flag=0;
					  warning_calibrated_screen(cursor);
//
//					if(cursor)
//					{
//
//					   EEPROM_datawrite(EEPROM_SL_PULL_IN,sl_pullin_ADC,2);
//					}
////
//					else
//					{
////
//					   EEPROM_datawrite(EEPROM_SL_PULL_OUT,sl_pullout_ADC,2);
////
//					}
					   delay_us(1000);
					   POW.power_status_flag=1;
					   cursor=0;
			  }

//			   while(KEY.value);
//				   //while(1)
//				   //{
//
//						   if(KEY.value==OK_KEY)
//						   {
//							   warning_calibrated_screen(cursor);
//								if(cursor)
//								{
//
//								   EEPROM_datawrite(EEPROM_SL_PULL_IN,sl_pullin_ADC,2);
//								}
//								else
//								{
//
//								   EEPROM_datawrite(EEPROM_SL_PULL_OUT,sl_pullout_ADC,2);
//
//								}
//							   while(KEY.value);
//							   delay_us(1000);
//							   plunger_adc_screen(cursor,1);
						//   }
				  // }

			  while(KEY.value);
			  break;

		  case BACK_KEY:

			   calib_loop=0;

		 	  while(KEY.value);
			  break;
		  }


	   }

}


void frame_footer_ok_conform()
{
//	 // Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Confirm");
//	   Rectangle(0,206,320,34,NEW_FTR_COL);
//	   Font_Select(ARIAL_14);
//	   TFT_String(140,210,(uint8_t *)"Confirm",NEW_FTR_COL,BLACK);  // "name" "1235"
//
//	    TFT_ST7789V_lcd_drawcircle1(150-30,221,15,MAIN_FRAME_COL,MAIN_FRAME_COL);
//	 	Font_Select(ARIAL_9);
//	 	TFT_String(141-30,216,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);
//	 	// Frame_Footer END
}

void frame_footer_press_ok_to_conform()
{
	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
	CENTER_SCR(MAIN_FRAME_COL);  //center green
}




void frame_footer_press_ok_to_edit()
{
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"SL Calibration");
   	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
    	//CENTRE GREEN
   	CENTER_SCR(MAIN_FRAME_COL);  //center green
    border_lines();

   	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
   	Font_Select(ARIAL_BOLD_14);
   	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

}


uint16_t syringe_length_edit(uint16_t length,uint8_t choose)
{
	     uint8_t Cursor_no=1;
	     uint8_t prev_cursor_no=0;
	     uint8_t space=0;
	     uint16_t xpos=0;

	     Rectangle(0,0,320,240,MAIN_FRAME_COL);
	     Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Accuracy Calibration");
	     frame_footer_press_ok_to_conform();

	     Font_Select(ARIAL_12);
	     if(choose)
	     {
	       TFT_String(10,44-2,(uint8_t*)"Collar Length",MAIN_FRAME_COL,WHITE);
	     }
	     else
	     {
	       TFT_String(10,44-2,(uint8_t*)"Syringe Length",MAIN_FRAME_COL,WHITE);
	     }

	     //
	     TFT_String(260+9+5,44-2,(uint8_t*)"mm",MAIN_FRAME_COL,WHITE);

	     TFT_ST7789V_lcd_drawcircle1(18,128,12,WHITE,WHITE);           //CIRCLE
	     TFT_ST7789V_lcd_drawcircle1(297,127,12,WHITE,WHITE);
	     TFT_ST7789V_left_Right_arrow_14_arial(121,13,0,WHITE,NEON_BLUE_1);
	     TFT_ST7789V_left_Right_arrow_14_arial(120,295,1,WHITE,NEON_BLUE_1);

	      split_frame();

	      Digit_Spiliting_L(length);

	     length_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
	     prev_cursor_no=1;
	     while(condition_L)
	          {
	    	    condition_L=0;
	         	 switch(KEY.value)
	         	{
	         	 case LEFT_KEY:
	         		 if(Cursor_no<4)
	         	  {
	         		Cursor_no++;
	         		length_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
	       		    if(Cursor_no !=2 && Cursor_no !=3 )
	         		{
	         		//Left_Right_Indi(Cursor_no);
	         	    }
	         	  }
	         		 prev_cursor_no=Cursor_no;
	         		 while(KEY.value);
	         		 break;

	         	 case RIGHT_KEY:
	         		if(Cursor_no>0)
	         	   {
	         		Cursor_no--;
	         		length_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
	         		 if(Cursor_no !=1 && Cursor_no !=2 )
	         		 {
	         		   // Left_Right_Indi(Cursor_no);
	         		 }
	         	   }
	        		prev_cursor_no=Cursor_no;
	        		while(KEY.value);
	         		break;

	         	 case DOWN_KEY:
	         		 if( array_dig_L[Cursor_no]<9)
	         	 {
	         		   array_dig_L[Cursor_no]++;
	         		  if(Cursor_no==0)
	         		  {
	         			space=15;
	         		    Font_Select(BEBAS_43);
	         		   xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
	         		  									      81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);
	                    TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],CURSOR_COL,BLACK);
	                  }

	                  else
	         		  {
	           			space=0;
	           		    Font_Select(BEBAS_43);
	           		    xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
	           		 		    		  	  	81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);
	           		    TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],CURSOR_COL,BLACK);
	                  }
	         	 }

	         		 while(KEY.value);
	         		 break;

	         	 case UP_KEY:
	                  if( array_dig_L[Cursor_no]>0)
	                  {
	                   array_dig_L[Cursor_no]--;
	                  if(Cursor_no==0)
	                 {
	           			space=15;
	                  Font_Select(BEBAS_43);
	                  xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
	             	         		  			81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);
	             	 TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],CURSOR_COL,BLACK);
	                 }

	                  else
	                 {
	             		space=0;
	                	Font_Select(BEBAS_43);
	                	xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
	                	           		 		81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);
	                	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],CURSOR_COL,BLACK);
	                 }
	             }
	     			 while(KEY.value);
	     			 break;

	         	 case OK_KEY:

	         		length= array_dig_L[4]*10000+ array_dig_L[3]*1000+ array_dig_L[2]*100+ array_dig_L[1]*10+ array_dig_L[0]*1;
	            	Font_Select(ARIAL_12);
	            	TFT_Number(30,40,length,MAIN_FRAME_COL,BLACK);
	            	condition_L=0;

	            	break;

	             }
	         //	condition_L=0;
	           }
	     return length;

}


/*
void frame_syringe_size_glow()
{
	uint8_t a=0;
	TFT_ST7789V__rectangle_curve(62,20,80,50,0,WHITE); // cursor box // caution window -- Y   X 1
	 TFT_ST7789V__rectangle_curve(130,20,80,50,0,WHITE); // cursor box // caution window -- Y   X 1

	 TFT_ST7789V__rectangle_curve(62,120,80,50,0,WHITE); // cursor box // caution window -- Y   X 1
	 TFT_ST7789V__rectangle_curve(130,120,80,50,0,WHITE); // cursor box // caution window -- Y   X 1

	 TFT_ST7789V__rectangle_curve(62,220,80,50,0,WHITE); // cursor box // caution window -- Y   X 1
	 TFT_ST7789V__rectangle_curve(130,220,80,50,0,WHITE); // cursor box // caution window -- Y   X 1

    Rectangle(220,130,80,50,WHITE); // -- Y   X6
}*/
void split_frame()
{
	 uint8_t loop_count=0;

	 for(loop_count=1;loop_count<=3;loop_count++)
		 {
		     Rectangle(35+(45*loop_count),68,0,115,BLACK);
	     }
	    Rectangle(35+(45*3)+15,68,0,115,BLACK);
	 	Rectangle(35+(45*4)+15,68,0,115,BLACK);
	 	Font_Select(BEBAS_43);
	    TFT_String(123-10,(45*5)-5,(uint8_t*)".",MAIN_FRAME_COL,WHITE);
}



void Digit_Spiliting_L(uint32_t Value)
 {
	uint8_t space=0;
	uint8_t loop_count=0;
	uint16_t xpos=0;
	L_position_1=0;


	 while(Value>0)
	     {
	     array_dig_L[L_position_1]=Value%10;
	     Value=Value/10;
	     L_position_1++;
	     }

	for(loop_count=0;loop_count<L_position_1;loop_count++)
     {
		if(loop_count==0)
		{
			space=15;
		}
		else
		{
			space=0;
		}

      Font_Select(BEBAS_43);

      letter_L[0] =	 array_dig_L[loop_count]+48;

      xpos=Letters_Alignments(36+(45*(4-loop_count))+space,
    		  	  	  	  	  	  	  	  	  81+(45*(4-loop_count))+space, (uint8_t*)letter_L,0);

     //  Font_Select(BEBAS_43);
     if(loop_count !=0 && loop_count !=1 )
      {
       TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[loop_count],MAIN_FRAME_COL,WHITE);
      }
    }
  }


void length_option_scroll(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
	uint32_t back_colour   = 0;
	uint32_t text_colour		= 0;
	uint8_t loop		        	= 0;
	uint8_t Cursor_no		= 0;
	uint8_t space				= 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint16_t xpos=0;

	for(loop = 0; loop<2; loop++)
	 	{
	 		if(!loop)
	 		{
	 			back_colour			= MAIN_FRAME_COL;
	 			text_colour 			= WHITE;
	 			Cursor_no			= prev_cursor_no;
                arrow_bg_color	= MAIN_FRAME_COL;
                arrow_fill_color	= MAIN_FRAME_COL;
	 		}
	 		else
	 		{
	 			back_colour 		= LIGHT_GRAY;
	 			text_colour  			= BLACK;
	 			Cursor_no			= current_cur_no;
	 			arrow_bg_color	= LIGHT_GRAY;
				arrow_fill_color	= BLACK;
	 		}

 if(Cursor_no==0)
 {
	 space=15;
	TFT_ST7789V__rectangle_curve(231,66,125,43,0,back_colour);
	Font_Select(BEBAS_43);
	xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
									      81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);
	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],back_colour,text_colour);
	TFT_ST7789V_up_down_arrow_20_arial(79,243,0,arrow_bg_color,arrow_fill_color);    //UP_ARROW
    TFT_ST7789V_up_down_arrow_20_arial(166,243,1,arrow_bg_color,arrow_fill_color);  //UP_ARROW
 }

	else
  {
    space=0;
	TFT_ST7789V__rectangle_curve(66,36+(45*(4-Cursor_no)),43,125,0,back_colour);
	Font_Select(BEBAS_43);
	xpos=Letters_Alignments(36+(45*(4-Cursor_no))+space,
		    		  	  	  	  	  	  	  	  	  81+(45*(4-Cursor_no))+space, (uint8_t*)letter_L,0);

	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig_L[Cursor_no],back_colour,text_colour);
    TFT_ST7789V_up_down_arrow_20_arial(79,50+(45*(4-Cursor_no)),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
    TFT_ST7789V_up_down_arrow_20_arial(166,50+(45*(4-Cursor_no)), 1,arrow_bg_color,arrow_fill_color)	;	//DOWN_ARROW
  }
}
}

void Left_Right_Indi_L(uint8_t cursor_pos)
{
	uint32_t Right_bg_color;
	uint32_t left_bg_color;

	if(cursor_pos==0 )
	{
    Right_bg_color	=	LIGHT_GRAY_3;
    left_bg_color		=	WHITE;
	}

	else if(cursor_pos==4)
	{
     left_bg_color=		LIGHT_GRAY_3;
	}

	else
	{
	Right_bg_color	=WHITE;
	left_bg_color   =WHITE;
	}

    TFT_ST7789V_lcd_drawcircle1(128,18,12,left_bg_color,left_bg_color);           				//	 Left Circle
    TFT_ST7789V_lcd_drawcircle1(127,297,12,Right_bg_color,Right_bg_color);   			// Right Circle

    TFT_ST7789V_left_Right_arrow_14_arial(121,13,0,left_bg_color,NEON_BLUE_1);        //	Left Arrow
    TFT_ST7789V_left_Right_arrow_14_arial(120,295,1,Right_bg_color,NEON_BLUE_1);  //	Right Arrow

}
/****************************************************************************
* Function Name : uint16_t edit_screen_1(uint16_t raw_data_20psi,uint8_t cursor)
* Arguments     : raw_data_20psi,cursor
* Return type   : leng_val
* Description   : this function used to edit coller length and syringe length

*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t edit_screen_1(uint16_t *addval,uint8_t cursor,uint16_t limit)              //local variable
{


	char letter1[2]={'\0','\0'} ;
	uint16_t raw_data_20psi;
	uint8_t Cursor_no=3;
	uint8_t prev_cursor_no=2;
	uint8_t space=0;
	uint16_t leng_val=0;
	uint16_t xpos=0;
	//uint16_t temp_psi=0;
//  uint16_t __1_psi=0;
	uint8_t condition=1;
	//uint8_t ypos1=0;

	         raw_data_20psi = *addval;

	         Rectangle(0,0,320,240,MAIN_FRAME_COL);
		     Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe Length");
		     frame_footer_press_ok_to_conform();

		     Font_Select(ARIAL_12);
		     if(cursor)
		     {
//		       TFT_String(10,44-2,(uint8_t*)"Collar Length",MAIN_FRAME_COL,WHITE);
		     }

		     else
		     {
//		       TFT_String(10,44-2,(uint8_t*)"Syringe Length",MAIN_FRAME_COL,WHITE);
		     }

//		     TFT_String(260+9+5,44-2,(uint8_t*)"mm",MAIN_FRAME_COL,WHITE);


// 	TFT_ST7789V_lcd_drawcircle1(31,128,12,WHITE,WHITE);           //arrow CIRCLE
//	TFT_ST7789V_lcd_drawcircle1(289,127,12,WHITE,WHITE);
//	TFT_ST7789V_left_Right_arrow_14_arial(121,26,0,WHITE,NEON_BLUE_1);
//	TFT_ST7789V_left_Right_arrow_14_arial(120,287,1,WHITE,NEON_BLUE_1);

	edit_split_frame2();
	Left_Right_Indi(Cursor_no,0);
	Digit_Spiliting2(raw_data_20psi,Cursor_no,prev_cursor_no);
	edit_option_scroll2(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);

	prev_cursor_no=3;
	 while(KEY.value);
	while(condition)
	{
		switch(KEY.value)
		{
			case LEFT_KEY:// LEFT_KEY:
			if(Cursor_no<3)
			{
				Cursor_no++;
				edit_option_scroll2(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
				if(Cursor_no !=2 && Cursor_no !=0 )
				{
						Left_Right_Indi(Cursor_no,0);
				}
			}
			prev_cursor_no=Cursor_no;
			while(KEY.value);
			break;

			case RIGHT_KEY://RIGHT_KEY:
			if(Cursor_no>0)
			{
				Cursor_no--;
				edit_option_scroll2(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
				if(Cursor_no !=1 && Cursor_no !=3 )
				{
					   Left_Right_Indi(Cursor_no,0);
				}
			}
			prev_cursor_no=Cursor_no;
			while(KEY.value);
			break;

			case UP_KEY:
			if(array_dig2[Cursor_no]<9)
			{
				if(Cursor_no<1)
								{
									space=15;

								}
								else
								{
									space=0;

								}
				Font_Select(BEBAS_43);
				letter1[0]=array_dig2[Cursor_no]+48;
				xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
				106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,CURSOR_COL);

				array_dig2[Cursor_no]++;
				if(array_dig2[3]*1000+array_dig2[2]*100+array_dig2[1]*10+array_dig2[0]*1>limit)
				{
					array_dig2[Cursor_no]--;
				}

				Font_Select(BEBAS_43);
				letter1[0]=array_dig2[Cursor_no]+48;
				xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
				106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);
			}

			while(KEY.value);
			break;

			case DOWN_KEY:
			if(array_dig2[Cursor_no]>0)
			{
				if(Cursor_no<1)
								{
									space=15;

								}

								else
								{
									space=0;

								}

				Font_Select(BEBAS_43);
				letter1[0]=array_dig2[Cursor_no]+48;
				xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
				106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,CURSOR_COL);

				array_dig2[Cursor_no]--;

				Font_Select(BEBAS_43);
				letter1[0]=array_dig2[Cursor_no]+48;
				xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
				106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);
			}
			while(KEY.value);
			break;

			case OK_KEY:

			leng_val=array_dig2[3]*1000+array_dig2[2]*100+array_dig2[1]*10+array_dig2[0]*1;
			//TFT_Number(30,40,leng_val,MAIN_FRAME_COL,BLACK);
			//while(KEY.value);
//			Font_Select(ARIAL_9);
//			TFT_Number(30,40,ten_psi,MAIN_FRAME_COL,BLACK);


//			temp_psi=(raw_data_20psi-G_zero_psi_data);
			//__1_psi=(temp_psi*100)/leng_val;

			//TFT_Number(30,40,leng_val,MAIN_FRAME_COL,BLACK);
			//while(1);
			condition=0;
			// while(KEY.value);
			break;

			case BACK_KEY:
				 leng_val=array_dig2[3]*1000+array_dig2[2]*100+array_dig2[1]*10+array_dig2[0]*1;
				condition=0;
				//while(KEY.value);
				break;



		}
	}
	*addval=leng_val;
	/*if(!cursor)
	{
		value_l[detect_syringe_size(1)] = leng_val;
	}

	else
	{
	    value_c[detect_syringe_size(1)] = leng_val;
	}
//	TFT_Number(30,40,value_l[ssize],MAIN_FRAME_COL,BLACK);*/
//	while(1);
	//return __1_psi;
	return leng_val;
}
void edit_split_frame2(void)
{
	uint8_t loop_count=0;
	for(loop_count=0;loop_count<=3;loop_count++)
	{
		Rectangle(62+(45*loop_count),68,0,115,WHITE);
	}
	//   Rectangle(40+(45*4)+15,68,0,115,WHITE);
	for(loop_count=3;loop_count<5;loop_count++)
	{
		Rectangle(62+15+(45*loop_count),68,0,115,WHITE);
	}
	Font_Select(BEBAS_43);
	TFT_String(202,119,(uint8_t*)".",MAIN_FRAME_COL,WHITE);
}
void Digit_Spiliting2(uint32_t Value,uint8_t cur_no,uint8_t prev_no)
 {
	uint8_t space=0;
	uint8_t loop_count=0;
    uint8_t position_2=0;
    uint16_t xpos=0;
    char letter1[2]={'\0','\0'} ;

 //   uint16_t ypos1=0;
    if(!Value)
    {
    	for(position_2=0;position_2<3;position_2++)
    	{
    		 array_dig2[position_2]=0;
    	}
    }
    else
    {
	 while(position_2<4)
		 {
			 array_dig2[position_2]=Value%10;
			 Value=Value/10;
			 position_2++;
	     }
    }

	for(loop_count=0;loop_count<position_2;loop_count++)
     {
		if(loop_count==0)//|| loop_count==1)
		{
			space=15;
		}
		else
		{
			space=0;
		}

      Font_Select(BEBAS_43);

      letter1[0] =	 array_dig2[loop_count]+48;

      xpos=Letters_Alignments(63+(45*(3-loop_count))+space,
    		  	  	  	  	  	  	  	  	  106+(45*(3-loop_count))+space, (uint8_t*)letter1,0);

       Font_Select(BEBAS_43);
     if(loop_count !=cur_no && loop_count !=prev_no )
      {
//       TFT_Number(98,ypos1,array_dig2[loop_count],MAIN_FRAME_COL,WHITE);

       TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[loop_count],MAIN_FRAME_COL,WHITE);
      }
    }
  }
void edit_option_scroll2(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
	uint32_t back_colour   = 0;
	uint32_t text_colour		= 0;
	uint8_t loop		        	= 0;
	uint8_t Cursor_no		= 0;
	uint8_t space				= 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint16_t xpos=0;
	//uint8_t ypos1=0;
	char letter1[2]={'\0','\0'} ;
	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour			= MAIN_FRAME_COL;
			text_colour 		= WHITE;
			Cursor_no			= prev_cursor_no;
			arrow_bg_color		= MAIN_FRAME_COL;
			arrow_fill_color	= MAIN_FRAME_COL;
		}
		else
		{
			back_colour 		= LIGHT_GRAY;
			text_colour  		= BLACK;
			Cursor_no			= current_cur_no;
			arrow_bg_color		= LIGHT_GRAY;
			arrow_fill_color	= BLACK;
		}

		if(Cursor_no<1)
		{
			space=15;
		}
		else
		{
			space=0;
		}

		TFT_ST7789V__rectangle_curve(1+62+(45*(3-Cursor_no))+space,64,125,43,0,back_colour);

		Font_Select(BEBAS_43);
		letter1[0]=array_dig2[Cursor_no]+48;
		xpos=Letters_Alignments(62+(45*(3-Cursor_no))+space,
		106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);

		TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],back_colour,text_colour);

		TFT_ST7789V_up_down_arrow_20_arial(79,15+62+(45*(3-Cursor_no))+space,0,
		arrow_bg_color,arrow_fill_color);    //UP_ARROW
		TFT_ST7789V_up_down_arrow_20_arial(166,15+62+(45*(3-Cursor_no))+space,1,
		arrow_bg_color,arrow_fill_color);  //UP_ARROW

	}
}


/****************************************************************************
* Function Name : void plunger_based_o_ml_max_ml_calculate()
* Arguments     : void
* Return type   : void
* Description   : this function calculate syringe maximum position adc & syringe end position adc for all syringe

*****************************************************************************/
void plunger_based_o_ml_max_ml_calculate()
{

	 float syringe_length[7];
	 float collar_length[7];
	 float full_plunger_out;
	 float full_plunger_in;
	 float Full_Plunger_length;
	 float  Syringe_length_ADC_differnece;
	 float one_mm_ADC;
	 float syringe_end;
	 float syr_max_ml_adc;

//		uint16_t temp;
	adc_rd=ADC_read(ADC_CH_PLUNGER_POT);

	float syr_default_value[7]={0.0,2.0,5.0,10.0,20.0,30.0,50.0};
//		volatile float zerp_ml_ADC;
//		volatile float one_ml;
	uint8_t ssize=0;

	ssize=detect_syringe_size(1);
	syringe_length[ssize]=value_l[ssize]/10.0;  //75.0; // read from eeprom
	collar_length[ssize]=value_c[ssize]/10.0;   //22.6;  // read from eeprom

		Full_Plunger_length=110.0;     // read from eeprom
		full_plunger_out =(float)sl_pullout_ADC;//5000.0;//Got the value from the SL ADC values
		full_plunger_in  =(float)sl_pullin_ADC;//45000.0;//Got the value from the SL ADC values

		one_mm_ADC = (float)((full_plunger_out-full_plunger_in)/Full_Plunger_length);
		Syringe_length_ADC_differnece = syringe_length[ssize]*one_mm_ADC;

	    if(14.6 <collar_length[ssize])
	    {
	    	syringe_end = (collar_length [ssize]-6.6-8.0) + ( (full_plunger_in*120.0)/65535.0);
	    }
	    else
	    {
	    	syringe_end = (collar_length [ssize]-collar_length [ssize]) + ( (full_plunger_in*120.0)/65535.0);
	    }

   __0_ml_ADC = (syringe_end*one_mm_ADC);
	syr_max_ml_adc =  (__0_ml_ADC + Syringe_length_ADC_differnece);



	LIN_POT.syr_max_pos[ssize] =(unsigned int) syr_max_ml_adc;
	LIN_POT.syr_end_pos[ssize] =(unsigned int) __0_ml_ADC;

//		__1_ml_ADC=((adc_rd - __0_ml_ADC)/syr_default_value[ssize]);
	__1_ml_ADC=((LIN_POT.syr_max_pos[ssize] - __0_ml_ADC)/syr_default_value[ssize]);


}

void SL_EEPROM_read()
{
//	//uint8_t loop_count=0;
//
//	 for(uint8_t loop=1;loop<7;loop++)
//		{
//		   value_l[loop]= EEPROM_dataread(EEPROM_SL_LEN_VAL+(brand_select_no*12)+((loop-1)*2), 2);
//		}
//		for(uint8_t loop=1;loop<7;loop++)
//		{
//		   value_c[loop]= EEPROM_dataread(EEPROM_SL_COL_VAL+(brand_select_no*12)+((loop-1)*2), 2);
//		}
}
