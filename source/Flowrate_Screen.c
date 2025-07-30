#include "LPC55S16.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "Flowrate_Screen.h"
#include "Power_off.h"
#include "FR_CALC.h"
#include "Dynamic_Pressure.h"
#include "Dosage_mode.h"
#include "Driver_Timer.h"
#include "Bolus_Header.h"
#include "Alarm.h"
#include "Motor.h"
#include "SD_Calib.h"
#include "Dosage_mode.h"
#include "sound.h"
#include "STEPPER.h"
#include "Main_screen.h"
#include  "Drug_List.h"

uint8_t condition=1;
//uint8_t add_flag=0;
//uint8_t position_1=0;
uint8_t array_dig1[5];
//extern uint8_t power_flag;
//uint16_t con_value=0;
uint32_t G_con_value=0;
//uint16_t xpos=0;
uint8_t G_fr_wrn_flag;
uint8_t G_fr_wrn_flag_1;

typedef struct
{
    char* headerText;
    char* headerText_1;
    char* footerText;
} ScreenConfig;

extern uint8_t auto_lock_time;//common_bug  //ZOOZ_1

extern uint32_t G_KVO_fr;
extern uint8_t G_opt_no;
extern uint8_t G_page_no;
extern uint8_t weight_opt_no;
extern uint8_t w_unit;
extern uint8_t drug_no;
//extern uint8_t G_infusion_modes;
extern char dose_rate_unit[3][13];
extern uint8_t dose_option_no;
extern char* disp_para[20];
extern float time_value[4];
extern  char* drug_unit[4][4];
extern volatile uint8_t normal_run;
extern uint8_t VTBI_check(uint8_t v_mode);
extern void dynamic_pressure(uint8_t dy_xpos,uint8_t dy_ypos,uint8_t pressure,uint8_t screen); //curved disp
extern uint8_t ON_Flag;
//extern uint8_t G_infusion_mode_opt;
extern uint8_t D_xpos;
extern uint8_t DF_xpos;//24-04
uint8_t Prev_DF_xpos=0;
extern void Notification(uint8_t notif);
extern uint8_t Bol_fr_flag;
extern uint16_t press_run_alarm;
extern void plunger_based_o_ml_max_ml_calculate();
extern float syringe_nearempty_calculation(uint16_t flowarte);
extern void event_no_update(uint8_t event_no);
//extern uint8_t G_pre_sy_size_1[1];
extern void syringe_disp_status(uint8_t screen);
extern uint8_t G_pre_sy_size_1[1];
extern uint8_t sy_size;
extern uint8_t Bolus_rate_flag;
extern uint16_t Pump_idle;
extern uint8_t G_infusion_modes;
extern uint8_t G_mode_switch_flag;
extern uint8_t home_flag;
extern uint8_t right_14(uint16_t xpos, uint16_t ypos,    //22-03
	       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);

void Screen_frame(ScreenConfig* screen,uint32_t conValue)
{
	//if(conValue)
	{
    Frame_Header(NEW_HDR_COL, HDR_TXT_COL, (uint8_t*)screen->headerText);
    CENTER_SCR(MAIN_FRAME_COL);  //center green
    Font_Select(ARIAL_12);
    TFT_String(260, 44, (uint8_t*)screen->headerText_1, MAIN_FRAME_COL, WHITE);
//    Frame_Footer(NEW_FTR_COL, HDR_TXT_COL, (uint8_t*)screen->footerText, (uint8_t*)"OK", 1,14);
	}

}






void FR_split_frame()
{
	 uint8_t loop_count=0;


	 for(loop_count=0;loop_count<=4;loop_count++)
		 {
		     Rectangle(40+(45*loop_count),70,0,115,BLACK);
	     }
	  Rectangle(40+(45*4)+15,70,0,115,BLACK);
	  Rectangle(40+(45*5)+15,68,0,115,BLACK);
	  Font_Select(BEBAS_43);
	  TFT_String(224,100+10,(uint8_t*)".",MAIN_FRAME_COL,WHITE);

	  TFT_ST7789V_lcd_drawcircle1(20,130,12,WHITE,WHITE);           						//	 Left Circle
	  TFT_ST7789V_lcd_drawcircle1(300,130,12,WHITE,WHITE);   //23-03					// Right Circle

	  TFT_ST7789V_left_Right_arrow_14_arial(123,20-5,0,WHITE,NEON_BLUE_1);        	//	Left Arrow
	  TFT_ST7789V_left_Right_arrow_14_arial(123,303-5,1,WHITE,NEON_BLUE_1);  	//	Right Arrow

}

void Digit_Spiliting(uint32_t Value)
 {
	char letter[2]={'\0','\0'} ;
	uint8_t space=0;
	uint8_t loop_count=0;
	uint8_t position_1=0;
	position_1=0;
	uint16_t xpos=0;

    letter[0]='0';

    if(Value==0)
    {
    	for(loop_count=0;loop_count<5;loop_count++)
    	{
    		array_dig1[position_1]=0;
    		position_1++;
    	}
    }

    else
    {
	 while(Value>0)
	     {
	     array_dig1[position_1]=Value%10;
	     Value=Value/10;
	     position_1++;
	     }
    }

	for(loop_count=0;loop_count<position_1;loop_count++)
     {
//		if(loop_count==0)
//		{
//			space=15;
//		}
//		else
//		{
//			space=0;
//		}

      Font_Select(BEBAS_43);

      letter[0] =	 array_dig1[loop_count]+48;

      xpos=Letters_Alignments(40+(45*(4-loop_count)),
    		  	  	40+(45*(5-loop_count)), (uint8_t*)letter,0);

     Font_Select(BEBAS_43);
	if(loop_count>1)  		//REW1
      {
       TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[loop_count],MAIN_FRAME_COL,WHITE);
      }
    }

    letter[0] =	 0+48;


//		Font_Select(BEBAS_60);
	//	xpos=Letters_Alignments(36+(45*(4-4))+0,
	//								 36+(45*(5-4))+0, (uint8_t*)letter,0);

	//	TFT_Number(xpos,98,0,WHITE,BLACK);


  }


void flowrate_option_frame(uint8_t current_cur_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL)
	{
	char letter[2]={'\0','\0'} ;
	uint32_t back_colour   = 0;
	uint32_t text_colour   = 0;
	uint8_t   loop		   = 0;
	uint8_t Cursor_no	   = 0;
	uint8_t space		   = 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint32_t con_value=0;
	uint8_t position_1=0;
	uint8_t fr_pos=0;
	uint16_t xpos=0;
	con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;

	while(con_value>0)
		     {
		     array_dig1[position_1]=con_value%10;
		     con_value=con_value/10;
		     position_1++;
		     }
	if(position_1 < 2)
	{
		position_1=2;
	}
	if(current_cur_no >= position_1)
	{
		fr_pos=current_cur_no;
	}
	else
	{
		fr_pos=position_1-1;
	}
	for(loop = 0; loop<=fr_pos; loop++)
	 	{
	 		if(loop!=current_cur_no)
	 		{
	 			back_colour		 = MAIN_FRAME_COL;
	 			text_colour 	 = WHITE;
	 		//	Cursor_no		 = prev_cursor_no;
                arrow_bg_color	 = MAIN_FRAME_COL;
                arrow_fill_color = MAIN_FRAME_COL;
	 		}
	 		else
	 		{
	 			back_colour 		= LIGHT_GRAY;
	 			text_colour  		= BLACK;
	 			Cursor_no			= current_cur_no;
	 			arrow_bg_color	    = LIGHT_GRAY;
				arrow_fill_color	= BLACK;
	 		}

 if(loop==0)
 {
	space=15;
 }
 else
 {
	space=0;
 }



 	TFT_ST7789V__rectangle_curve(40+1+(45*(4-loop)+space),64+10,125-20,43,0,back_colour);
 	//while(1);
 	Font_Select(BEBAS_43);
 	letter[0]=array_dig1[loop]+48;
 	xpos=Letters_Alignments(40+(45*(4-loop))+space,
 		    		  	  	  	  	  	  	  	  	  85+(45*(4-loop))+space, (uint8_t*)letter,0);

 	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[loop],back_colour,text_colour);
 	if(!loop)
 	{
 	TFT_ST7789V_up_down_arrow_20_arial(79,40+15+(45*(4-Cursor_no)+space),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
 	TFT_ST7789V_up_down_arrow_20_arial(166,40+15+(45*(4-Cursor_no)+space), 1,arrow_bg_color,arrow_fill_color)	;
 	}
// 	else
// 	{
// 		TFT_ST7789V_up_down_arrow_20_arial(79,40+15+(45*(4-Cursor_no)+space),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
// 		 	TFT_ST7789V_up_down_arrow_20_arial(166,40+15+(45*(4-Cursor_no)+space), 1,arrow_bg_color,arrow_fill_color)	;
// 	}

//	TFT_ST7789V__rectangle_curve_1(231,66,125,43,0,back_colour);
//	Font_Select(BEBAS_60);
//    xpos=Letters_Alignments(40+(45*(4-loop))+space,
//									      85+(45*(4-loop))+space, (uint8_t*)letter,0);
//	TFT_Number(xpos,98+2,array_dig1[loop],back_colour,text_colour);
//	TFT_ST7789V_up_down_arrow_20_arial(81,243,0,arrow_bg_color,arrow_fill_color);   //UP_ARROW
//    TFT_ST7789V_up_down_arrow_20_arial(168,243,1,arrow_bg_color,arrow_fill_color);  //down_ARROW//273
//  }
//
//	else
//  {
//    space=0;
//
//    TFT_ST7789V_up_down_arrow_20_arial(81,50+(45*(4-loop)),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
//    TFT_ST7789V_up_down_arrow_20_arial(168,50+(45*(4-loop)), 1,arrow_bg_color,arrow_fill_color)	;	//DOWN_ARROW
//  }
	 	}
	}


void flowrate_option_scroll(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
	{
	char letter[2]={'\0','\0'} ;
	uint32_t back_colour   = 0;
	uint32_t text_colour   = 0;
	uint8_t   loop		   = 0;
	uint8_t Cursor_no	   = 0;
	uint8_t space		   = 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint16_t xpos=0;

	for(loop = 0; loop<2; loop++)
	 	{
	 		if(!loop)
	 		{
	 			back_colour		 = MAIN_FRAME_COL;
	 			text_colour 	 = WHITE;
	 			Cursor_no		 = prev_cursor_no;
                arrow_bg_color	 = MAIN_FRAME_COL;
                arrow_fill_color = MAIN_FRAME_COL;
	 		}
	 		else
	 		{
	 			back_colour 		= LIGHT_GRAY;
	 			text_colour  		= BLACK;
	 			Cursor_no			= current_cur_no;
	 			arrow_bg_color	    = LIGHT_GRAY;
				arrow_fill_color	= BLACK;
	 		}

 if(Cursor_no==0)
 {
	space=15;
 }
 else
 {


	 space=0;
 }
 	TFT_ST7789V__rectangle_curve(40+1+(45*(4-Cursor_no)+space),64+10,125-20,43,0,back_colour);
	Font_Select(BEBAS_43);

	letter[0]=array_dig1[Cursor_no]+48;
    xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
									      85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],back_colour,text_colour);
	TFT_ST7789V_up_down_arrow_20_arial(79,40+15+(45*(4-Cursor_no)+space),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
    TFT_ST7789V_up_down_arrow_20_arial(166,40+15+(45*(4-Cursor_no)+space), 1,arrow_bg_color,arrow_fill_color)	;

//  }
//
//	else
//  {
//    space=0;
//	TFT_ST7789V__rectangle_curve(40+1+(45*(4-Cursor_no)),66,125,43,0,back_colour);
//	//while(1);
//	Font_Select(BEBAS_60);
//	xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
//		    		  	  	  	  	  	  	  	  	  85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
//
//	TFT_Number(xpos,98+2,array_dig1[Cursor_no],back_colour,text_colour);
//    TFT_ST7789V_up_down_arrow_20_arial(81,40+15+(45*(4-Cursor_no)),0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
//    TFT_ST7789V_up_down_arrow_20_arial(168,40+15+(45*(4-Cursor_no)), 1,arrow_bg_color,arrow_fill_color)	;	//DOWN_ARROW
//  }
	 	}
	}



uint16_t Flow_rate_screen(uint32_t *flow_rate ,uint8_t Min_Limit,uint32_t Max_Limit,uint8_t screen)
{
	 char letter[2]={'\0','\0'} ;
		 uint16_t syr_limit[7]={0,1800,3500,5000,9000,12000,12000};
	     uint8_t Cursor_no=1;
	     uint8_t prev_cursor_no=0;
	     uint8_t space=0;
	     uint8_t loop_count=0;
	     uint8_t check_flag=0;
	     uint32_t con_value=0;
	     uint8_t condition=1;
	     uint8_t Err_flag=0;
	     uint8_t fr_flag=0;
	     uint16_t xpos=0;
	     uint32_t P_value=*flow_rate;      //That poiter variable store to the local var

	     auto_lock_time=AUTO_EXIT_TIME;

	     ScreenConfig screens[] =
	         	 {
	                {"Volume", "ml", "        Confirm"},
	                {"Rate", "ml/h", "        Confirm"},
	                {"Bolus Volume", "ml", "        Start"},
					{"Rate","ml/h", "        Start Infusion"}
	         	 };

	     con_value=P_value;
	     Pump_idle=10;
		 fr_flag=1;
		 home_flag=0;
		 POW.power_status_flag=1;
		 memset(array_dig1, 0, sizeof(array_dig1));
		 prev_cursor_no=1;
	//	if(P_value)
	//	{
	//		press_run_alarm=10;
	//	}


	     while(KEY.value);
	     while(condition)
	     {

	     Power_off();

	     if((!ON_Flag)||(!auto_lock_time))
		{
			break;//26-03
		}

		else if(POW.power_status_flag)
		{

			POW.power_status_flag=0;//26-03
		    CLRSCR(MAIN_FRAME_COL);

	//		if (screen >= 1 && screen <= 3)////REW_UP
	//		{
				//, con_value ? 1 : 0);
		//	}
		    Screen_frame(&screens[screen - 1],con_value);
	//		else
	//		{
			//	Screen_frame(&(ScreenConfig){"Rate", "ml/hr", "        Start Infusion"},con_value);//, 1);
		//	}

				Left_Right_Indi(Cursor_no,1);
			    FR_split_frame();

			    Digit_Spiliting(P_value);
			    flowrate_option_frame(Cursor_no,CURSOR_COL,BLACK);
			    flowrate_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
			    flowrate_frame_layout();
			    fr_flag=1;

		}

	     if(KEY.value)//common_bug  //ZOOZ_1
		{
		   auto_lock_time=AUTO_EXIT_TIME;
		}

	  //  if(normal_run==1)
	    {
	    	syringe_disp_status(0);
	    }

	    sy_size=detect_syringe_size(1);//REW_UP

		if(screen==2)
		{
			Max_Limit=syr_limit[sy_size];
		}

		if(((normal_run==0) && (sy_size!=G_pre_sy_size_1[0])) && (!G_fr_wrn_flag) )
	  	{
//	  		Frame_Footer(RED,YELLOW,(uint8_t*)"Syringe Load error",(uint8_t*)"",0,14);
	   		G_fr_wrn_flag=1;
	   	}

	   	if(((con_value==0) && (fr_flag==1))  && (!Bol_fr_flag) )
		{
	   		fr_flag=0;
	   		TFT_ST7789V__rectangle_curve(2,185,51,315,0,RED);
			Rectangle(3,185,313,12,MAIN_FRAME_COL);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(90,206,(uint8_t*)"FR under range",RED,YELLOW);
		}

		if(((con_value>=1) && (con_value<12000)) && (fr_flag!=1)  )
		{
			fr_flag=1;
			flowrate_frame_layout();
		}



	//
	//	else if(!auto_lock_time)
	//	{
	//		condition=0;
	//	  // break;
	//	}


         	 switch(KEY.value)
         	{
         	 case LEFT_KEY:

         		 if(Cursor_no<4)
         		 {
         			 //beep_sound();
         			 Cursor_no++;
         			 flowrate_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
         			 if(Cursor_no !=2 && Cursor_no !=3 )
         			 {
         				 Left_Right_Indi(Cursor_no,1);
         			 }
         		 }
         		 prev_cursor_no=Cursor_no;
         		 while(KEY.value);
         		 break;

         	 case RIGHT_KEY:

         		if(Cursor_no>0)
         		{
         			//beep_sound();
         			Cursor_no--;
         			flowrate_option_scroll(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
         			if(Cursor_no !=1 && Cursor_no !=2 )
         			{
         				Left_Right_Indi(Cursor_no,1);
         			}
         		}
        		prev_cursor_no=Cursor_no;
        		while(KEY.value);
         		break;

         	 case UP_KEY:

				if(array_dig1[Cursor_no]<9)
				{
					//beep_sound();
					if(Cursor_no==0)
					{
						space=15;
					}

					else
					{
						space=0;
					}

					Font_Select(BEBAS_43);
					letter[0]=array_dig1[Cursor_no]+48;

					xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
					85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
					TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,CURSOR_COL);

					if(sy_size!=0)
					{
						array_dig1[Cursor_no]++;
					}


				    con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
				    G_con_value=con_value;
				  // Font_Select(ARIAL_14);
				  //TFT_Number(0,110,con_value,YELLOW,RED);
				   if(sy_size!=0)
				   {
						if((array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1)>Max_Limit)
						{
							array_dig1[Cursor_no]--;
						}
				   }

					letter[0]=array_dig1[Cursor_no]+48;

					xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
					85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
					TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,BLACK);

				}
					else
					{
						WARN_SOUND(3,1,5);
					}

         		 while(KEY.value);
         		 break;

         	 case DOWN_KEY:

         		 if(array_dig1[Cursor_no]>0)
         		 {
         			 //beep_sound();
         			 if(Cursor_no==0)
         			 {
         				 space=15;
         			 }
         			 else
         			 {
         				 space=0;
         			 }
					letter[0]=array_dig1[Cursor_no]+48;
					Font_Select(BEBAS_43);
					xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
					85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
					TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,CURSOR_COL);
					array_dig1[Cursor_no]--;

				    con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
				    G_con_value=con_value;



				    if((array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1)<Min_Limit)
					{
						array_dig1[Cursor_no]++;
					}

					letter[0]=array_dig1[Cursor_no]+48;
					xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
					85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
					TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,BLACK);


					 if((con_value==0) && (fr_flag==1)  && (!Bol_fr_flag) )
					 {

						 fr_flag=0;
						 TFT_ST7789V__rectangle_curve(2,185,51,315,0,RED);
						 Rectangle(3,185,313,12,MAIN_FRAME_COL);
						 Font_Select(ARIAL_BOLD_14);
						 TFT_String(90,206,(uint8_t*)"FR under range",RED,YELLOW);
					 }

					 if(((con_value>1) && (con_value<12000)) && (fr_flag!=1)  )
					 {
						 fr_flag=1;
						 TFT_ST7789V__rectangle_curve(2,185,51,315,0,ORANGE_DARK);
						Rectangle(3,185,313,12,MAIN_FRAME_COL);
						 right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
						 Font_Select(ARIAL_BOLD_14);
						 TFT_String(80,203,(uint8_t*)"OKAY",ORANGE_DARK,WHITE);
						 Font_Select(ARIAL_BOLD_14);

						 TFT_String(195-30-5+10-7,202+3,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
					 //   TFT_String(90,203,(uint8_t*)"to confirm",ORANGE_DARK,WHITE);

						 //Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to confirm",(uint8_t*)"OK",1);
					 }


         		 }
         		 else
         		 {
         			 WARN_SOUND(3,1,5);
         		 }
     			 while(KEY.value);
     			 break;

         	 case OK_KEY:
			  //beep_sound();
			  con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
			  G_con_value=con_value;
			  check_flag=1;

				if(!Err_flag)
				{

					if (normal_run==0)
					 {
						  *flow_rate=con_value;

						 if(!Bol_fr_flag)// || (Bolus_rate_flag))
						 {

						   *flow_rate=con_value;
						   check_flag=1;
						   condition=0;
						 }
						   else if((Bol_fr_flag) && (con_value))
						   {
							   *flow_rate=con_value;
							   check_flag=1;
							   condition=0;


						   }
						 }


					 if((normal_run==1) &&  (con_value==0))
					 {
							WARN_SOUND(3,1,3);
						   //*flow_rate=con_value;
					 }

					  else
					  {
						 if(normal_run==1)
						  {
							 *flow_rate=con_value;
							  check_flag=1;
							  condition=0;
						  }
					  }

				}

//				   check_flag=1;
//				   condition=0;


			while(KEY.value);
			break;

			case BACK_KEY:
			//beep_sound();
			check_flag=0;
			condition=0;
			while(KEY.value);
			break;

			case START_STOP_KEY:
				con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
				G_con_value=con_value;
				*flow_rate=con_value;


				if(!G_infusion_modes)
				{
					if(normal_run==0)
					{
						if(MOT.flowrate!=0)
						{
							normal_run=1;
						//	Rectangle(0,0,320,240,MAIN_FRAME_COL);		//21-03-2024
						///	Font_Select(ARIAL_14);
						//	TFT_Number(0,70,100,YELLOW,RED);
				//			press_run_alarm=180;
							//Warning_Scr_Erase();
							plunger_based_o_ml_max_ml_calculate();
							syringe_nearempty_calculation(MOT.flowrate);
	//						event_no_update(3);
							BOL.run_mode=0;
							MFB.Motor_pulse=0;
							RAMP.RPM_control=0;    //ZOOZ_1
							DRV8428_ENABLE();
							MR_Value_calculate(MOT.flowrate);
							condition=0;

						}
					}

					else
					{
						normal_run=0;
						BOL.run_mode=0;
	//					event_no_update(4);
						Timer_disable();
						RAMP.down_flag=0;
						condition=0;

						//siphonic_time=15;
					}
				check_flag=1;


				}
				while(KEY.value);
				break;
         	}

     }

     return check_flag;
}


void FR_split_frame_DD()
{
	 uint8_t loop_count=0;

	 for(loop_count=0;loop_count<=3;loop_count++)

		 {
		     Rectangle(40+(45*loop_count),68,0,115,BLACK);
	     }

	    Rectangle(40+(45*3)+15,68,0,115,BLACK);
	    Rectangle(40+(45*4)+15,68,0,115,BLACK);
	 	Rectangle(40+(45*5)+15,68,0,115,BLACK);
	 	Font_Select(BEBAS_43);
	    TFT_String(221-41,123,(uint8_t*)".",MAIN_FRAME_COL,WHITE);

}


void Digit_Spiliting_DD(uint32_t Value)
{
	char letter[2]={'\0','\0'} ;
	uint8_t space=0;
	uint8_t loop_count=0;
	uint8_t position_1=0;
	uint16_t xpos=0;
	if(Value==0)
	{
		for(loop_count=0;loop_count<5;loop_count++)
		{
			array_dig1[position_1]=0;
			position_1++;
		}
	}

	else
	{
		while(Value>0)
		{
			array_dig1[position_1]=Value%10;
			Value=Value/10;
			position_1++;
		}
	}

	for(loop_count=0;loop_count<position_1;loop_count++)
	{
		if(loop_count==0 || loop_count==1)
		{
			space=15;
		}
		else
		{
			space=0;
		}

		Font_Select(BEBAS_43);

		letter[0] =	 array_dig1[loop_count]+48;

		xpos=Letters_Alignments(40+(45*(4-loop_count))+space,
		85+(45*(4-loop_count))+space, (uint8_t*)letter,0);

		//Font_Select(BEBAS_60);
		if(loop_count !=1 && loop_count !=2 )
		{
			TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[loop_count],MAIN_FRAME_COL,WHITE);
		}
	}
}

void flowrate_option_frame_DD(uint8_t current_cur_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL)
	{
	char letter[2]={'\0','\0'} ;
	uint32_t back_colour   = 0;
	uint32_t text_colour   = 0;
	uint8_t   loop		   = 0;
	uint8_t Cursor_no	   = 0;
	uint8_t space		   = 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint32_t con_value=0;
	uint8_t position_1=0;
	uint8_t fr_pos=0;
	uint16_t xpos=0;
	con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;

	while(con_value>0)
		     {
		     array_dig1[position_1]=con_value%10;
		     con_value=con_value/10;
		     position_1++;
		     }
	if(position_1 < 3)
	{
		position_1=3;
	}
	if(current_cur_no >= position_1)
	{
		fr_pos=current_cur_no;
	}
	else
	{
		fr_pos=position_1-1;
	}
	for(loop = 0; loop<=fr_pos; loop++)
	 	{
	 		if(loop!=current_cur_no)
	 		{
	 			back_colour		 = MAIN_FRAME_COL;
	 			text_colour 	 = WHITE;
	 		//	Cursor_no		 = prev_cursor_no;
                arrow_bg_color	 = MAIN_FRAME_COL;
                arrow_fill_color = MAIN_FRAME_COL;
	 		}
	 		else
	 		{
	 			back_colour 		= LIGHT_GRAY;
	 			text_colour  		= BLACK;
	 			Cursor_no			= current_cur_no;
	 			arrow_bg_color	    = LIGHT_GRAY;
				arrow_fill_color	= BLACK;
	 		}

 if(loop==0 || loop==1)
 {
	space=15;
 }
 else
 {
	space=0;
 }
 //letter[0]=array_dig1[Cursor_no]+48;
   TFT_ST7789V__rectangle_curve(40+1+(45*(4-loop)+space),64,125,43,0,back_colour);
 	//while(1);
 	Font_Select(BEBAS_43);
 	letter[0]=array_dig1[loop]+48;
 	xpos=Letters_Alignments(40+(45*(4-loop))+space, 85+(45*(4-loop))+space, (uint8_t*)letter,0);

 	TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[loop],back_colour,text_colour);
 	if(loop==current_cur_no)
 	{
 	TFT_ST7789V_up_down_arrow_20_arial(79,52+(45*(4-Cursor_no))+space,0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
	TFT_ST7789V_up_down_arrow_20_arial(166,52+(45*(4-Cursor_no))+space, 1,arrow_bg_color,arrow_fill_color)	;	//DOWN_ARROW

 	}
	}
	}
void flowrate_option_scroll_DD(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
	char letter[2]={'\0','\0'} ;
	uint32_t back_colour   = 0;
	uint32_t text_colour   = 0;
	uint8_t   loop		   = 0;
	uint8_t Cursor_no	   = 0;
	uint8_t space		   = 0;
	uint32_t arrow_bg_color	= 0;
	uint32_t arrow_fill_color =  0;
	uint16_t xpos=0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour		 = MAIN_FRAME_COL;
			text_colour 	 = WHITE;
			Cursor_no		 = prev_cursor_no;
			arrow_bg_color	 = MAIN_FRAME_COL;
			arrow_fill_color = MAIN_FRAME_COL;
		}
		else
		{
			back_colour 		= LIGHT_GRAY;
			text_colour  		= BLACK;
			Cursor_no			= current_cur_no;
			arrow_bg_color	    = LIGHT_GRAY;
			arrow_fill_color	= BLACK;
		}

		if(Cursor_no==0 ||  Cursor_no==1 )
		{
			space=15;
		}
//			TFT_ST7789V__rectangle_curve_1(40+(45*(4-Cursor_no))+space,66,125,43,0,back_colour);
//			Font_Select(BEBAS_60);
//			xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
//			50+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
//			TFT_Number(xpos,98,array_dig1[Cursor_no],back_colour,text_colour);
//			TFT_ST7789V_up_down_arrow_20_arial(81,52+(45*(4-Cursor_no))+space,0,arrow_bg_color,arrow_fill_color);   //UP_ARROW
//			TFT_ST7789V_up_down_arrow_20_arial(168,52+(45*(4-Cursor_no))+space,1,arrow_bg_color,arrow_fill_color);  //UP_ARROW
//		}

		else
		{
			space=0;
		}

		letter[0]=array_dig1[Cursor_no]+48;
			TFT_ST7789V__rectangle_curve(40+1+(45*(4-Cursor_no))+space,64,125,43,0,back_colour);
			//while(1);
			Font_Select(BEBAS_43);

			xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
		85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);

			TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],back_colour,text_colour);
			TFT_ST7789V_up_down_arrow_20_arial(79,52+(45*(4-Cursor_no))+space+2,0,arrow_bg_color,arrow_fill_color);  		//UP_ARROW
			TFT_ST7789V_up_down_arrow_20_arial(166,52+(45*(4-Cursor_no))+space+2, 1,arrow_bg_color,arrow_fill_color)	;	//DOWN_ARROW
		}
	}





uint16_t Dual_Deci_val_edit_scr(uint32_t *flow_rate,uint8_t min_limit, uint32_t Limit ,uint8_t mode)
{
	char letter[2]={'\0','\0'} ;
	uint8_t Cursor_no=2;
	uint8_t prev_cursor_no=1;
	uint8_t space=0;
	uint8_t loop_count=0;
	uint16_t con_value=0;
	uint8_t check_flag=1;
	uint8_t edit_flag=0;
	uint8_t condition=1;
	uint16_t xpos=0;
	uint16_t P_value=*flow_rate;      //That poiter variable store to the local var
	//auto_lock_time=AUTO_EXIT_TIME;
//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	CENTER_SCR(MAIN_FRAME_COL);  //center green
	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);



	if(mode==1)
	{
		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Rate");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green

		Font_Select(ARIAL_12);
		//TFT_String(250,44,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
	//	CENTER_SCR(MAIN_FRAME_COL);  //center green


	}
	else if(mode==2)
	{

		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Patient  Weight");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green
		Font_Select(ARIAL_12);
		TFT_String(280,40+4,(uint8_t*)"kg",MAIN_FRAME_COL,WHITE);


	}
	else if(mode==3)
	{

		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Dose Concentration");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green
		Font_Select(ARIAL_12);
		//Rectangle(260,38,60,22,LIGHT_GRAY);
		TFT_String(265,38+6,(uint8_t*)drug_unit[G_page_no][G_opt_no],MAIN_FRAME_COL,WHITE);
		//CENTER_SCR(MAIN_FRAME_COL);  //center green

		//Rectangle(260,38,60,22,LIGHT_GRAY);


	}
	else if(mode==4)
	{

		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Dose Concentration");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green
		//Rectangle(285,38,30,20,LIGHT_GRAY);

		Font_Select(ARIAL_12);
		TFT_String(290,38+6,(uint8_t*)"ml",MAIN_FRAME_COL,WHITE);
		TFT_String(10,38+6,(uint8_t*)"Conc :",MAIN_FRAME_COL,WHITE);
		TFT_String(60,38+6,(uint8_t*)" = ",MAIN_FRAME_COL,WHITE);

	}
	else if(mode==5)
	{

		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Drug Dosage");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green
		Font_Select(ARIAL_12);
		//Rectangle(203,38,117,25,LIGHT_GRAY);
		TFT_String(210,40+4,(uint8_t*)dose_rate_unit[dose_option_no],MAIN_FRAME_COL,WHITE);
		TFT_String(10,38+6,(uint8_t*)"=",MAIN_FRAME_COL,WHITE);
		TFT_String(100+40,38+4,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
		//Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to Confirm",(uint8_t*)"OK",1);

		dosage_range_limit_fun(mode,con_value);
//		Rectangle(80+D_xpos,37,88+20+20+10,25,MAIN_FRAME_COL);
//		TFT_String(80+D_xpos+35,38,(uint8_t*)disp_para,MAIN_FRAME_COL,WHITE);

	}
	else if(mode==6)
	{

		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Patient  Height");	// header
		CENTER_SCR(MAIN_FRAME_COL);  //center green
		Font_Select(ARIAL_12);
		TFT_String(280,44,(uint8_t*)"cm",MAIN_FRAME_COL,WHITE);
		TFT_String(10,38+6,(uint8_t*)"BSA",MAIN_FRAME_COL,WHITE);
		Rectangle(40+20,44,20+10+30,20,MAIN_FRAME_COL);
		TFT_String(60-3,38+6,(uint8_t*)"=",MAIN_FRAME_COL,WHITE);
		TFT_String(37+30+60,40+4,(uint8_t*)"m2",MAIN_FRAME_COL,WHITE);
		dosage_range_limit_fun(mode,con_value);


	}
	Left_Right_Indi(Cursor_no,1);

	for(loop_count=0;loop_count<5;loop_count++)
	{
		array_dig1[loop_count]=0;
	}

	FR_split_frame_DD();
	Digit_Spiliting_DD(P_value);
	flowrate_option_scroll_DD(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
	prev_cursor_no=2;
	edit_flag=1;
	while(KEY.value);

	while(condition)
	{


		Power_off();

	    if((!ON_Flag))//||(!auto_lock_time))
		{
			break;		//01-04
		}

		else if(POW.power_status_flag)
		{

			home_flag=0;
			POW.power_status_flag=0;		//01-04
			//Rectangle(0,0,320,240,MAIN_FRAME_COL);
			Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
			right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
			Font_Select(ARIAL_BOLD_14);
			TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
			Font_Select(ARIAL_12);

				if(mode==1)
				{
					Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Rate");
					CENTER_SCR(MAIN_FRAME_COL);  //center green
					Font_Select(ARIAL_12);
					//TFT_String(250,44,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
				}

				else if(mode==2)
				{
					Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Patient  Weight");
					CENTER_SCR(MAIN_FRAME_COL);  //center green
					Font_Select(ARIAL_12);
					TFT_String(280,40+4,(uint8_t*)"kg",MAIN_FRAME_COL,WHITE);

				}

				else if(mode==3)
				{

					Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Dose Concentration");
					CENTER_SCR(MAIN_FRAME_COL);  //center green
					Font_Select(ARIAL_12);
					//Rectangle(260,38,60,22,LIGHT_GRAY);
					TFT_String(280,40+4,(uint8_t*)drug_unit[G_page_no][G_opt_no],LIGHT_GRAY,BLACK);

				}

				else if(mode==4)
				{

					Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Dose Concentration");
					CENTER_SCR(MAIN_FRAME_COL);  //center green
					//Rectangle(285,38,30,20,LIGHT_GRAY);
					Font_Select(ARIAL_12);
					TFT_String(290,38+6,(uint8_t*)"ml",LIGHT_GRAY,BLACK);
					TFT_String(10,38+6,(uint8_t*)"Conc :",MAIN_FRAME_COL,WHITE);
					TFT_String(60,38+6,(uint8_t*)" = ",MAIN_FRAME_COL,WHITE);
					dosage_range_limit_fun(mode,con_value);
					//Rectangle(80+D_xpos,37,88+20+20+10-9,25,MAIN_FRAME_COL);
				    TFT_String(120+50,38+4,(uint8_t*)disp_para,MAIN_FRAME_COL,WHITE);

				}

				else if(mode==5)
				{

					Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Drug Dosage");
					Font_Select(ARIAL_12);
					CENTER_SCR(MAIN_FRAME_COL);  //center green
					//Rectangle(203,38,117,25,LIGHT_GRAY);
					TFT_String(10,38+6,(uint8_t*)"=",MAIN_FRAME_COL,WHITE);
					TFT_String(210,44,(uint8_t*)dose_rate_unit[dose_option_no],MAIN_FRAME_COL,WHITE);
					//Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"OKAY");
					dosage_range_limit_fun(mode,con_value);
					Font_Select(ARIAL_12);
					TFT_String(100+40,38+4,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);

//					if(!Prev_DF_xpos)
//					{
//							TFT_String(80+Prev_DF_xpos,38+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,MAIN_FRAME_COL);
//							TFT_String(80+DF_xpos,38+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
//						Prev_DF_xpos=DF_xpos;
//					}
//
//					else
//					{
//							TFT_String(80+Prev_DF_xpos-5,38+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,MAIN_FRAME_COL);
//							TFT_String(80+DF_xpos-5,38+6,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
//						Prev_DF_xpos=DF_xpos;
//					}

				}
				else if(mode==6)
					{

						Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Patient  Height");	// header
						CENTER_SCR(MAIN_FRAME_COL);  //center green
						Font_Select(ARIAL_12);
						TFT_String(280,44,(uint8_t*)"cm",MAIN_FRAME_COL,WHITE);
						TFT_String(10,38+6,(uint8_t*)"BSA",MAIN_FRAME_COL,WHITE);
						TFT_String(60,38+6,(uint8_t*)"=",MAIN_FRAME_COL,WHITE);
						TFT_String(37+30+60,40+4,(uint8_t*)"m2",MAIN_FRAME_COL,WHITE);
						Rectangle(40+20,44,20+10+30,20,MAIN_FRAME_COL);
						dosage_range_limit_fun(mode,con_value);
						//TFT_writenumber_float(37,40+4,1.63,MAIN_FRAME_COL,WHITE);


					}
			FR_split_frame_DD();
			flowrate_option_frame_DD(Cursor_no,CURSOR_COL,BLACK);
			Left_Right_Indi(Cursor_no,0);

		}

//	    if(KEY.value)//common_bug  //ZOOZ_1
//		{
//		   auto_lock_time=AUTO_EXIT_TIME;
//		}

	    if(((mode==2)||(mode==4) || (mode==5)||(mode==6))  && (edit_flag==1))
		{

    	con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
		dosage_range_limit_fun(mode,con_value);

		if(mode==4)
		{
				//Rectangle(80+D_xpos,37,88+20+20+10-9,25,MAIN_FRAME_COL);
			Font_Select(ARIAL_12);
			TFT_String(120+50,44,(uint8_t*)disp_para,MAIN_FRAME_COL,WHITE);
		}

		else if(mode==5)
		{

			Font_Select(ARIAL_12);
			//TFT_String(83+10,38,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
			TFT_String(100+40,38+4,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
//			if(!Prev_DF_xpos)
//			{
//					TFT_String(88+Prev_DF_xpos,38,(uint8_t*)"ml/h",MAIN_FRAME_COL,MAIN_FRAME_COL);
//					TFT_String(83+DF_xpos,38,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
//				Prev_DF_xpos=DF_xpos;
//			}
//
//			else
//			{
//					TFT_String(80+Prev_DF_xpos,38,(uint8_t*)"ml/h",MAIN_FRAME_COL,MAIN_FRAME_COL);
//					//Rectangle(90,38,100+10,25,MAIN_FRAME_COL);
//					TFT_String(80+DF_xpos-5,38,(uint8_t*)"ml/h",MAIN_FRAME_COL,WHITE);
//				Prev_DF_xpos=DF_xpos;
//			}

		}
		else if(mode==6)
		{
			//Rectangle(40+20,44-10,20,20,YELLOW);
			//Rectangle(40+20,44,20+10+30,20,MAIN_FRAME_COL);
			TFT_String(37+30+60,40+4,(uint8_t*)"m2",MAIN_FRAME_COL,WHITE);
		}

		edit_flag=0;
		}


		switch(KEY.value)
		{
			case LEFT_KEY:
			//beep_sound();
			if(Cursor_no<4)
			{
				Cursor_no++;
				flowrate_option_scroll_DD(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
				if(Cursor_no !=2 && Cursor_no !=3 )
				{
					Left_Right_Indi(Cursor_no,1);
				}
			}
			prev_cursor_no=Cursor_no;
			while(KEY.value);
			break;

			case RIGHT_KEY:
			//beep_sound();
			if(Cursor_no>0)
			{
				Cursor_no--;
				flowrate_option_scroll_DD(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
				if(Cursor_no !=1 && Cursor_no !=2 )
				{
					Left_Right_Indi(Cursor_no,1);
				}
			}
			prev_cursor_no=Cursor_no;
			while(KEY.value);
			break;

			case UP_KEY:
			//beep_sound();
			edit_flag=1;
				if((array_dig1[Cursor_no]<9) &&( dose.flow_rate<=120000))
			{

				if(Cursor_no==0 || Cursor_no==1)
				{
					space=15;
				}

				else
				{
					space=0;
				}
				letter[0]=array_dig1[Cursor_no]+48;

				Font_Select(BEBAS_43);
				xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
				85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,CURSOR_COL);

				array_dig1[Cursor_no]++;
				if((array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1)>Limit)
				{
					array_dig1[Cursor_no]--;
				}

					letter[0]=array_dig1[Cursor_no]+48;

					Font_Select(BEBAS_43);
					xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,
					85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
					TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,BLACK);

			}

			 else
			 {
					 WARN_SOUND(1,1,2);
			 }

			while(KEY.value);
			break;

			case DOWN_KEY:

			//beep_sound();
			edit_flag=1;
			if(array_dig1[Cursor_no]>0)
			{

				if(Cursor_no==0 || Cursor_no==1)
				{
					space=15;
				}

				else
				{
					space=0;
				}

				letter[0]=array_dig1[Cursor_no]+48;
				Font_Select(BEBAS_43);
				xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,CURSOR_COL);

				array_dig1[Cursor_no]--;
				letter[0]=array_dig1[Cursor_no]+48;

				Font_Select(BEBAS_43);
				xpos=Letters_Alignments(40+(45*(4-Cursor_no))+space,85+(45*(4-Cursor_no))+space, (uint8_t*)letter,0);
				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig1[Cursor_no],CURSOR_COL,BLACK);

			}

			 else
			 {
					 WARN_SOUND(1,1,2);
			 }
			while(KEY.value);
			break;

			case OK_KEY:

			//beep_sound();
			if(((mode==5) && ((dose.flow_rate>10 && dose.flow_rate<=120000))) || ((mode!=5)))			//(dose.flow_rate>0.0) )  //(dose.flow_rate<=120000)&& (dose.flow_rate>0)
			{

			con_value=array_dig1[4]*10000+array_dig1[3]*1000+array_dig1[2]*100+array_dig1[1]*10+array_dig1[0]*1;
			*flow_rate=con_value;
			check_flag=1;
			condition=0;

			}

			else
			{
					WARN_SOUND(3,1,3);
			}
			while(KEY.value);
			break;

			case BACK_KEY:

			//beep_sound();
			dosage_range_limit_fun(mode,P_value);
			condition=0;
			check_flag=0;

			while(KEY.value);
			break;

		}
	}
	return check_flag;;
}


void Left_Right_Indi(uint8_t cursor_pos,uint8_t screen_id)
{
	uint32_t Right_bg_color;
	uint32_t left_bg_color;
	uint8_t cursor_no=0;

    if(screen_id==1)
    {
    	cursor_no=4;
    }
    else
    {
    	cursor_no=3;
    }

	if(cursor_pos==0 )
	{
    Right_bg_color		=	LIGHT_GRAY_3;
    left_bg_color		=	WHITE;
	}

	else if(cursor_pos==cursor_no)
	{
	 Right_bg_color		= WHITE;
     left_bg_color		=		LIGHT_GRAY_3;
	}

	else
	{
	Right_bg_color		= WHITE;
	left_bg_color		=WHITE;
	}

    TFT_ST7789V_lcd_drawcircle1(20,130,12,left_bg_color,left_bg_color);           						//	 Left Circle
    TFT_ST7789V_lcd_drawcircle1(300,130,12,Right_bg_color,Right_bg_color);   //23-03					// Right Circle

    TFT_ST7789V_left_Right_arrow_14_arial(123,20-5,0,left_bg_color,NEON_BLUE_1);        	//	Left Arrow
    TFT_ST7789V_left_Right_arrow_14_arial(123,303-5,1,Right_bg_color,NEON_BLUE_1);  	//	Right Arrow

}

void flowrate_frame_layout(void)
{
	TFT_ST7789V__rectangle_curve(2,185,51,315,0,ORANGE_DARK);
	Rectangle(3,185,313,12,MAIN_FRAME_COL);
	right_14(210,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(82,209,(uint8_t*)"OKAY",ORANGE_DARK,WHITE);

	TFT_String(163,209,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

}
