#include <Alarm.h>
#include "Bolus_Header.h"
#include <Color.h>
#include <Dosage_mode.h>
#include <Driver_Timer.h>
#include <Dynamic_Pressure.h>
#include <Events_log.h>
#include <Flowrate_Screen.h>
#include <FR_CALC.h>
#include <infused_calculate.h>
#include <Keypad.h>
#include <Linear_Header.h>
#include <Linear_pot.h>
#include <Main_Menu.h>
#include <Main_screen.h>
#include <Motor.h>
#include <Occlusion.h>
#include <Power_off.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Shapes.h>
#include <STEPPER.h>
#include <Target_fun.h>
#include <TFT_Commands.h>
#include <TFT_String.h>
#include "Driver_ADC.h"
#include "Power_off.h"
#include "Drug_List.h"
#include "infused_calculate.h"
#include "UART.h"

uint8_t key_check=1;
uint16_t G_key_failure=0;
uint16_t alarm_longpress;
uint8_t G_Brand_back_flag=0;
extern uint16_t first_lmt[7];
extern uint8_t G_syringe_size_flag;
uint8_t G_Complete_reset=0;
extern uint32_t adc;
extern uint8_t syr_size;
//extern char local_buffer[10];
//extern uint16_t Sample_value;
//extern uint8_t run;
//extern uint8_t batt_high_flag;
//extern uint16_t Pump_idle;
uint8_t pre_flowrate=10;
uint16_t PSI_temp_value=0;
uint16_t example_var=0;
//uint16_t PSI_temp_value=0;
uint8_t home_flag=0;
uint8_t PSI_Value=0;
uint16_t p_xpos=0;
extern char p_local_buffer[10];
extern uint8_t warn_disp_flag;
//NEWLY ADDED VARIABLES//

uint32_t linear_pot_pulse=0;
uint8_t G_Menu_Brand_flag=0;//17.09.24
uint32_t colour[TOTAL_LED+1];
extern uint32_t  G_minval[7];//old pot adc value
uint16_t auto_lock=0;
uint8_t keylock=0;
uint16_t key_longpress;
uint8_t key_pressed=30;
uint8_t g_size_vol;
uint8_t  longpress=0;
volatile uint8_t waitingtime_1;
uint8_t quick_run=0;
uint8_t G_mode_exit_flag;
uint8_t  mode_longpress=0;
uint8_t motor_on_flag=0;
//uint16_t event1_no;			//25-03
uint16_t temp_ADC_values=0;
uint32_t FR_1=0;
uint8_t Bolus_status_flag=0;
uint8_t Bolus_reset_flag=0;
uint8_t press_run_flag=0;
uint16_t Pump_idle_flag=0;
uint8_t press_clear_flag=0;
uint8_t G_Bolus_target_flag=0;
uint8_t G_Bolus_warning_flag=0;
uint32_t check=0;
uint32_t dummy_variable=285164096;
uint8_t G_Long_press_back=0;
uint8_t G_main_screen_flag=0;
uint8_t low_battery_flag=0;
uint8_t low_battery_erase_flag=0;
uint8_t Led_Alarm_flag=0;
uint8_t  brightness;

uint8_t syr_status_chk=0;

volatile uint16_t bit_count;
volatile uint16_t LED_count;

extern char local_buffer[10];
extern uint16_t Sample_value;
extern uint8_t run;
extern uint8_t Led_Alarm_flag;
extern uint16_t m_count1;
extern uint8_t G_bottom_scr_flag=0;
extern uint8_t G_infusion_modes;
uint8_t alarm_flag;
extern uint16_t total_events;   //29-03
extern uint16_t event_id;		//25-03
//extern uint8_t ssize;
extern uint8_t sy_size;
extern uint8_t G_correct_move;
extern uint8_t G_wrong_move;
extern uint16_t G_Remaining_volume;
extern uint8_t pulse_counter;

void auto_keylock(uint8_t auto_level);
void syringe_disp_status(uint8_t screen);
void Disc_failure_condition(void);
void Calibration_error_condition(void);

extern uint8_t G_lock_time;
extern uint8_t brand_select_no;
extern uint8_t syr_brand;
volatile uint8_t normal_run;
extern uint8_t brand_width_disp;

extern uint16_t linear_temp_adc;
extern uint16_t temp_ADC_values_1;
uint16_t temp_ADC_values_2=0;
uint16_t temp_ADC_values_3=0;
uint16_t general_count=0;
extern uint16_t temp_ADC_values_1;

extern  char *brand_list[17];
extern void Running_symbol(uint8_t X_start, uint8_t Y_start,uint32_t col);
extern uint8_t TFT_ST7789V_Left_Arrow_22pt(uint16_t xpos, uint16_t ypos,  uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
extern void infused_data(uint8_t screen,uint8_t r_mode);
extern void plunger_based_o_ml_max_ml_calculate();
extern float syringe_nearempty_calculation(uint16_t flowarte);
extern void plunger_alarm_check();

extern uint16_t m_count;
extern uint32_t MR_VALUE;
extern uint8_t Target_mode_setting(void);
uint8_t alarm_flag;
extern uint8_t ON_Flag;
extern void infuse_status(void);
extern void infuse_clr();
extern uint16_t auto_lock;
extern uint8_t G_infusion_mode_opt;
extern uint32_t G_loaded_adc_val[9];
extern uint8_t siphonic_time;
extern uint8_t G_occl_unit;

extern uint8_t prev_data_flag;  //ZOOZ_1
extern uint16_t m_count1_1;
extern uint16_t m_count1;
extern uint8_t end_flag;
extern uint16_t Pump_idle;
uint8_t home_return_flag;

extern clk time;//ZOOZ_1
extern uint8_t G_warn_flag;//28-03
void  read_last_data(uint8_t mode);
extern uint8_t dose_rate_mode(void);
//extern void //// // // beep_sound();
extern uint16_t G_con_value;
//extern uint8_t sy_size;
extern uint16_t press_run_alarm;
extern uint8_t disc;
//extern uint8_t sy_size;
extern uint8_t drv;
extern char disp_para[15];    //ZOOZ_1
extern uint8_t dose_option_no;
extern uint32_t G_pressure_value;
extern uint16_t array_date[50],array_month[50],array_year[50];
extern uint16_t array_hour[50],array_minute[50],array_event_no[50];
extern clk sec_to_time_format(uint32_t sec);
extern uint8_t TFT_ST7789V_keylock4_symbol_16pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
//extern Occlusion Occl;
//extern uint8_t sy_size;
extern uint8_t release;
extern uint8_t G_mode_switch_flag;
extern uint8_t G_mode_sw_flag;
extern uint8_t near_to_target_flag;
extern uint8_t cuts_sec;
extern uint8_t plunger_distrubed;
extern uint16_t adc_rd;


extern uint16_t ramp_up_sum_ml_value[10];
extern float Ramp_FR[10];
extern uint8_t ramp_step;
extern float Ramp_up_step;
extern float Ramp_down_step;
//extern uint8_t ramp_stp_inc;
extern uint32_t plateau_FR;
extern uint16_t ramp_down_sum_ml_value[10];
extern uint16_t Ramp_down_FR[10];
extern uint16_t ramp_down_count;
extern uint16_t ramp_up_count;
extern uint16_t plateau_count;
extern uint32_t Ramp_pre_FR;
extern uint8_t Ramp_mode();
uint8_t ramp_flag=0;
extern uint32_t total_ramp_min;
extern uint32_t total_ramp_up_min;
extern uint32_t total_ramp_dwn_min;
extern void ramp_para_reset(void);
extern uint16_t debug_temp;
extern void ramp_Fr_dispaly(float ramp_fr);
void pressure_display(uint8_t bol_run,uint8_t bol_reset_flg);
extern uint8_t dose_blink_count;
extern uint8_t sec_count;
extern uint8_t G_add;

uint8_t war_status[5];
extern uint32_t G_kvo;
extern uint8_t G_standby;
void warning_check_fun();
//uint16_t example_var=0;
//uint8_t home_flag=0;
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
uint8_t Home_screen_functionality(uint8_t mode)
{

	uint16_t PSI_temp_value=0;
	uint8_t home_exit_flag=1;
	uint8_t exit_flag=0;
	uint8_t loop_count=0;
	uint8_t M_nav_flag=1;//common_bug_33
	uint8_t confirm_value=0;
	uint8_t compound_value=0;
	uint16_t battery_checker=0;
	uint16_t Battery_data=0;
	uint16_t xpos=0;
	uint16_t PSI_Value=0;
	uint32_t temp_FR=0;
	uint8_t pre_mode=0;


	float pre_Ramp_FR[10];
	POW.power_status_flag=0;
	MOT.flowrate=0;
	RAMP.on_off=1;



	if((!G_mode_switch_flag)&&(prev_data_flag))
	{
		read_last_data(mode);
	}
	else
	{
		if(mode==0)
		{
			MOT.flowrate=1;
			M_nav_flag = Flow_rate_screen(&MOT.flowrate,1,12000,2); //common_bug_33
		}

		else if(mode==1)
		{
			MOT.flowrate=0;
			MOT.volume=0;
			MOT.time=0;
			M_nav_flag = Target_mode_setting();						//common_bug_33
		}

		else if(mode==2)
		{
			M_nav_flag =dose_rate_mode();							//common_bug_33
		}
		else if(mode==3)
		{
			ramp_para_reset();
			M_nav_flag=Ramp_mode();
			G_mode_switch_flag=1;
		}

		if(G_mode_switch_flag)
		{
			M_nav_flag=1;
			G_mode_switch_flag=0;
		}
		else
		{
			if(!M_nav_flag)
				{
					if((G_infusion_mode_opt==4) &&  (!prev_data_flag))//common_bug_1
					{
					  G_infusion_modes=4;
					}
					exit_flag=2;
				}

		}
	}


//	if(!G_mode_switch_flag)
//	{
//		if(prev_data_flag)
//		{
//			read_last_data(mode);
//			M_nav_flag=1;
//		}
//	}
//
//	if((!prev_data_flag)||(G_mode_switch_flag))
//	{
//
//		if(mode==0)
//		{
//			MOT.flowrate=1;
//			M_nav_flag = Flow_rate_screen(&MOT.flowrate,1,12000,2); //common_bug_33
//		}
//
//		else if(mode==1)
//		{
//			MOT.flowrate=0;
//			MOT.volume=0;
//			MOT.time=0;
//			M_nav_flag = Target_mode_setting();						//common_bug_33
//		}
//
//		else if(mode==2)
//		{
//			M_nav_flag =dose_rate_mode();							//common_bug_33
//		}
//
//		if(M_nav_flag)
//		{
//			G_mode_switch_flag =0;
//		}
//
//		if(G_mode_switch_flag)
//		{
//			M_nav_flag = 0;
//			G_mode_switch_flag =0;
//			exit_flag=2;
//		}
//
//		if(G_mode_sw_flag)
//		{
//			exit_flag=2;
//		}
//
//		if(!M_nav_flag)
//		{
//			if((G_infusion_mode_opt==3) &&  (!prev_data_flag))//common_bug_1
//			{
//			  G_infusion_modes=3;
//			}
//			exit_flag=2;
//		}
//	}
	POW.power_status_flag=1;
	if(M_nav_flag)
	{
		prev_data_flag=0;
		G_infusion_modes =mode;
		G_Complete_reset=mode;
		//27-03
	//	siphonic_time=30;
		exit_flag=0;

/*****************DEFAULT SYR ADC VALUE******************/

	while(home_exit_flag)
	{
	// linear_pot( adc_rd);
	   Power_off();
       if(!ON_Flag) 	//MAIN.main_power_status_flag == 1 )
       {
    	  break;//23-03
       }

       else if(POW.power_status_flag)
	   {
    	   // G_main_screen_flag=1;
    	   home_flag=1;
		   home_return_flag=1;
		   near_to_target_flag=0;
		   siphonic_time=30;
		   G_syringe_size_flag=0;
		   POW.power_status_flag=0;
		   main_screen_frame( MOT.flowrate,G_infusion_modes);
		   syringe_size_display();
		   speedometer_shape();
		   PSI_temp_value=0;
		   speedometer3(0/10,1);
		   POW.power_status_flag=0;
//		    if(disc==0)           //REW_UP(start/stop case)
//			{
//			  Alarm(NOT_ENGAGE);
//			  TFT_String(50,100,"1",RED,YELLOW);
//			}
		   while(KEY.value);
	   }

       if(!sec_count)
       {
    	   if((!BOL.run_mode)&&(!Bolus_status_flag))
    	   {
    		   warning_check_fun();
    	   }
    	   sec_count=2;
       }


//      Battery_data=ADC_read(ADC_CH_BATTERY);
//	   Font_Select(ARIAL_14);
//	   TFT_Number(0,140,Battery_data,YELLOW,RED);


	   if(GPIO->PIN[1] & (1<<27))
	   {
		   compound_value=1;
	   }

	   else
	   {
		   compound_value=0;
	   }

	   if(GPIO->PIN[1] & (1<<28))
	   {
		   confirm_value=1;
	   }

	   else
	   {
		   confirm_value=0;
	   }


	   plunger_alarm_check();
       Motor_struck_alarm();   //REW_UP
       syringe_disp_status(0);

     ///////////////////////////////////////

	   Font_Select(ARIAL_14);


	   	   if(mode==2)
		   {

			   if((!warn_disp_flag))
			   {
				   dose_rate_para(0,3);
			   }

		   }


	 if((!BOL.run_mode) && (!Bolus_status_flag))  //28-03
	 {
		 if((!warn_disp_flag)&&(!near_to_target_flag))
		 {
			// if(mode!=2)
			 {
				 infused_data(0,mode);                   //Display the Infused data in the Main screen
			 }
		 }

		 if((!BOL.run_mode))
		{
			 remaining_volume_in_syringe();
		}

	   if(normal_run)
	   	   {
	   		   TFT_String(160,12,(uint8_t*)"ON  ",LIGHT_TURQUI,GREEN);
	   	   }
	   	   else
	   	   {
	   		   TFT_String(160,12,(uint8_t*)"OFF",LIGHT_TURQUI,RED);
	   	   }
	   pressure_display(BOL.run_mode,Bolus_status_flag);
	 }

	if(!KEY.value)
	{
	  longpress=0;
	}
		 auto_keylock(G_lock_time);
		 ADC_to_PSI_conversion();  //REW_UP

		if(normal_run ) //|| (BOL.mode))
		{
			if(!sec_count)
			{
			   if((!BOL.run_mode)&&(!Bolus_status_flag))
			   {
				   warning_check_fun();
			   }
			   sec_count=2;
			}

			if((!BOL.run_mode) && (!Bolus_status_flag))
			{
				syringe_position_check();//REW_UP
			}



			  if(BOL.mode<=1)
			  {
				  Occlusion_Status();
				  infuse_status();
			  }

			  if(mode==3)
			  {
				  Font_Select(ARIAL_14);


				  if(!ramp_flag)
				  {
					  if(MOT.ramp_up)
					  {
						  if(!ramp_up_count)
						  {
							  if(ramp_step<10)
							  {

								    ramp_Fr_dispaly(Ramp_FR[ramp_step]);
									ramp_shap(mode);

									MR_Value_calculate(Ramp_FR[ramp_step]*10.0);
									ramp_step++;
									ramp_up_count=total_ramp_up_min*60;
									ramp_up_count=ramp_up_count/10;

							 }

							  else
							  {
								  ramp_flag=1;
								 // Font_Select(ARIAL_14);
								  //TFT_Number(15,180,MOT.plateau_rate,YELLOW,RED);
								  ramp_Fr_dispaly(MOT.plateau_rate/10);
								  ramp_shap(mode);
								  MR_Value_calculate(MOT.plateau_rate);
							  }
						  }

					  }
					  else
					  {
						  ramp_flag=1;
						  ramp_Fr_dispaly(MOT.plateau_rate/10.0);

						  MR_Value_calculate((uint32_t)MOT.plateau_rate);
						  ramp_shap(mode);
					  }
				  }

				  else if(ramp_flag==1)
				  {


					  if(!plateau_count)
						{
						  if(MOT.ramp_down)
						  {
						  ramp_flag=2;
						  ramp_shap(mode);
						  ramp_step=9;
						  ramp_Fr_dispaly(Ramp_FR[ramp_step]);
						  MR_Value_calculate(Ramp_FR[ramp_step]*10.0);
						  ramp_down_count=total_ramp_dwn_min*60;//
						  ramp_down_count=ramp_down_count/10;
						  }
						  else
						  {
							  Timer_disable();
							  ramp_flag=0;
							  Alarm(TAR_TIME_REACH);
							  ramp_para_reset();
						  }
						}

				  }

				  else if(ramp_flag==2)
				  {
					  if(!ramp_down_count)
					  {
						  if(ramp_step)
						  {
							  ramp_step--;
							  ramp_Fr_dispaly(Ramp_FR[ramp_step]);
							  MR_Value_calculate(Ramp_FR[ramp_step]*10.0);
							  ramp_down_count=total_ramp_dwn_min*60;
							  ramp_down_count=ramp_down_count/10;



						  }
						  else
						  {
							  Timer_disable();
							  ramp_flag=0;
							  Alarm(TAR_TIME_REACH);
							  ramp_para_reset();
						  }
					  }

				  }

				  else
				  {
					  Timer_disable();
					  Alarm(TAR_TIME_REACH);
					  ramp_flag=0;
					  ramp_para_reset();
				  }
			  }
		}

	    else
		{
	    	if((!BOL.run_mode ||normal_run) && disc==1)
			{
	    		//Alarm(NOT_ENGAGE);
			}

//	    	 	 if(Pump_idle_flag)
//	    		 {
//	    			Pump_idle=15;
//	    		    Pump_idle_flag=0;
////	    		    delay_secs(2);
//	    		    notif_Scr_Erase();
//
//	    		 }
////
//				 if((!Pump_idle))
//				 {
//					 delay_secs(1);
//					 Notification(PUMP_IDLE);
//
////					 Warning_screen((uint8_t*)"   Pump Idle",0);
//					 WARN_SOUND(3,1,3);
//					 Pump_idle_flag=1;
//				 }

//				 if((sy_size!=0) && ((disc==1) && (MOT.flowrate>0)))

//	    	Font_Select(ARIAL_14);
//	    	TFT_Number(50,70,press_run_alarm,YELLOW,RED);
//
//	    	Font_Select(ARIAL_14);
//	    	TFT_Number(50,90,sy_size,YELLOW,RED);
//
//	    	TFT_Number(50,110,disc,YELLOW,RED);
//
//	    	TFT_Number(50,130,MOT.flowrate,YELLOW,RED);


//	    	if(((normal_run==0) && (MOT.flowrate>0)) && (sy_size!=0)&&(!BOL.run_mode))//&&(home_flag))  //
//	    	{
//
//	    		 if(!press_run_flag)    //REW 1
//				 {
//	    			 	press_run_alarm=20;
//						press_run_flag=1;
//						press_clear_flag=0;
//						G_warn_flag=0;
//				 }
//
//				if((!press_run_alarm))// && (!BOL.run_mode))    //REW 1  NEED TO IMPROVE
//				{
//						Warning_screen((uint8_t*)"Press run",0);
//						WARN_SOUND(3,1,3);
//						delay_secs(2);
//						warning_screen_erase(LIGHT_GRAY_5);
//						bottom_para_frame(mode);
//						press_run_flag=0;
//				}
//			}

	    	if(((sy_size!=0) && (!disc)) && (MOT.flowrate>0))
			{

				if(!press_run_flag)
				{
					press_run_alarm=180;
					press_run_flag=1;
					press_clear_flag=0;
				}

				if((!press_run_alarm) && (!BOL.run_mode))
				{
				//  Alarm(PRESS_RUN);
					Warning_screen((uint8_t*)"Press Start",0);
					WARN_SOUND(3,1,3);
					delay_secs(2);
					warning_screen_erase(LIGHT_GRAY_5);
					bottom_para_frame(mode);
					press_run_flag=0;
				}

		    }

			 else
			 {
				 press_run_alarm=180;
				 if(!press_clear_flag)
				 {
					 warning_screen_erase(LIGHT_GRAY_5);
					 press_clear_flag=1;
				 }
			 }




	    }



		 if(mode==1)
		 {
			 if((!BOL.run_mode) && (!Bolus_status_flag))
			 {
				 near_to_target();
			 }
		 }




	 	 	 	 	if(RAMP.up_flag)
					{
						RAMP_UP(_MILLISEC,RAMP_RPM,RAMP.RPM_control);
					}
					else if(RAMP.down_flag)
					{
						RAMP_DOWN(_MILLISEC,RAMP_RPM);
					}

	 	 	 	 	home_exit_flag=Key_functionality(mode);

					if(!home_exit_flag)
					{
					  exit_flag=3;
					}
	}
 }
	return exit_flag;
 }

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint8_t key_main_screen()
{
	uint8_t exit_flag = 1;
	uint8_t fr_flag=0;
    uint8_t loop_count=0;

switch(KEY.value)
{
		case OK_KEY :

		  //// // // // beep_sound();
		   home_flag=0;
		   press_run_alarm=180;

		   Pump_idle=10;

			if(!keylock)
			{
				key_pressed=30;
				MFB_Reset_data();
				G_wrong_move=0;
				Flow_rate_screen(&MOT.flowrate,0,24000,2);
				if(MOT.flowrate)
				{
					if(normal_run)
					{
						MR_Value_calculate(MOT.flowrate);
					}
				}

				else
				{
					normal_run=0;
					Timer_disable();
				}
				POW.power_status_flag=1;//26-03
			}

			else
			{
				Notification(KEY_LOCK);
			}

		while(KEY.value);
		break;


			case START_STOP_KEY:


			Bolus_reset_flag=0;

		    if(!Bolus_reset_flag)
			{
				if ((!keylock)||(normal_run))
	 			{
					key_pressed=30;
					if(!LIN_POT.syr_end_flag)
					{

						if((normal_run==0) && (!Bolus_status_flag))
						{
							if((MOT.flowrate!=0) && (sy_size!=0) &&(!disc))
							{
								normal_run=1;
								press_run_alarm=180;
								Pump_idle=60;
								Font_Select(ARIAL_14);
//								TFT_Number(100,70,Pump_idle,WHITE,RED);
							//	Warning_Scr_Erase();
								plunger_based_o_ml_max_ml_calculate();
								syringe_nearempty_calculation(MOT.flowrate);
								event_no_update(2);
								linear_pot( adc_rd);
								BOL.run_mode=0;
								MFB_Reset_data();
								G_wrong_move=0;
								RAMP.RPM_control=0;    //ZOOZ_1
								DRV8428_ENABLE();
								MR_Value_calculate(MOT.flowrate);
//								Calibration_error_condition();
								  linear_pot( adc_rd);
//								if(GPIO->SET[0]|=(1<<19))
//								{
//									Alarm(RLSW_ERR);
//								}
//								if(disc==0)           //REW_UP(start/stop case)
//								{
//								  Alarm(NOT_ENGAGE);
//								  TFT_String(50,100,"1",RED,YELLOW);
//								}
							}

							else
							{
								if((sy_size==0) &&(!disc))
								{
								Warning_screen((uint8_t*)"No syringe",0);
								G_bottom_scr_flag=1;
								WARN_SOUND(3,1,3);
								delay_secs(2);
								warning_screen_erase(LIGHT_GRAY_5);
								bottom_para_frame(G_infusion_modes);
								}
								if(disc==1)
								{
									Warning_screen((uint8_t*)"Syr Disengage",0);
									WARN_SOUND(3,1,3);
									delay_secs(2);
									warning_screen_erase(LIGHT_GRAY_5);
									bottom_para_frame(G_infusion_modes);
								}
//								else
//								{
//									//if(sy_size>0 && G_bottom_scr_flag==1)
//													{
//										 warning_screen_erase(LIGHT_GRAY_5);
//														bottom_para_frame(0);
//
//														G_bottom_scr_flag=0;
//
//													}
//								}
								if((MOT.flowrate==0) && (sy_size==0))
								{

									WARN_SOUND(2,1,2);
									Notification(SET_FLOWRATE);
									fr_flag=1;
								}


								if( (MOT.flowrate==0) && (fr_flag!=1) )
								{
									Notification(SET_FLOWRATE);
								}

//								else if(ssize==0)
//								{
//									{
//
//										Warning_screen((uint8_t*)"Syringe unavailable",0);
//										WARN_SOUND(3,1,3);
//										delay_secs(2);
////										G_warn_flag=1;
//										Warning_Scr_Erase();
//										//delay_secs(2);
//									}
//
//								}

							}

						 }

					else
					{
						if(!Bolus_reset_flag)
						{
							sec_to_time_format(INFUSED.sec );
							BOL.run_mode=0;
					//		event_no_update(4);
					//		siphonic_time=30;
							normal_run=0;
							RAMP.down_flag=0;
					    	press_run_alarm=180;
							Pump_idle=10;

							if( (RAMP.RPM_control > 50.0) && (RAMP.on_off == 1) ) // RPM ramp on off flag
							{
							   RAMP.down_flag=1;
						//	   RAMP.dec=RAMP.RPM_control/_RPM;  /*CHECK*/
							}

							else
							{
							//	DRV8428_DISABLE();
								Timer_disable();
							}
							RAMP.up_flag=0;
							Reset_linear_pot();
						//	RAMP.inc=0;  /*CHECK*/
						}

							if(!Bolus_reset_flag)
							{
								//Rectangle(10,10,60,30,MAIN_FRAME_COL);
							}
				 }

				}
					else
					{
						//WARN_SOUND(3,1,3);
					}
				}

				else
				{
					Notification(KEY_LOCK);
				}
	}
			while(KEY.value);
		    break;

}
return exit_flag;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint8_t key_main_screen_VTBI()
{
	uint8_t exit_flag=1;
	uint32_t Max_limit=0;



	switch(KEY.value)
	{


		case OK_KEY :

		home_flag=0;
		// // // beep_sound();
		if(!keylock)
		{
			key_pressed=30;
			MFB_Reset_data();
			G_wrong_move=0;
			if((!normal_run) && (!RAMP.down_flag))        //12-06-2024
			{
				Target_mode_setting();
			}

			else
			{
				Max_limit=volume_cal(MOT.time , 12000);
				if(Max_limit>20000)
				{
					Max_limit=20000;
				}

				if(!RAMP.down_flag)
				{
				Flow_rate_screen(&MOT.volume,0,Max_limit,1);
				}


				if(MOT.volume && (MOT.flowrate || MOT.time))
				{
					if(MOT.flowrate)
					{
						VTBI_check(3);
					}
					else
					{
						VTBI_check(1);
					}
				}
			}

			if(normal_run)
			{
				MR_Value_calculate(MOT.flowrate);
			}


				POW.power_status_flag=1;	//26-03
			}
			else
			{
				Notification(KEY_LOCK);     //12-06-2024
			}

		while(KEY.value);
		break;


		case START_STOP_KEY:
		// // // beep_sound();
		if((!keylock)||(normal_run))//29-03
		{
			key_pressed=30;
			if(INFUSED.ml==MOT.volume)
			{
//				Warning_screen((uint8_t *)"Target reached",0);
				WARN_SOUND(3,1,3);
				pop_up(TAR_REACH_1);
				POW.power_status_flag=1;

			}

			else
			{
				if(normal_run==0)
				{
					if(MOT.flowrate!=0)
					{
						 if((sy_size!=0) &&(!disc))
							{
							    normal_run=1;
								DRV8428_ENABLE();
						//		event_no_update(3);
								MR_Value_calculate(MOT.flowrate);
								MFB_Reset_data();
								G_wrong_move=0;
					//			plunger_based_o_ml_max_ml_calculate();
								syringe_nearempty_calculation(MOT.flowrate);
//								if(disc==0)           //REW_UP(start/stop case)
//								{
//								  Alarm(NOT_ENGAGE);
//								}
//							   if(drv==1)
//							   {
//								   //Alarm(DRV_ERR);
//							   }
							}
						 //Alarm(SYR_NOT_ENGAGE);
						 	 else
						 	 {
//								Warning_screen((uint8_t*)"Syringe unavailable",0);
						 		if((sy_size==0))// &&(!disc))
									{
									Warning_screen((uint8_t*)"No syringe",0);
									G_bottom_scr_flag=1;
									WARN_SOUND(3,1,3);
									delay_secs(2);
									warning_screen_erase(LIGHT_GRAY_5);
									bottom_para_frame(G_infusion_modes);
									}
									if(disc==1)
									{
										Warning_screen((uint8_t*)"Syr Disengage",0);
										WARN_SOUND(3,1,3);
										delay_secs(2);
										warning_screen_erase(LIGHT_GRAY_5);
										bottom_para_frame(G_infusion_modes);
									}
								WARN_SOUND(3,1,3);


						 	 }

					}

				else
				{

					WARN_SOUND(2,1,2);
					Notification(SET_FLOWRATE);

				}
				}

				else
				{
					normal_run=0;
					Timer_disable();
					RAMP.up_flag=0;
				}

				//Rectangle(10,10,60,30,MAIN_FRAME_COL);
			}
		}

		else
		{
			Notification(KEY_LOCK);
		}
		while(KEY.value);
		break;



	}
return exit_flag;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint8_t key_fun_doserate_mode(void)
{
	uint8_t check_flag=0;
	uint8_t exit_flag=1;




	switch(KEY.value)
	{


		case OK_KEY :
		// // // beep_sound();
		if(!keylock)
		{
			key_pressed=30;
			MFB.twenty_cuts_count=0;
			MFB.cuts_time_diff=0;
			MFB.cuts_time_diff_cnt=0;
			G_wrong_move=0;

			if(normal_run)
			{
				check_flag=Dual_Deci_val_edit_scr(&dose.drug_dos,1,100000,5);
					if(check_flag)
					{
						MOT.flowrate=dose.flow_rate/10;
						MR_Value_calculate(MOT.flowrate);

					}
			}
			else
			{
				overview_screen();
			}
//
//			if(!normal_run)
//			{
//				overview_screen();
//			}
//			else
//			{
//				home_flag=0;
////				check_flag=Double_Decimal_FR_screen(&dose.drug_dos,100000,5);
//				if(dose.flow_rate)
//				{
//					if(normal_run)
//					{
//						MR_Value_calculate(dose.flow_rate);//dose.flow_rate);
//					}
//				}
//
//				else
//				{
//					normal_run=0;
//					Timer_disable();
//				}


			POW.power_status_flag=1;//26-03

		}

		else
		{
			Notification(KEY_LOCK);
		}
		while(KEY.value);
		break;

		case START_STOP_KEY:


			event_no_update(3);
		// // // beep_sound();
		if(!keylock)
		{
			key_pressed=30;

			if(!normal_run)
			{
				if(MOT.flowrate)
				{
					if((sy_size!=0) &&(!disc))
					{
						normal_run=1;
					    press_run_alarm=180;
						Pump_idle=10;
						DRV8428_ENABLE();
		//				event_no_update(3);
						MOT.flowrate=dose.flow_rate/10;
						MR_Value_calculate(MOT.flowrate);
						plunger_based_o_ml_max_ml_calculate();
						syringe_nearempty_calculation(MOT.flowrate);
						MFB_Reset_data();
						G_wrong_move=0;
//						if(disc==0)
//						{
//						  Alarm(NOT_ENGAGE);
//						}
					}

					else
					{
//						Warning_screen((uint8_t*)"Syringe unavailable",0);
						WARN_SOUND(3,1,3);
						if(sy_size==0)
								{
								Warning_screen((uint8_t*)"No syringe",0);
								G_bottom_scr_flag=1;
								WARN_SOUND(3,1,3);
								delay_secs(2);
								//warning_screen_erase(LIGHT_GRAY_5);
								bottom_para_frame(G_infusion_modes);
								//dose_rate_para(0,3);
								}
								if(disc==1)
								{
									Warning_screen((uint8_t*)"Syr Disengage",0);
									WARN_SOUND(3,1,3);
									delay_secs(2);
									//warning_screen_erase(LIGHT_GRAY_5);
									bottom_para_frame(G_infusion_modes);
								}
								//dose_blink_count=10;
								POW.power_status_flag=1;
				//		        delay_secs(3);
				//              Warning_Scr_Erase();
					}

				}
				else
				{
					WARN_SOUND(2,1,2);
				}
			}

			else
			{
				 normal_run=0;
				 Timer_disable();
			//   event_no_update(4);
				 RAMP.down_flag=0;
			}
//			Rectangle(10,10,60,30,MAIN_FRAME_COL);
		}

		else
		{
			Notification(KEY_LOCK);
		}
		while(KEY.value);
		break;

	}
	return exit_flag;
}

void key_fun_ramp_mode()
{

	uint32_t Max_limit=0;



	switch(KEY.value)
	{


		case OK_KEY :
			home_flag=0;
		//beep_sound();
		if(!keylock)
		{
			key_pressed=30;
			if((!normal_run)) //&& (!RAMP.down_flag))
			{
				Ramp_mode();
				POW.power_status_flag=1;
			}

		}
		else
		{
			Notification(KEY_LOCK);
		}
		while(KEY.value);
		break;


		case START_STOP_KEY:

			if(MOT.time && (MOT.ramp_down || MOT.ramp_up) && MOT.volume && MOT.plateau_rate &&(!disc)&&(sy_size!=0))
			{
				if(!normal_run)
				{
					normal_run=1;
					MR_Value_calculate(Ramp_pre_FR);
				}
				else
				{
					normal_run=0;
					Timer_disable();
					RAMP.up_flag=0;
				}
			}
			else
			{
				if((!MOT.time && !MOT.ramp_down && !MOT.ramp_up && !MOT.volume && !MOT.plateau_rate ) ||
					(MOT.volume && !MOT.time && !MOT.ramp_down && !MOT.ramp_up)||
					(MOT.volume && MOT.time && !MOT.ramp_down && !MOT.ramp_up))
				{
				pop_up(SET_RAMP);
				POW.power_status_flag=1;
				}


				if(sy_size==0)
				{
					Warning_screen((uint8_t*)"No syringe",0);
					//G_bottom_scr_flag=1;
					WARN_SOUND(3,1,3);
					delay_secs(2);
					//warning_screen_erase(LIGHT_GRAY_5);
					bottom_para_frame(G_infusion_modes);
				}
				if(disc==1)
				{
					Warning_screen((uint8_t*)"Syr Disengage",0);
					WARN_SOUND(3,1,3);
					delay_secs(2);
					//warning_screen_erase(LIGHT_GRAY_5);
					bottom_para_frame(G_infusion_modes);
				}
			}

			//home_flag=0;
		//beep_sound();
//		if((!keylock)||(normal_run))//29-03
//		{
//			key_pressed=30;
//
//			if((INFUSED.ml==MOT.volume))
//			{
//
//				if(last_clr_flag)
//				{
//					//Notification(SET_TARGET);
//					WARN_SOUND(2,1,2);
//				}
//				else
//				{
//					Warning_screen((uint8_t *)"Target reached",0);
//					WARN_SOUND(2,1,2);
//					delay_secs(2);
//					Warning_Scr_Erase();
//				}
//			}
//
//			else
//			{
//				if(normal_run==0)
//				{
//					if(MOT.flowrate!=0)
//					{
//
//						normal_run=1;
//						DRV8825_ENABLE();
////						event_no_update(3);
//						MFB.Motor_pulse=0;
//						war_disp=0;
//						//infuse_clr();
//						MR_Value_calculate(MOT.flowrate);
//					}
//
//					else
//						{
//
//							WARN_SOUND(2,1,2);
//						//	Font_Select(ARIAL_14);
//						//	TFT_Number(150,70,23,BLACK,RED);
//							Notification(SET_FLOWRATE);
//
//						}
//
//				}
//
//				else
//				{
//
//					//event_no_update(4);
//					normal_run=0;
//				press_run_alarm=180;
//					if( (RAMP.RPM_control > 50.0) && (RAMP.on_off == 1) ) // RPM ramp on off flag //25-03
//						{
//
//							RAMP.down_flag=1;
//							normal_run=0;
//							//RAMP.dec=RAMP.RPM_control/_RPM;  /*CHECK*/
//						}
//						else
//						{
//		//					DRV8825_DISABLE();
//		//					NVIC_DisableIRQ(CTIMER1_IRQn);
//						Timer_disable();//29-03
//						}

//						RAMP.up_flag=0;
//
//				}
//				Rectangle(10,10,60,30,MAIN_FRAME_COL);
//			}
//		}
//		else
//		{
//			Notification(KEY_LOCK);
//		}

		while(KEY.value);
		break;

	}

}

//void auto_keylock(uint16_t lock_time)
//{
//
//	if(key_pressed==0)
//	{
//		auto_lock++;
//
//		if(auto_lock==lock_time)
//		{
//			auto_lock=0;
//
//			keylock=1;
//			Notification(KEY_LOCK);
//		}
//
//	}
//
//}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void auto_keylock(uint8_t auto_level)
{
	static uint8_t lock_check=0;
	uint16_t lock_time[4]={0,60,300,600};

	if( !keylock &&   lock_time[auto_level] )
	{
		if(!auto_lock )
		{
			if(!lock_check)
			{
				lock_check=1;
				auto_lock=lock_time[auto_level];
			}
			else
			{
				lock_check=0;
				auto_lock=0;
				keylock=1;
				Notification(KEY_LOCK);
			}

		}
		else if(KEY.value)
		{
			auto_lock=lock_time[auto_level];
		}
	}
	else if(auto_lock || lock_check)
	{
		auto_lock=0;
		lock_check=0;
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Disc_failure_condition(void)
{
	uint16_t strain_adc=0;
	uint8_t value=3;
	while(value)
	{
		strain_adc=Pressure_ADC();
		value--;
	}

//	Font_Select(ARIAL_14);
//    TFT_Number(0,70,strain_adc,YELLOW,RED);

		if(!normal_run) //REW_UP
		{
			if(( (strain_adc<=Occl.Zero_PSI) && (sy_size)) && ((!disc) ||(release) ))
			{
				Alarm(DISC_ERR);
			}
		}

		if(normal_run)
		{
			if(( (strain_adc>Occl.Zero_PSI) || (sy_size)) && ((!disc) ||(!release) ))
			{
				Alarm(DISC_ERR);
			}
		}


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void pressure_display(uint8_t bol_run,uint8_t bol_reset_flg)
{
	uint8_t presure_ratio[5]={51.7,1,6.8,6.8};
//  uint8_t PSI_Value=0;

	if((bol_run!=1) && (!bol_reset_flg))    //REW1F change to function
	{
		 if((PSI_temp_value!=(Occl.PSI_VALUE*presure_ratio[G_occl_unit])))
		 {
			    Font_Select(ARIAL_12);
//			    p_xpos=Letters_Alignments(0,75,(uint8_t*)local_buffer,0);
//				TFT_String(p_xpos,DYN_VAL_POS,(uint8_t*)local_buffer,MAIN_FRAME_COL,MAIN_FRAME_COL);//Hang
				TFT_String_align(345,122,117,(uint8_t *)local_buffer,0,LIGHT_GRAY_5,LIGHT_GRAY_5);
				memset(local_buffer,0,6);	//Hang

				if(G_occl_unit!=1)
				{
					sprintf((char *)local_buffer,"%02d",(int)(Occl.PSI_VALUE*presure_ratio[G_occl_unit])/10);
				}

				else
				{
					sprintf((char *)local_buffer,"%.1f",(float)(Occl.PSI_VALUE*presure_ratio[G_occl_unit])/10.0);
				}

//				p_xpos=Letters_Alignments(0,75, (uint8_t*)local_buffer,0);
//				TFT_String(p_xpos,DYN_VAL_POS,(uint8_t*)local_buffer,MAIN_FRAME_COL,YELLOW);
			//	Font_Select(ARIAL_9);
				TFT_String_align(355,122,117,(uint8_t*)local_buffer,0,LIGHT_GRAY_5,DARK_TEAL);
				PSI_Value=Occl.PSI_VALUE*Occl.dynamic_ratio/10;
	//			dynamic_pressure(PSI_Value,0);
//				speedometer3(PSI_Value);
				PSI_temp_value=Occl.PSI_VALUE*presure_ratio[G_occl_unit];
		}
	 }
//			Font_Select(ARIAL_BOLD_14);
//			TFT_Number(150,70,PSI_temp_value,BLACK,WHITE);

			speedometer3(PSI_temp_value,0);
//				speedometer3(PSI_temp_value/10);

}

uint8_t Key_functionality(uint8_t mode)
{
	uint8_t exit_flag = 1;

	     if((BOL.mode) && (BOL.infused_ml==pre_flowrate))
	     {
//	    	 Power_off_// // // beep_sound();
	    	 pre_flowrate+=10;
	     }

		if((BOL.run_mode) && (BOL.infused_ml > first_lmt[sy_size]))
		{
			bolus_mode_stop();
			Bolus_status_flag=1;
			Bolus_reset_flag=1;
		}

		if(BOL.run_mode)
		{
			if(BOL.flowrate!=0)
			{
				Bolus_Running_symbol(70, 10,70, 240,0xFFE7C4);
				Auto_bolus_timedisplay();
			}

		//	Running_symbol(56,3,MAIN_FRAME_COL);
			bolus_fr_screen(BOL.infused_ml);

			if((BOL.mode == 1) && (KEY.value==PURGE_START_STOP_KEY))
			{
				bolus_mode_stop();
				Bolus_status_flag=1;
				Bolus_reset_flag=0;
			}

		   if((MOT.flowrate>BOL.flowrate) && (!G_Bolus_warning_flag) )
		   {
//				Frame_Footer_warning(WARNING_COLOR,BLACK,(uint8_t*)"Less Bolus FR",(uint8_t*)"",0);
				G_Bolus_warning_flag=1;
		   }

			if(!KEY.value)
			{
				bolus_mode_stop();
				Bolus_status_flag=1;
				Bolus_reset_flag=2;
			}

		}

		if(Bolus_status_flag)
		{
			if(!Bolus_reset_flag)
			{
				bolus_mode_stop();
				MFB_Reset_data();
				waitingtime_1=2;
			//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
				CLRSCR(MAIN_FRAME_COL);
//				TFT_ST7789V__rectangle_curve(40,40,160,240,0,WHITE);
//				Font_Select(ARIAL_14);
//				TFT_String(80,110,(uint8_t *)"Purge completed",WHITE,BLACK);
//				delay_secs(1);
				press_run_alarm=180;
				Pump_idle=10;
				POW.power_status_flag=1;
				Bolus_status_flag=0;
			}
		}

		if(!KEY.value)
		{
			longpress=0;
			key_longpress=0;
			mode_longpress=0;        //21-03-2024
		}
		exit_flag = 1;
	switch(KEY.value)
	{

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

		case BOLUS_KEY:

				if(!keylock)
				{
					if(G_infusion_modes!=3)
					{
						home_flag=0;

						key_pressed=30;
						if(LIN_POT.syr_end_flag==1 )
						{
							LIN_POT.nr_empty_alarm_flag = 0;
							//Warning_Scr_Erase();
							Alarm(SYR_END);
						}
						else if(!BOL.run_mode)
						{
							if(BOL.mode == 0)
							{
								// // // beep_sound();
								G_Bolus_warning_flag=0;
								MFB_Reset_data();
								bolus_fun();  						//auto bolus
								Bolus_Running_symbol(70, 10,70, 240,0xFFE7C4);
							//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
								CLRSCR(MAIN_FRAME_COL);
								POW.power_status_flag=1;
							}

							else if(BOL.mode == 1)
							{
								if(longpress==0)
								{
									longpress=1;
									waitingtime_1=2;
								}

								if((!waitingtime_1) )
								{
									// // // beep_sound();
									G_Bolus_warning_flag=0;
//									Bolus_Running_symbol(105,15,105,230,ORPIMENT_YELLOW);
									purge_bolus();
									Bolus_status_flag=0;
									longpress=0;
								}
							}
						}

					}
					else
					{
						pop_up(NO_BOLUS);
						POW.power_status_flag=1;
					}
		      }
		      else
		      {
		    	  Notification(KEY_LOCK);
		      }
		   // while(KEY.value);
			break;

//		case BACK_KEY:
//			//  home_flag=0;
//
//				if(!keylock)
//				{
//					key_pressed=30;
//					if((G_infusion_mode_opt==3) && (!normal_run)&& (!RAMP.down_flag))
//					{
//						if(mode_longpress==0)
//						{
//							waitingtime_1=2;
//							mode_longpress=1;
//						while(KEY.value==BACK_KEY && mode_longpress)
//						{
//							if(!waitingtime_1)
//							{
//								// // // beep_sound();
////								 Alarm(BACK_KEY_FAIL);
//								G_infusion_modes=3;// To Clear the current mode
////								G_Long_press_back=1;
////								G_Brand_back_flag=1;
//								exit_flag = 0;
//								break;             //21-03-2024
//							}
//						}
//					   }
//				  }
//
//				}
//
//				else
//				{
//					Notification(KEY_LOCK);
//				}
//
//		//		while(KEY.value);
//				break;

		case MUTE_KEY:
					// // // beep_sound();

					if(keylock==1)
					{
						home_flag=0;//12-04-2024
					//	Notification(KEY_LOCK);
						Notif_screen((uint8_t *)"Keypad locked");
//						TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
					}

					key_longpress=0;

					if(mode_longpress==0)
					{
						waitingtime_1=2;
						mode_longpress=1;
						while(KEY.value==MUTE_KEY && mode_longpress)//21-03
						{
							if(!waitingtime_1)
							{
								if(keylock)
								{
									keylock=0;
									Notification(KEY_UNLOCK);
								}
								else
								{
									keylock=1;
									Notification(KEY_LOCK);
								}
								break;//21-03
							}
						}
					}

					if(waitingtime_1)
					{
					//	delay_secs(2);
					//	Warning_Scr_Erase();
					}
					while(KEY.value);
					break;

	}

		if(mode==0)
		{
			key_main_screen();	//21-03
		}
		else if(mode==1)
		{
			key_main_screen_VTBI();
		}
		else if(mode==2)
		{
			key_fun_doserate_mode();
		}
		else if(mode==3)
		{
			key_fun_ramp_mode();
		}

	return exit_flag;

 }

void MFB_Reset_data(void)
{
	MFB.Min_cuts=0;
	MFB.Motor_pulse=0;
	MFB.twenty_cuts_count=0;
	MFB.cuts_time_diff=0;
	MFB.cuts_time_diff_cnt=0;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void LED_PixelColor(uint32_t led_pos ,uint32_t Red,uint32_t Green,uint32_t Blue)
{
    colour[led_pos] =  ((( (  (Green/brightness)) ) <<16)|((( Red/brightness)) << 8 ) | (((Blue/brightness)) <<0 )  );

}

void Alarm_LED(uint8_t Led_color)
{
	static uint8_t Led_color_temp=4;
	uint8_t col_sel[3][3]={{255,0,0},{0,255,0},{0,0,0}};

	if(Led_color_temp!=Led_color)
	{
		for(int loop =0;loop<=2;loop++)
		{
			colour[loop] =  ((( (  (col_sel[Led_color][0]/brightness)) ) <<16)|((( col_sel[Led_color][1]/brightness)) << 8 ) | (((col_sel[Led_color][2]/brightness)) <<0 )  );
		}
		 NVIC_SetPriority(CTIMER3_IRQn,0);
		 NVIC_EnableIRQ(CTIMER3_IRQn);
//		 CTIMER3->TCR = 0x1U;                  //start
		//PWM_Init();
		Led_color_temp=Led_color;

	}
}

void Calibration_error_condition(void)//REW_UP
{
	uint8_t syringe_size=0;
	syringe_size=detect_syringe_size(1);
	if(((Occl.Zero_PSI>2010))||(Occl.Zero_PSI<1990))
	{
		Alarm(PSI0_ERR);
	}

	else if(((Occl.strain_20PSI_value[syringe_size]>2700))||(Occl.strain_20PSI_value[syringe_size]<1990))
	{
		Alarm(PSI20_ERR);
	}
}

void key_alarm()
{
//	switch(KEY.value)

	if(KEY.value)
	{
		if(key_check)
		{
		G_key_failure=5;

		key_check=0;
		}
		if(!G_key_failure)
		{
//			Font_Select(ARIAL_BOLD_14);
//			TFT_Number(110,50,123,YELLOW,BLACK);
			//Rectangle(10,30,150,100,BLUE);
//			Alarm(UP_KEY_FAIL);
//			while(1);
		}
	//  home_flag=0;
//		Font_Select(ARIAL_BOLD_14);
//		TFT_Number(190,50,KEY.value,YELLOW,BLACK);
//		Font_Select(ARIAL_BOLD_14);
//		TFT_Number(190,100,G_key_failure,YELLOW,BLACK);
			   }

	else
	{
		G_key_failure=0;
		key_check=1;
	}
  }

void warning_check_fun()
{
	static uint8_t war_loop_cnt=0;
	char* war_names[6]={"Device in use","Near to Target","Near to empty","Syringe end","Pre occlusion","Battery Low"};

	if(war_status[war_loop_cnt]==1)
	{
		Warning_screen((uint8_t *)war_names[war_loop_cnt],0);
		delay_ms(500);
		bottom_para_frame(G_infusion_modes);
	}

	 if(war_loop_cnt>5)
	 {
		 war_loop_cnt=0;
	 }
	 else
	 {
		 war_loop_cnt++;
	 }

	  Font_Select(ARIAL_14);
	  TFT_Number(200,70,war_loop_cnt,YELLOW,RED);
	  TFT_Number(200,100,war_status[5],YELLOW,RED);

}
