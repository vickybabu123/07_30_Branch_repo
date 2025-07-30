
#include "LPC55S16.h"
#include "Color.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "Driver_Timer.h"
#include "fsl_clock.h"
//#include "../Menu_Setting/Main_Menu.h"
#include "Driver_ADC.h"
#include "TFT_Commands.h"
#include "Power_off.h"
#include "Bolus_Header.h"
#include"Alarm.h"
#include "Events_log.h"
#include <Consumed_hrs.h>
#include "Power_off.h"


power_flag POW;
main_flag MAIN;
battery BATT;

uint8_t Confirm_count=0;
uint8_t purge_device_in=0;
uint8_t device_check_cnt;
uint16_t status_of_power_off();
uint8_t status=0;

extern uint16_t Power_Interval_Delay;
extern uint8_t cnfrm_flag;
extern uint8_t normal_run;//12-06-2024
extern uint8_t G_infusion_modes;
extern uint8_t near_to_target_flag;
extern uint8_t war_disp;
extern uint8_t home_flag;
extern uint8_t G_bright_level;
extern uint8_t G_infusion_modes;
extern uint8_t G_main_screen_flag;
extern uint8_t ON_Flag;
extern uint8_t G_standby;
extern uint16_t ADC_power_key_sampling(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples);
extern void power_status(void);
extern void batt_stat(void);
extern void battery_status(void);
extern void  write_current_data(uint8_t mode);
extern uint8_t syr_status_chk;

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Power_off()
 {
	uint16_t Output_sample=0;
	uint16_t PWR_values=0;
	uint16_t status_values=0;
	static uint8_t device_off_flag=0;

	power_status();
	battery_status();
	batt_stat();

	PWR_values=ADC_read(ADC_CH_POWER_KEY);
	Font_Select(ARIAL_14);
//	TFT_Number(0,70,PWR_values,YELLOW,RED);
	Output_sample=ADC_power_key_sampling(PWR_values,100,10);
	//usedhrs_status();

	if(Power_Interval_Delay==0)
	{
		Power_Interval_Delay=100;
	}
		if(Output_sample > 700)   //2000)
		{
			if(Confirm_count>20)
				{
					if((!normal_run)&&(!BOL.run_mode))//12-06-2024
					{
						status_values = status_of_power_off();
						if(status_values > 110)
							{
							    Timer_disable();
								EEPROM_usedhrs_write(0);//ALL USED_HRS WRITE WHILE POWER OFF
								event_no_update(2);
								write_current_data(G_infusion_modes);
								SUPPLY_TRIGGER_OFF;
								Rectangle(0,0,320,240,BLACK);
						    	home_flag=0;
						    	G_bright_level=0;
							//  time_screen_saver();

								ON_Flag =0;
//								MAIN.main_power_status_flag=1;
								Confirm_count=0;
								syr_status_chk=0;
								while(1)
								{
									PWR_values=ADC_read(ADC_CH_POWER_KEY);
									delay_ms(100);

									if(PWR_values<700)
									break;
								}
							}

						else
						{
							if((G_standby)&&(cnfrm_flag))
							{
								cnfrm_flag=0;
								status=standby_mode_function();
								//ON_Flag=0;
								if(status)
								{
									Confirm_count=0;
									POW.power_status_flag=1;
								}

							}
							else
							{
								Confirm_count=0;
								POW.power_status_flag=1;
							}


						}
				}


			else
			{
						if(device_off_flag==0)//12-06-2024
						{
							device_off_flag=1;
							if((normal_run) && (!BOL.run_mode))
							{
//								Warning_screen((uint8_t *)"Device in use",0);
							}

							else if(BOL.run_mode)
							{
								Frame_Footer_warning(WARNING_COLOR,BLACK,(uint8_t*)"Device in use",(uint8_t*)"",0);

								purge_device_in=2;
							}

//							if(!purge_device_in)
//							{
//								Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to Stop",(uint8_t*)"BK",1);
//							}

							WARN_SOUND(3,1,3);
							device_check_cnt=3;
						}

						else
						{
							if(!device_check_cnt)
							{
//								if((normal_run) &&(!BOL.run_mode))
								{
									//Warning_Scr_Erase();
								}
								war_disp=0;
								device_off_flag=0;
							}

//							else
//							{
//								device_off_flag=0;
//							}

						}

					}
				}             //12-06-2024
				else
				{
					Confirm_count++;
				}
		}

	else
	{
		Confirm_count=0;
			if(device_off_flag)
			{
				if(!device_check_cnt)
					{

						if( (normal_run<=1) && (home_flag) )
						{
							//Warning_Scr_Erase();
						}

							if((!purge_device_in) && (!home_flag))
							{
//								Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"OKAY");
//								right_14(202,100-40-5-3,0,ORANGE_DARK,WHITE);
//								Font_Select(ARIAL_BOLD_14);
//								TFT_String(195-30-5+10,202+3,(uint8_t* )"to select",ORANGE_DARK,WHITE);
							}
								war_disp=0;
								device_off_flag=0;

					}

			}
	}
 }
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint16_t status_of_power_off()
{
	uint16_t loop_count=0;
	uint16_t PWR_values=0;
	uint16_t Output_sample=0;


	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Font_Select(ARIAL_14);
	Rectangle(30,110,260,1,WHITE);
	Rectangle(30,150,260,1,WHITE);

	Rectangle(30,111,1,40,WHITE);
	Rectangle(290,111,1,40,WHITE);


	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Power Off");
	Frame_Footer_new(NEW_FTR_COL,HDR_TXT_COL,(uint8_t *)"Keep Press");
	CENTER_SCR(MAIN_FRAME_COL); //center greenFont_Select(ARIAL_12);
	for(loop_count=0;loop_count<=120;loop_count++)
	{
		PWR_values=ADC_read(ADC_CH_POWER_KEY);
		delay_ms(10);
		Output_sample=ADC_power_key_sampling(PWR_values,100,10);

		if(Output_sample > 70)
			{
				Rectangle(40+(loop_count*2),120,0,20,WHITE);
			}
		else
		{
			break;
		}
	}
		return loop_count;
}



