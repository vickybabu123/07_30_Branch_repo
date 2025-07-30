#include "LPC55S16.h"
#include "Color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Keypad.h"
#include "Driver_Timer.h"
#include <Shapes.h>
#include "Calibration_Screen.h"
#include "Power_off.h"
#include  "sound.h"
#include "Flowrate_Screen.h"
#include "Main_screen.h"


uint8_t array_dig2[6];
//char letter1[2]={'\0','\0'} ;
extern uint8_t ON_Flag;
//extern uint8_t last_value;
//extern uint8_t std_confirm_flag;
//extern char letter1[2];
uint8_t std_confirm_flag;



void Zero_PSI_Calibration()
{
	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"0 PSI Calibration");	// header
	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");	//CENTRE GREEN
	Rectangle(11,47,298,145,MAIN_FRAME_COL); //center green
	Rectangle(11,53,298,131,LIGHT_GRAY);




	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,202+3-1,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

//    Font_Select(BEBAS_43);
//    TFT_String(63,85,(uint8_t *)2010,MAIN_FRAME_COL,WHITE);
//
//   Rectangle(7,162,290,0,BLACK);
//   TFT_ST7789V__rectangle_curve(165,5,290,34,0,LIGHT_GRAY);  //Rectangle Curve Option 1
//
//
//	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
//	Font_Select(ARIAL_BOLD_14);
//	TFT_String(163,202+3-1,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

//   TFT_ST7789V_lcd_drawcircle1(222,100,15,YELLOW,YELLOW);//CIRCLE
//
//   Font_Select(ARIAL_14);
//   TFT_String(210,120,(uint8_t *)"Confirm",NEW_FTR_COL,BLACK);
//
//   Font_Select(ARIAL_9);
//   TFT_String(216,92,(uint8_t*)"OK",YELLOW,BLACK);

//
//   Font_Select(ARIAL_14);
//   TFT_String(36+(4*34),110,(uint8_t*)"Calibrate",LIGHT_GRAY,BLACK);
}

void Warning_Frame()
{
    Rectangle(0,0,320,240,MAIN_FRAME_COL);
 //TFT_ST7789V__Bottom_curve(0,30,320-60,30,500,HDR_BG_COL);                                                                                              //DRAW A RECTANGULAR CURVE IN THE TOP OF THE TFT DISPLAY

// Font_Select(ARIAL_12);
// TFT_String(4,100,(uint8_t *)"0  PSI  Calib",AKAS_GREEN,WHITE);
     Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"0 PSI Calibration");


 //Rectangle(0,200,320,240,FTR_COL);

 TFT_String(65,25,(uint8_t *)"i)  Remove the Syringe",MAIN_FRAME_COL,YELLOW);

 TFT_String(105,25,(uint8_t *)"ii) Don't touch the",MAIN_FRAME_COL,YELLOW);

 TFT_String(135,45,(uint8_t *)"Plunger Disc",MAIN_FRAME_COL,YELLOW);

 right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
 	Frame_Footer__new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
 	Font_Select(ARIAL_BOLD_14);
 	TFT_String(195-30-5+10-7,202+3,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

// TFT_ST7789V_lcd_drawcircle1(219,100,15,YELLOW,YELLOW);//CIRCLE
//
// TFT_String(210,120,(uint8_t *)"Confirm",NEW_FTR_COL,BLACK);
//
// Font_Select(ARIAL_9);
// TFT_String(214,92,(uint8_t*)"OK",YELLOW,BLACK);
}

//void Indication_Frame()
//{
// Rectangle(0,0,320,240,MAIN_FRAME_COL);
// TFT_ST7789V__Bottom_curve(0,30,320-60,30,500,HDR_BG_COL);                                         //DRAW A RECTANGULAR CURVE IN THE TOP OF THE TFT DISPLAY
//
//// Font_Select(ARIAL_14);
//// TFT_String(4,100,(uint8_t *)"0  PSI  Calib",AKAS_GREEN,WHITE);
//  Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"0 PSI Calibration");
//
//  Rectangle(4,43+20,311,0,LIGHT_GRAY);       //TOP LINE
//  	Rectangle(315,44+20,0,151,LIGHT_GRAY);     //RIGHT LINE
//  	Rectangle(3,196+20,311,0,LIGHT_GRAY);		//BOTTOM LINE
//  	Rectangle(3,44+20,0,151,LIGHT_GRAY);		//LEFT LINE
//
//  	Font_Select(ARIAL_BOLD_14);
//  	TFT_ST7789V__rectangle_curve(10,50+20,138,300,0,WHITE);
//
//  	TFT_String(95+5,120+10,(uint8_t*)"Calibrated",WHITE,BLACK);
//
//// TFT_ST7789V_lcd_drawcircle1(219,100,15,YELLOW,YELLOW);//CIRCLE
////
//// TFT_String(210,120,(uint8_t *)"Confirm",WHITE,BLACK);
////
//// Font_Select(ARIAL_9);
//// TFT_String(214,92,(uint8_t*)"OK",YELLOW,BLACK);
//}



uint8_t edit_screen(uint16_t *Edit_value,uint8_t screen,uint16_t max_value)

{
	uint8_t prev_cursor_no=1;
				uint8_t space=0;
				uint8_t Cursor_no=2;
				uint8_t max_arr[4];

				uint16_t result=0;
				uint8_t condition=1;
				uint8_t xpos=0;
				uint16_t Value=0;
				uint8_t Err_flag=0;
			//	uint8_t Err_Chk_flag=0;
				uint8_t loop_cnt=0;
				Value=*Edit_value;
				char letter1[2]={'\0','\0'} ;

	//			TFT_ST7789V_lcd_drawcircle1(31,128,12,WHITE,WHITE);
	//			TFT_ST7789V_lcd_drawcircle1(289,127,12,WHITE,WHITE);
	//			TFT_ST7789V_left_Right_arrow_14_arial(121,26,0,WHITE,NEON_BLUE_1);
	//			TFT_ST7789V_left_Right_arrow_14_arial(120,287,1,WHITE,NEON_BLUE_1);

				edit_split_frame(screen);
				Digit_Spiliting1(Value,Cursor_no,prev_cursor_no);
				Left_Right_Indi(Cursor_no,0);
				while(max_value)
				{
					max_arr[loop_cnt]=max_value%10;
					max_value=max_value/10;
					loop_cnt++;
				}

				edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
				prev_cursor_no=2;



				while(condition)
				{
					Power_off();	//26-03
					if(!ON_Flag)
					{
						break;
					}		//26-03
					else if(POW.power_status_flag)
					{
						POW.power_status_flag=0;
						edit_split_frame(screen);
						result=(array_dig2[3]*1000)+(array_dig2[2]*100)+(array_dig2[1]*10)+array_dig2[0];
						Digit_Spiliting1(result,Cursor_no,prev_cursor_no);
						 Left_Right_Indi(Cursor_no,0);
						 edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
						 while(KEY.value); //23-03
					}

					switch(KEY.value)
					{
						case LEFT_KEY:
							////beep_sound();
						if(Cursor_no<3)
						{
							Cursor_no++;
							edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
							if(Cursor_no !=2 && Cursor_no !=0 )
							{
									Left_Right_Indi(Cursor_no,0);
							}
						}
						prev_cursor_no=Cursor_no;
						while(KEY.value);
						break;

						case RIGHT_KEY:
							////beep_sound();
						if(Cursor_no>0)
						{
							Cursor_no--;
							edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
							if(Cursor_no !=1 && Cursor_no !=3 )
							{
								   Left_Right_Indi(Cursor_no,0);
							}
						}
						prev_cursor_no=Cursor_no;
						while(KEY.value);
						break;



						case UP_KEY:
							////beep_sound();
							if(Cursor_no<2)
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

							TFT_Number(50,100,Cursor_no,RED,BLACK);

							if((Cursor_no==3)||(Cursor_no==1))
								{
								//TFT_Number(50,100,Cursor_no,RED,BLACK);
									if(array_dig2[Cursor_no-1]<=max_arr[Cursor_no-1])
									{
										if(array_dig2[Cursor_no]<max_arr[Cursor_no])
										{
											array_dig2[Cursor_no]++;

										}
										else
										{
											WARN_SOUND(3,1,5);
										}
									}
									else
									{
										if(array_dig2[Cursor_no]<max_arr[Cursor_no]-1)
									// 	if(array_dig2[Cursor_no]<max_arr[Cursor_no])
										{
											array_dig2[Cursor_no]++;
										}
										else
										{
											WARN_SOUND(3,1,5);
										}
									}
								}

								else if((Cursor_no==2)||(Cursor_no==0))
								{
									if(array_dig2[Cursor_no+1]<max_arr[Cursor_no+1])
									{
										if(array_dig2[Cursor_no]<9)
										{
											array_dig2[Cursor_no]++;
										}
										else
										{
											WARN_SOUND(3,1,5);
										}
									}

									else
									{
										if(array_dig2[Cursor_no]<max_arr[Cursor_no])
										{
											array_dig2[Cursor_no]++;
										}
										else
										{
											WARN_SOUND(3,1,5);
										}
									}
								}
						Font_Select(BEBAS_43);
						letter1[0]=array_dig2[Cursor_no]+48;
						xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
						106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
						TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);

						while(KEY.value);
						break;

						case DOWN_KEY:
							////beep_sound();

							if(Cursor_no<2)
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

						if(((Cursor_no==2)||(Cursor_no==0) )&& screen==1)
						{
							if(array_dig2[Cursor_no]>0)// 1 change to 0 //23-03
							{

								array_dig2[Cursor_no]--;

							}
							else
							{
								WARN_SOUND(3,1,5);
							}
						}
						else
						{
							if(array_dig2[Cursor_no]>0)
							{
								array_dig2[Cursor_no]--;
							}
							else
							{
								WARN_SOUND(3,1,5);
							}
						}

						Font_Select(BEBAS_43);
						letter1[0]=array_dig2[Cursor_no]+48;
						xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
						106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
						TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);



						while(KEY.value);
						break;

						case OK_KEY:
						////beep_sound();
						result=(array_dig2[3]*1000)+(array_dig2[2]*100)+(array_dig2[1]*10)+array_dig2[0];
						*Edit_value=result;
	//					if(result>0101)//common_bug
	//					{
	//
	//						condition=0;
	//
	//					}
	//					else
	//					{
	//						Notification(7);
	//						POW.power_status_flag=1;
	//
	//					}//common_bug

						Err_flag=1;
						condition=0;
						while(KEY.value);
						break;

						case BACK_KEY:
						////beep_sound();
						condition=0;
						Err_flag=0;
						while(KEY.value);
						break;


					}
				}
				return Err_flag;
}


void edit_split_frame(uint8_t screen)
{
	uint8_t loop_count=0;
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	border_lines();



	if(screen==1)
		{

		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Date & Month");
		Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");

		CENTER_SCR(MAIN_FRAME_COL);  //center green
		right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

		Font_Select(ARIAL_BOLD_14);
		TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
	//	Font_Select(ARIAL_12);
	//		TFT_String(80,50,(uint8_t*)"D",MAIN_FRAME_COL,LIGHT_GRAY);
	//		TFT_String(125,50,(uint8_t*)"D",MAIN_FRAME_COL,LIGHT_GRAY);
		}
		else if(screen==2)
		{
			Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Time");
			Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
			CENTER_SCR(MAIN_FRAME_COL);  //center green
			right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

		}
		else if(screen==3)
			{

				Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"20 PSI edit");
				Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
				CENTER_SCR(MAIN_FRAME_COL);  //center green
				right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

				Font_Select(ARIAL_BOLD_14);
				TFT_String(163,210,(uint8_t* )"to edit",ORANGE_DARK,WHITE);
				Font_Select(ARIAL_20);
				TFT_String(158,112,(uint8_t*)":",MAIN_FRAME_COL,LIGHT_GRAY);

			}
		else
		{
			Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Edit Collected mL");
			Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
			CENTER_SCR(MAIN_FRAME_COL);  //center green
			right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
		}


	    Rectangle(11,47,298,145,MAIN_FRAME_COL);//CENTRE GREEN
	    for(loop_count=0;loop_count<=2;loop_count++)
	    {
	    	Rectangle(62+(45*loop_count),68,0,115,LIGHT_GRAY);
	    }
	//       Rectangle(40+(45*4)+15,68,0,115,WHITE);
	    for(loop_count=2;loop_count<5;loop_count++)
	    {
	    	Rectangle(62+15+(45*loop_count),68,0,115,LIGHT_GRAY);
	    }
	//    Rectangle(11,47,298,145,MAIN_FRAME_COL);//CENTRE GREEN
	    if((screen==1)||(screen==2))//common_bug
	       {
	    	Font_Select(ARIAL_12);
	    	TFT_String(80,45,(uint8_t*)"D",MAIN_FRAME_COL,LIGHT_GRAY);
			TFT_String(125,45,(uint8_t*)"D",MAIN_FRAME_COL,LIGHT_GRAY);
	   		TFT_String(185,45,(uint8_t*)"M",MAIN_FRAME_COL,LIGHT_GRAY);
	   		TFT_String(230,45,(uint8_t*)"M",MAIN_FRAME_COL,LIGHT_GRAY);

	   		Font_Select(ARIAL_20);
	   		TFT_String(158,112,(uint8_t*)":",MAIN_FRAME_COL,LIGHT_GRAY);
	   	}
	       else
	       {
	       	Font_Select(ARIAL_20);
	       	TFT_String(157,119,(uint8_t*)".",MAIN_FRAME_COL,WHITE);
	       }
	    if(screen==2)
	    {
	    	Font_Select(ARIAL_12);
	    	TFT_String(80,45,(uint8_t*)"H",MAIN_FRAME_COL,LIGHT_GRAY);
			TFT_String(125,45,(uint8_t*)"H",MAIN_FRAME_COL,LIGHT_GRAY);
	    }

}

void Digit_Spiliting1(uint16_t Value,uint8_t cur_no,uint8_t prev_no)
 {
	uint8_t space=0;
		uint8_t loop_count=0;
	    uint8_t position_2=0;
		char letter1[2]={'\0','\0'} ;


	    uint16_t ypos1=0;

	    if(!Value)
			{
				for(position_2=0;position_2<=3;position_2++)
				{
					array_dig2[position_2]=0;
				}
			}
		 while(Value>0)
		     {
		     array_dig2[position_2]=Value%10;
		     Value=Value/10;
		     position_2++;
		     }
	    if(position_2==3)
	    {
	    	array_dig2[3]=0;

	    }
	    else if(position_2==2)
	    {
	    	array_dig2[2]=0;
	    	array_dig2[3]=0;
	    }
	    else if(position_2==1)
		{
			array_dig2[1]=0;
			array_dig2[2]=0;
			array_dig2[3]=0;
		}

		for(loop_count=0;loop_count<=3;loop_count++)
	     {
			if(loop_count==0|| loop_count==1)
			{
				space=15;
			}
			else
			{
				space=0;
			}

	      Font_Select(BEBAS_43);

	      letter1[0] =	 array_dig2[loop_count]+48;

	      ypos1=Letters_Alignments(63+(45*(3-loop_count))+space,
	    		  	  	  	  	  	  	  	  	  106+(45*(3-loop_count))+space, (uint8_t*)letter1,0);

	   //    Font_Select(BEBAS_43);
	     if(loop_count !=cur_no && loop_count !=prev_no )
	      {

	       TFT_Number(ypos1,CALIB_EDT_YPOS,array_dig2[loop_count],MAIN_FRAME_COL,WHITE);
	      }
	    }
  }

void edit_option(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
	uint32_t back_colour   = 0;
		uint32_t text_colour		= 0;
		uint8_t loop= 0;
		uint8_t Cursor_no		= 0;
		uint8_t space				= 0;
		uint32_t arrow_bg_color	= 0;
		uint32_t arrow_fill_color =  0;
		uint8_t ypos1=0;
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

			if(Cursor_no<2)
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

			ypos1=Letters_Alignments(62+(45*(3-Cursor_no))+space,
			106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);

			TFT_Number(ypos1,CALIB_EDT_YPOS,array_dig2[Cursor_no],back_colour,text_colour);

			TFT_ST7789V_up_down_arrow_20_arial(79,15+62+(45*(3-Cursor_no))+space,0,
			arrow_bg_color,arrow_fill_color);

			TFT_ST7789V_up_down_arrow_20_arial(166,15+62+(45*(3-Cursor_no))+space,1,
			arrow_bg_color,arrow_fill_color);

		}
}

uint8_t edit_screen_Time(uint32_t *Edit_value,uint8_t screen,uint16_t max_value)
{
	uint8_t prev_cursor_no=1;
		uint8_t space=0;
		uint8_t Cursor_no=2;
		uint8_t condition=1;
		uint8_t xpos=0;
		uint8_t Err_flag=0;
		uint32_t result=0;
		uint32_t Value=0;
		char letter1[2]={'\0','\0'} ;
		Value=*Edit_value;

		edit_split_frame(screen);
		Digit_Spiliting1(Value,Cursor_no,prev_cursor_no);

	//	TFT_ST7789V_lcd_drawcircle1(31,128,12,WHITE,WHITE);
	//	TFT_ST7789V_lcd_drawcircle1(289,127,12,WHITE,WHITE);
	//	TFT_ST7789V_left_Right_arrow_14_arial(121,26,0,WHITE,NEON_BLUE_1);
	//	TFT_ST7789V_left_Right_arrow_14_arial(120,287,1,WHITE,NEON_BLUE_1);
		edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
		prev_cursor_no=2;
		Left_Right_Indi(Cursor_no,0);

		while(KEY.value);

		while(condition)
		{
			 Power_off();

					 if(!ON_Flag)
					{
						break;//26-03
					}

					 else if(POW.power_status_flag)
				   {
						POW.power_status_flag=0;//26-03
						edit_split_frame(screen);
						Left_Right_Indi(Cursor_no,0);
						edit_option_frame(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);

				   }
			switch(KEY.value)
			{
				case LEFT_KEY:
					////beep_sound();
						if(Cursor_no<3)
						{
							Cursor_no++;
							edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
							if(Cursor_no !=0 && Cursor_no !=2 )
								{
									Left_Right_Indi(Cursor_no,0);
								}
						}
						prev_cursor_no=Cursor_no;
						while(KEY.value);
						break;

				case RIGHT_KEY:
					////beep_sound();
						if(Cursor_no>0)
						{
							Cursor_no--;
							edit_option(Cursor_no,prev_cursor_no,CURSOR_COL,BLACK,BLACK);
							if(Cursor_no !=1 && Cursor_no !=3 )
							{
								Left_Right_Indi(Cursor_no,0);
							}
						}
						prev_cursor_no=Cursor_no;
						while(KEY.value);
						break;



				case UP_KEY:
					////beep_sound();
					if(Cursor_no<2)
						{
							space=15;

						}

						else
						{
							space=0;
						}
					//	Font_Select(BEBAS_43);

						letter1[0]=array_dig2[Cursor_no]+48;
						Font_Select(BEBAS_43);
						xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
						106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
						TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,CURSOR_COL);
						if((array_dig2[Cursor_no]<9 && Cursor_no!=1) || (array_dig2[Cursor_no]<5 && Cursor_no==1))
						{
							array_dig2[Cursor_no]++;
						}
						if(max_value<((array_dig2[3]*1000)+(array_dig2[2]*100)+(array_dig2[1]*10)+array_dig2[0]))
						{
							array_dig2[Cursor_no]--;
						}

						letter1[0]=array_dig2[Cursor_no]+48;
						Font_Select(BEBAS_43);
						xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
						106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
						TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);
						while(KEY.value);
						break;

				case DOWN_KEY:
					////beep_sound();

						if(Cursor_no<2)
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
						if(array_dig2[Cursor_no]>0)
						{
							array_dig2[Cursor_no]--;
						}

						Font_Select(BEBAS_43);
						letter1[0]=array_dig2[Cursor_no]+48;
						xpos=Letters_Alignments(63+(45*(3-Cursor_no))+space,
						106+(45*(3-Cursor_no))+space, (uint8_t*)letter1,0);
						TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[Cursor_no],CURSOR_COL,BLACK);
						while(KEY.value);
						break;

				case OK_KEY:
					////beep_sound();
						result=(array_dig2[3]*1000)+(array_dig2[2]*100)+(array_dig2[1]*10)+array_dig2[0];
						*Edit_value=result;
						std_confirm_flag=1;
						condition=0;
						Err_flag=0;
						while(KEY.value);
						break;

				case BACK_KEY:
					////beep_sound();
						condition=0;
//						last_value=0;
						Err_flag=1;
						while(KEY.value);
						break;
			}
		}
		return Err_flag;
}

void edit_option_frame(uint8_t current_cur_no,uint8_t prev_cursor_no, uint32_t CUR_COL,uint32_t CUR_BACK_COL,uint32_t UP_ARROW_ERASE)
{
	char letter1[2]={'\0','\0'} ;
		    uint32_t back_colour   = 0;
			uint32_t text_colour   = 0;
			uint8_t loop		   = 0;
		//	uint8_t Cursor_no	   = 0;
			uint8_t space		   = 0;
			uint32_t arrow_bg_color	= 0;
			uint32_t arrow_fill_color =  0;
			uint8_t xpos=0;

			for(loop = 0; loop<=3; loop++)
			{
				if(loop!=current_cur_no)
				{
					back_colour			= MAIN_FRAME_COL;
					text_colour 		= WHITE;
					//Cursor_no			= prev_cursor_no;
					arrow_bg_color		= MAIN_FRAME_COL;
					arrow_fill_color	= MAIN_FRAME_COL;
				}
				else
				{
					back_colour 		= LIGHT_GRAY;
					text_colour  		= BLACK;
				//	Cursor_no			= current_cur_no;
					arrow_bg_color		= LIGHT_GRAY;
					arrow_fill_color	= BLACK;
				}

				if(loop<2)
				{
					space=15;
				}
				else
				{
					space=0;
				}

				TFT_ST7789V__rectangle_curve(1+62+(45*(3-loop))+space,64,125,43,0,back_colour);

				Font_Select(BEBAS_43);
				letter1[0]=array_dig2[loop]+48;
				xpos=Letters_Alignments(62+(45*(3-loop))+space,
				106+(45*(3-loop))+space, (uint8_t*)letter1,0);

				TFT_Number(xpos,CALIB_EDT_YPOS,array_dig2[loop],back_colour,text_colour);

				TFT_ST7789V_up_down_arrow_20_arial(79,15+62+(45*(3-loop))+space,0,
				arrow_bg_color,arrow_fill_color);    //UP_ARROW
				TFT_ST7789V_up_down_arrow_20_arial(166,15+62+(45*(3-loop))+space,1,
				arrow_bg_color,arrow_fill_color);  //UP_ARROW

			}
}
