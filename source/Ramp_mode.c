 /*
 * Ramp_mode.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Welcome
 */

#include "LPC55S16.h"
#include "Target_fun.h"
#include <math.h>
#include "Color.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "Keypad.h"
#include "Driver_Timer.h"
#include "FR_CALC.h"
#include "Flowrate_Screen.h"
#include "Driver_I2C.h"
#include <STEPPER.h>
#include "Alarm.h"
#include "Main_screen.h"
#include "sound.h"
#include "Events_log.h"				//25-03
#include "Power_off.h"
#include"FR_CALC.h"
#include "infused_calculate.h"


#define 	RAMP_STEP      10.0
#define 	REF_ML		   0.1

const char* ramp_disp[5]={

		                    "VTBI","Time","Ramp up","Ramp down","Plateau F/R"
							//{,"","",""}
						};
const char* ramp_unit_disp[5]={

								 "mL","hrs","hrs","hrs"

						};

uint8_t Ramp_mode();
void ramp_para_reset(void);
void Ramp_mode_frame(void);
void Ramp_mode_cursor(uint8_t prev_option, uint8_t curr_option);//, uint8_t page, const char* array[2][4]);
void Ramp_mode_opt_Frame(uint8_t start_opt,uint8_t end_opt);

uint32_t Plateau_FR_calculation(void);
void Ramp_Fr_calculation(void);
void Ramp_down_calculation(void);
uint16_t Ramp_time_calculation(uint32_t tot_volume,uint32_t ramp_up_time,uint32_t ramp_down_time,uint32_t plateau_Fr);
uint32_t Ramp_volume_calculation(uint32_t total_time,uint32_t ramp_up_time,uint32_t ramp_down_time,uint32_t plateau_Fr);
extern void Frame_layout(uint8_t start_pos,uint8_t end_pos,uint8_t *header_txt,uint8_t *footer_txt,uint8_t *nav_text);
extern uint8_t ON_Flag;
extern volatile uint8_t normal_run;
extern uint8_t G_infusion_modes;

void ramp_Fr_dispaly(float ramp_fr);

uint16_t plateau_time;
float Ramp_up_vol=0.0;
float Ramp_down_vol=0.0;
float volume_y=0.0;
float Ramp_up_step=0.0;
float Ramp_down_step=0.0;
//uint8_t ramp_stp_inc=0;
uint16_t ramp_up_sum_ml_value[10];
float Ramp_FR[10];

uint16_t ramp_down_sum_ml_value[10];
uint16_t Ramp_down_FR[10];
uint8_t ramp_step=1;
uint16_t plateau_count=0;
uint32_t plateau_FR=0;
uint16_t ramp_down_count=0;
uint16_t ramp_up_count=0;
uint32_t ramp_up_secs=0;
uint32_t ramp_up_hr=0;
uint32_t ramp_up_mins=0;

uint32_t ramp_down_secs=0;
uint32_t ramp_down_hr=0;
uint32_t ramp_down_mins=0;
float plateau_volume=0.0;
extern uint32_t Ramp_pre_FR;

uint32_t total_ramp_min=0;
uint32_t total_ramp_up_min=0;
uint32_t total_ramp_dwn_min=0;

extern uint8_t ramp_flag;
float base_ramp_FR=0.0;

uint8_t FR_change_flg=0;
extern uint8_t keylock;
extern uint8_t G_Brand_back_flag;
extern uint8_t key_pressed;
extern uint8_t sy_size;
extern uint8_t disc;

uint8_t Ramp_mode()
 {
	 uint8_t option_no=0;
	 //uint8_t page_no=0;
	 uint8_t prev_option=0;
	 uint8_t exit_flag=1;
	 uint8_t check_flag=0;
	 uint8_t end_opt_no=4;
	// uint8_t total_option=5;

//	 uint8_t down_opt_arr[2]={3,0};
//	 uint8_t up_opt_arr[2][2]={	{3,3},
//	 								{0,0}
//
//	 								};
//	Ramp_mode_frame();
//	Ramp_mode_opt_Frame(page_no,opt_no,3);
	 POW.power_status_flag=1;

	 while(KEY.value);

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
	 			Ramp_mode_frame();
	 			Ramp_mode_opt_Frame(option_no,end_opt_no);//26-03
	 			//scroll_bar(page_no,option_no,total_option);
	 	   }

	 		if(RAMP.up_flag)
			{
				RAMP_UP(_MILLISEC,RAMP_RPM,RAMP.RPM_control);
			}
			else if(RAMP.down_flag)
			{
				RAMP_DOWN(_MILLISEC,RAMP_RPM);
			}


	 		//TFT_writenumber_float_com((21+110+20+90+20-5),INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)+6,
	 		//		MOT.plateau_rate,WHITE,RED);


	 		switch(KEY.value)
	 		{
	 			case UP_KEY:

	 				if( option_no )
					{
						option_no--;
					}
					else
					{
						option_no=4;

					}


					if(option_no!=prev_option)
					{

					  Ramp_mode_cursor(prev_option,option_no);//,page_no,ramp_disp);

						prev_option = option_no;
					}

					 while(KEY.value);
					 break;

	 			case DOWN_KEY :

//				beep_sound();

	 				if(option_no<4)
	 				{
	 					option_no++;
	 				}
	 				else
	 				{
	 					option_no=0;
	 				}

					if(option_no!=prev_option)
					{

						 Ramp_mode_cursor(prev_option,option_no);//,page_no,ramp_disp);

						prev_option = option_no;

					}

					while(KEY.value);
					break;
	 			case MENU_KEY:
				if(!normal_run)
					{
						//beep_sound();
					}
					if(!keylock)
					{
						key_pressed=30;
						G_Brand_back_flag=0;
						if((!normal_run) && (!RAMP.down_flag))
						{
						Main_Menu_Screen();
						POW.power_status_flag=1;
						}

					}
					else
					{
						Notification(KEY_LOCK);
					}
				while(KEY.value);
				break;

	 			case OK_KEY:
	 				 while(KEY.value);

	 				 if((option_no==0))//&&(!page_no))
	 				 {
	 					check_flag=Flow_rate_screen(&MOT.volume,0,12000,1);
	 					if(!FR_change_flg)
	 					{
	 						if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
	 						{
	 							MOT.plateau_rate=Plateau_FR_calculation();
	 						}
	 					}
						else
						{
							if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.time=Ramp_time_calculation(MOT.volume,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
						}

						if(check_flag<2)
						{
							exit_flag=1;
							POW.power_status_flag=1;
						}


	 				 }
	 				 else if(option_no==1)
	 				 {
	 					check_flag=edit_screen_Time(&MOT.time, TIME_HHMM,9959);
	 					if(!FR_change_flg)
						{
							if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.plateau_rate=Plateau_FR_calculation();
							}
						}
						else
						{
							if(MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.volume=Ramp_volume_calculation(MOT.time,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
						}
	 					if(check_flag<2)
						{
							exit_flag=1;
							POW.power_status_flag=1;
						}
	 				 }
	 				 else if(option_no==2)
	 				 {
	 					check_flag=edit_screen_Time(&MOT.ramp_up, TIME_HHMM,9959);
	 					if(!FR_change_flg)
						{
							if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.plateau_rate=Plateau_FR_calculation();
							}
						}
						else
						{
							if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.volume=Ramp_volume_calculation(MOT.time,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
							else if((MOT.plateau_rate && MOT.volume && (MOT.ramp_up||MOT.ramp_down)))
							{
								MOT.time=Ramp_time_calculation(MOT.volume,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
						}
	 					if(check_flag<2)
						{
							exit_flag=1;
							POW.power_status_flag=1;
						}
	 				 }
	 				else if(option_no==3)
					 {
						check_flag=edit_screen_Time(&MOT.ramp_down, TIME_HHMM,9959);
						if(!FR_change_flg)
						{
							if(MOT.volume && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.plateau_rate=Plateau_FR_calculation();
							}
						}
						else
						{
							if(MOT.plateau_rate && MOT.time && (MOT.ramp_up||MOT.ramp_down))
							{
								MOT.volume=Ramp_volume_calculation(MOT.time,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
							else if((MOT.plateau_rate && MOT.volume && (MOT.ramp_up||MOT.ramp_down)))
							{
								MOT.time=Ramp_time_calculation(MOT.volume,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
							}
						}
						if(check_flag<2)
						{
							exit_flag=1;
							POW.power_status_flag=1;
						}
					 }
	 				else if((option_no==4)) //&&(page_no))
	 				{
	 					FR_change_flg=1;
	 					check_flag=Flow_rate_screen(&MOT.plateau_rate,0,12000,2);

	 					if(!MOT.plateau_rate)
	 					{
	 						FR_change_flg=0;
	 					}

	 					if(FR_change_flg)
						{
	 						//FR_change_flg=0;
	 						if( MOT.time && (MOT.ramp_up||MOT.ramp_down))
								{
									if(MOT.volume)
									{
										MOT.time=Ramp_time_calculation(MOT.volume,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
									}
									else
									{
										MOT.volume=Ramp_volume_calculation(MOT.time,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
									}
								}
	 						else
	 						{
								 if((MOT.plateau_rate && MOT.volume && (MOT.ramp_up||MOT.ramp_down)))
									{
										MOT.time=Ramp_time_calculation(MOT.volume,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate);
									}
	 						}


						}
						if(check_flag<2)
						{
							exit_flag=1;
							POW.power_status_flag=1;
						}
	 				}
	 				Ramp_Fr_calculation();

	 				ramp_flag=0;
	 				ramp_step=1;
	 				ramp_up_count=total_ramp_up_min*60;
	 				ramp_up_count=ramp_up_count/10;
	 				time.hour=0;
					time.minute=0;
					INFUSED.ml=0;
	 				 while(KEY.value);
	 				 break;

	 			case START_STOP_KEY:


	 				if((!MOT.time && !MOT.ramp_down && !MOT.ramp_up && !MOT.volume && !MOT.plateau_rate ) ||

							(MOT.volume && !MOT.time && !MOT.ramp_down && !MOT.ramp_up)||
							(MOT.volume && MOT.time && !MOT.ramp_down && !MOT.ramp_up))
					{
						pop_up(SET_RAMP);
						exit_flag=1;
						POW.power_status_flag=1;
					}

	 				else if((MOT.time==MOT.ramp_up)||(MOT.time==MOT.ramp_down)||(MOT.time<MOT.ramp_up)||(MOT.time<MOT.ramp_down) ||(MOT.time==(MOT.ramp_up+MOT.ramp_down)))
					{
						pop_up(TIME_EXCEED);
						exit_flag=1;
						POW.power_status_flag=1;
					}
	 				else if(MOT.plateau_rate>=12000)
					{
						pop_up(SET_RAMP_FR);
						exit_flag=1;
						POW.power_status_flag=1;
					}


	 				else
	 				{
						if((!normal_run))
						{
							if(MOT.volume && MOT.time && (MOT.ramp_down || MOT.ramp_up) && MOT.plateau_rate &&(!disc) && (sy_size!=0) )
							{
								if(!ramp_flag)
								{
								MR_Value_calculate((uint32_t)base_ramp_FR);

								}
								else
								{
									MR_Value_calculate(Ramp_pre_FR);
								}
								normal_run=1;
							}
							else
							{
								pop_up(SET_RAMP);
								exit_flag=0;
								POW.power_status_flag=1;
							}
						}
						else
						{
//							if(sy_size==0)
//							{
//								pop_up(SET_RAMP);
//								exit_flag=1;
//								POW.power_status_flag=1;
//							}
//							if(disc==1)
//							{
//								pop_up(SET_RAMP);
//								exit_flag=1;
//								POW.power_status_flag=1;
//							}
						}
						exit_flag=0;
	 				}
			//	 				MOT.plateau_rate=Plateau_FR_calculation();
			//	 				Ramp_Fr_calculation();
		//					//Ramp_down_calculation();

	 				while(KEY.value);
	 				break;

	 			case BACK_KEY:
	 				check_flag=0;
	 				exit_flag=0;
	 				while(KEY.value);
	 				break;
	 		}
	 	}
return check_flag;
 }

void ramp_para_reset(void)
{
	MOT.time=0;
	MOT.ramp_down=0;
	MOT.ramp_up=0;
	MOT.volume=0;
	MOT.plateau_rate=0;
}

 /**************************************************************************
  *Function Name :void Target_mode_frame(void)
  *Arguments     :void
  *Return        :void
  *Description	:In this function for VTBI frame
 ***************************************************************************/
 void Ramp_mode_frame(void)
 {

 	//Frame_layout(1,3,(uint8_t*)"VTBI mode",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	// Frame_layout(1,3,(uint8_t*)"Ramp mode",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	 CLRSCR(MAIN_FRAME_COL);
	 Rectangle(4,43,311,0,LIGHT_GRAY);       //TOP LINE
	Rectangle(315,44,0,187,LIGHT_GRAY);     //RIGHT LINE
	Rectangle(3,232+2,311,0,LIGHT_GRAY);		//BOTTOM LINE
	Rectangle(3,44,0,187,LIGHT_GRAY);		//LEFT LINE
	 Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Parameter");
	 //Frame_layout(0,3,,(uint8_t*)"",(uint8_t*)"");
 }

 /**************************************************************************
  *Function Name :void Target_mode_opt_Frame(uint8_t start_opt,uint8_t end_opt)
  *Arguments     :uint8_t start_opt,uint8_t end_opt
  *Return        :void
  *Description	:In this function goto the next parameter in current position
 ***************************************************************************/

 void Ramp_mode_opt_Frame(uint8_t start_opt,uint8_t end_opt)
 {


 	 uint8_t loop_count=0;
     uint32_t back_colour = 0;
     uint32_t text_colour = 0;
  //   uint32_t Target_val_disp[3]={MOT.flowrate,MOT.volume,MOT.time};
     uint32_t Ramp_val_disp[5]=  	{MOT.volume,MOT.time,MOT.ramp_up,MOT.ramp_down,                                                                                                                                                                                                                                                                                                                                                                                    MOT.plateau_rate};
//    		 	 	 	 	 	 	{MOT.plateau_rate*10.0,0,0,0}
//     	 	 	 	 	 	 	 };

     uint16_t xpos=0;
 	char local_buffer[6];
 	uint32_t value = 0;



 	for(loop_count=0;loop_count<=end_opt;loop_count++)
 	{
 		if(loop_count == start_opt)//0==0
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
 		value=Ramp_val_disp[loop_count];

 		Font_Select(ARIAL_BOLD_14);
 		TFT_String(10,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
 				(uint8_t*)ramp_disp[loop_count],back_colour,text_colour);

 		TFT_String(21+110-10+20+7,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
 				(uint8_t*)":",back_colour,text_colour);

 		if((!loop_count)||(loop_count==4))
 		{
 			sprintf(local_buffer,"%.1f",((float)Ramp_val_disp[loop_count]/10.0));    //store the integer variables in to string using sprintf
 			xpos=Right_Alignments((uint8_t*)local_buffer,240);
 			TFT_writenumber_float_1d(xpos-45+15+15+10,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
 					Ramp_val_disp[loop_count],back_colour,text_colour);
 		}
 		else
 		{
 			sprintf(local_buffer,"%04d",Ramp_val_disp[loop_count]);    			   //store the integer variables in to string using sprintf
 			xpos=Right_Alignments((uint8_t*)local_buffer,240);
 			TFT_Time_disp(xpos-45+15+15+10,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
 					Ramp_val_disp[loop_count],back_colour,text_colour);
 		}
// 		Font_Select(ARIAL_BOLD_14);
// 		TFT_String((21+110+20+90+20-5),INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
// 				(uint8_t*)ramp_unit_disp[page][loop_count],back_colour,text_colour);

// 		Font_Select(ARIAL_BOLD_14);
// 						TFT_String(12,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+6,
// 								(uint8_t* )"Start/Stop to RUN",back_colour,RED);


 		Rectangle(8,44+(38*1),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*2),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*3),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*4),301,0,LIGHT_GRAY);//LINE
 	}

 }

 /**************************************************************************
  *Function Name :void Target_mode_cursor(uint8_t prev_option, uint8_t curr_option)
  *Arguments     :uint8_t prev_option, uint8_t curr_option
  *Return        :void
  *Description	:In this function goto the next parameter in current position
 ***************************************************************************/

 void Ramp_mode_cursor(uint8_t prev_option, uint8_t curr_option) //, uint8_t page, const char* array[2][4])
 {

 	uint8_t loop = 0;
 	uint8_t opt_no  = 0;
 	uint32_t back_colour = 0;
 	uint32_t text_colour = 0;
    uint32_t Ramp_val_disp[5]={MOT.volume,MOT.time,MOT.ramp_up,MOT.ramp_down,MOT.plateau_rate};
 	uint16_t xpos=0;
 	char local_buffer[6];

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

 		TFT_String(10,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
 				(uint8_t*)ramp_disp[opt_no],back_colour,text_colour);

 		TFT_String(21+110-10+20+7,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
 			(uint8_t*)":",back_colour,text_colour);



			if((opt_no==0)||(opt_no==4))//||((page)&&(opt_no==0)))
			{
				sprintf(local_buffer,"%.1f",((float)Ramp_val_disp[opt_no]/10.0));    //store the integer variables in to string using sprintf
				xpos=Right_Alignments((uint8_t*)local_buffer,240);    //(uint8_t*)letter
				TFT_writenumber_float_1d(xpos-45+15+15+10,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,
						Ramp_val_disp[opt_no],back_colour,text_colour);
			}
			else
			{
				sprintf(local_buffer,"%04d",Ramp_val_disp[opt_no]);    //store the integer variables in to string using sprintf
				xpos=Right_Alignments((uint8_t*)local_buffer,240);
				TFT_Time_disp(xpos-45+15+15+10,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,
						Ramp_val_disp[opt_no],back_colour,text_colour);
			}



//			if(MOT.plateau_rate>=12000)
//				 {
//					 //plareau_FR=12000;
//					 TFT_String(21+110-10+20+7+50,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
//										(uint8_t*)"--",MAIN_FRAME_COL,LIGHT_GRAY);
//
//				 }
//				 else
//				 {
//					 TFT_String(21+110-10+20+7+50,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
//												(uint8_t*)"--",MAIN_FRAME_COL,MAIN_FRAME_COL);
//				 }

 		Font_Select(ARIAL_BOLD_14);
// 		TFT_String((21+110+20+90+20-5),INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
// 		(uint8_t*)ramp_unit_disp[page][opt_no],back_colour,text_colour);

 		Rectangle(8,44+(38*1),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*2),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*3),301,0,LIGHT_GRAY);//LINE
 		Rectangle(8,44+(38*4),301,0,LIGHT_GRAY);//LINE
 	}
 }


uint32_t Plateau_FR_calculation(void)
{
	float temp_plateau_fr=0.0;
	uint32_t temp_total_hr=0;
	uint32_t temp_total_min=0;
	uint32_t plareau_FR=0;




	temp_total_hr=MOT.time/100;
	temp_total_min=MOT.time%100;
	total_ramp_min=(temp_total_hr*60)+temp_total_min;

	ramp_up_hr=(MOT.ramp_up / 100) ;   //hr
	ramp_up_mins=(MOT.ramp_up % 100) ; 	//mins
	total_ramp_up_min=(ramp_up_hr*60)+ramp_up_mins;//ramp_up_secs*60;

	ramp_down_hr=(MOT.ramp_down / 100) ;   //hr
	ramp_down_mins=(MOT.ramp_down % 100) ; 	//mins
	total_ramp_dwn_min=(ramp_down_hr*60)+ramp_down_mins;//ramp_up_secs*60;



	//plateau_time=MOT.time-(MOT.ramp_up+MOT.ramp_down);

	plateau_time=total_ramp_min-(total_ramp_up_min+total_ramp_dwn_min);

	plateau_count=plateau_time*60;
//	TFT_Number(250,100,plateau_count,WHITE,RED);
	volume_y=((float)(MOT.volume/10.0)/((float)plateau_time+((float)total_ramp_up_min/2.0)+((float)total_ramp_dwn_min/2.0)));

	//volume_y= volume_y ;///10;
	plateau_volume=(float)plateau_time*volume_y;

	temp_plateau_fr=(volume_y*60.0)*10;

	 plareau_FR=(uint32_t)temp_plateau_fr;

//	 if(plareau_FR>=12000)
//	 {
//		 //plareau_FR=12000;
//		 TFT_String(45+15+15+10,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
//		  		 			(uint8_t*)"--",MAIN_FRAME_COL,LIGHT_GRAY);
//
//	 }
//	 else
//	 {
//		 TFT_String(45+15+15+10,INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+7,
//				  		 			(uint8_t*)"--",MAIN_FRAME_COL,MAIN_FRAME_COL);
//	 }



	//MOT.flowrate=MOT.plateau_rate;
	return plareau_FR;

}

uint16_t Ramp_time_calculation(uint32_t tot_volume,uint32_t ramp_up_time,uint32_t ramp_down_time,uint32_t plateau_Fr)
{
	float height_tre=0.0;
	float ramp_up_vol=0.0;
	float ramp_down_vol=0.0;
	float ramp_add_time=0.0;
	float cal_time=0.0;
	uint16_t time_conv=0;


	height_tre=(((float)plateau_Fr/10.0)/60.0);

	ramp_up_vol=0.5*height_tre*ramp_up_time;

	ramp_down_vol=0.5*height_tre*ramp_down_vol;

	ramp_add_time=ramp_up_vol+ramp_down_vol;

	cal_time=((float)tot_volume/10.0)-ramp_add_time;

	cal_time=cal_time/height_tre;

	time_conv=(uint32_t)cal_time+ramp_up_time+ramp_down_time;

	time_conv=(((time_conv/60)*100)+(time_conv%60));

//
//	TFT_writenumber_float_com((21+110+20+90+20-5),INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,
//						height_tre,YELLOW,RED);
//
//				TFT_writenumber_float_com((21+110+20+90+20-5),INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
//						ramp_up_vol,YELLOW,RED);
//		 		TFT_Number((21+110+20+90+20-5),INITIAL_TEXT_POS+(3*VER_TEXT_SPACE)+6,
//		 				ramp_down_vol,RED,YELLOW);
//		 		TFT_writenumber_float_com(10,INITIAL_TEXT_POS+(0*VER_TEXT_SPACE)+6,
//		 				Ramp_up_vol,YELLOW,RED);
//		 		TFT_writenumber_float_com(10,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
//		 				ramp_add_time,YELLOW,RED);
//		 		TFT_writenumber_float_com(10,INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)+6,
//		 				time_conv,YELLOW,RED);
//		 		delay_secs(3);

return time_conv;


}

void Ramp_Fr_calculation(void)
{


	uint8_t ramp_stp_inc=0;
	for(ramp_stp_inc=1;ramp_stp_inc<=(uint8_t)RAMP_STEP;ramp_stp_inc++)
	{
		base_ramp_FR=MOT.plateau_rate/(RAMP_STEP+1.0);

		//Ramp_FR[ramp_stp_inc-1]=base_ramp_FR*ramp_stp_inc;
		Ramp_FR[ramp_stp_inc-1]=(base_ramp_FR*ramp_stp_inc)/10.0;
//		TFT_writenumber_float_com(15,180,Ramp_FR[ramp_stp_inc-1],BLACK,YELLOW);
//		TFT_writenumber_float_com(15,150,base_ramp_FR,BLACK,YELLOW);
//				 delay_secs(1);
	}


}

uint32_t Ramp_volume_calculation(uint32_t total_time,uint32_t ramp_up_time,uint32_t ramp_down_time,uint32_t plateau_Fr)
{
	uint32_t plateau_time=0;
	uint32_t ramp_vol=0;
	float vol_y=0.0;
	float ramp_up_vol=0.0;
	float ramp_down_vol=0.0;
	float plateau_vol=0.0;

	plateau_time=total_time-(ramp_up_time+ramp_down_time);

	vol_y=(float)plateau_Fr/60;

	ramp_up_vol=0.5*vol_y*ramp_up_time;

	ramp_down_vol=0.5*vol_y*ramp_down_vol;

	plateau_vol=vol_y*(uint32_t)plateau_time;

	ramp_vol=(uint32_t)((uint32_t)ramp_up_vol+(uint32_t)ramp_down_vol+(uint32_t)plateau_vol);


	return ramp_vol;

}
void ramp_Fr_dispaly(float ramp_fr)
{
	char FR_buffer[6];
	uint16_t xpos=0;
	float flowrate_view = 0;
	static float pre_Ramp_FR=0.0;

	if((pre_Ramp_FR!=ramp_fr)||(POW.power_status_flag))
	{

		flowrate_view = pre_Ramp_FR;//400


		Font_Select(BEBAS_60);

		 sprintf(FR_buffer,"%.1f",flowrate_view);
		 xpos=Flow_rate_Alignments((uint8_t*)FR_buffer);
		// xpos=50;
		 xpos=TFT_writenumber_float_com(xpos+2,50,flowrate_view,LIGHT_GRAY_5,LIGHT_GRAY_5);
//		 xpos=TFT_String(xpos+3,50,(uint8_t *)".",LIGHT_GRAY_5,LIGHT_GRAY_5);		//2-04
//
//		 TFT_Number(xpos+3,50,ramp_fr%10,LIGHT_GRAY_5,LIGHT_GRAY_5);

		 flowrate_view = ramp_fr;

		 sprintf(FR_buffer,"%.1f",flowrate_view);
		 xpos=Flow_rate_Alignments((uint8_t*)FR_buffer);
		 //xpos=50;
		 xpos=TFT_writenumber_float_com(xpos+2,50,flowrate_view,LIGHT_GRAY_5,DARK_TEAL);
//		 xpos=TFT_String(xpos+3,50,(uint8_t *)".",LIGHT_GRAY_5,DARK_TEAL);		//2-04
//		 TFT_Number(xpos+3,50,ramp_fr%10,LIGHT_GRAY_5,DARK_TEAL);
		 pre_Ramp_FR=ramp_fr;
	}
}
