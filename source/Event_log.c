/*
 * Event_log.c
 *
 *  Created on: 09-Mar-2024
 *      Author: SoftwareRND
 */
#include "LPC55S16.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "Events_log.h"
#include"Driver_I2C.h"
#include "fsl_rtc.h"
#include "Events_log.h"				//25-03
#include "Power_off.h"
#include<FR_CALC.h>
#include <infused_calculate.h>
#include "Occlusion.h"
#include<Drug_List.h>
extern uint8_t ON_Flag;
uint8_t G_ending_event=0;
extern G_Motor_run_flag;
extern char Drug_Names[150][4][20];
extern char *event_alarm1[35];
extern  uint8_t G_infusion_modes;
//extern uint16_t event1_no;
//void event_info_display(uint8_t date);
//void event_no_update();
void event_filter();
void event_info_display(uint8_t date_1);
void event_eeprom_write(uint16_t event_count);
void event_eeprom_read(uint16_t event_count);
void Event_update_1();
void Event_update_1_option_frame(uint16_t page_no);
extern rtc_datetime_t date;
uint16_t array_drug_name[50];
uint16_t array_date[50],array_month[50],array_year[50];
extern uint8_t auto_lock_time;//common_bug
extern void scroll_bar(uint8_t page_no,uint8_t option_no,uint8_t total_option);     //23-03                                  //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
extern void split_screen_info(uint16_t count);
char *event_alarm_up[20]={
		 	 	 	 	 	 	 	 	 	 	 	    "No Alarm      ",//0
														"Device ON     ",//1
														"Device OFF    ",//2
														"Motor run     ",//3
														"Motor stop    ",//4
														"Occlusion     ",//5
														"Motor Error   ",//6
														"Plunger Distu.",//7
														"Syr load Err  ",//8
														"End of syringe",//9
														"Plunger not Eng"//10
														"Syringe not Eng"//11
};
uint16_t array_hour[50],array_minute[50],array_event_no[50];
extern void event_info_display_2(uint8_t date_1);

uint16_t array_hour[50],array_minute[50],array_event_no[50],array_flowrate_no[50],array_infusedml_no[50],array_pressure_no[50],array_mode_no[50];
uint16_t array_drug_name[50];
uint16_t tot_day=0,tot_month,tot_year;
uint16_t array_evnt_pos[50];
uint16_t date1,month;
uint16_t year;
uint16_t event_id;
uint16_t total_events=0;//29-03
uint16_t event_filter_day[20];
uint16_t event_filter_month[20];
uint16_t event_filter_year[20];
uint16_t event_info[10][10];
uint8_t starting_event;
uint8_t E_main_screen=0;
uint8_t E_main_screen1=0;
uint8_t G_month_event=0;
uint8_t G_month_event1=0;
uint8_t G_Motor_flag=0;
uint8_t G_Alarm_flag=0;
uint16_t Drug_value=0;
uint16_t single_array[50];
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} DateTime;

extern DateTime currentDateTime;
extern DateTime getRTCDateTime();
void event_no_update(uint8_t event_no)
{
   // Font_Select(ARIAL_12);

//	uint8_t loop_count=1;
//	uint16_t value_of_date=0,value_of_month=0,value_of_year=0;
//	uint16_t value_of_hour,value_of_minute;

	//RTC_INIT_time();
	DateTime currentDateTime = getRTCDateTime();


	if(total_events>1000)
	{
		total_events=0;
	}

	array_date[total_events]=currentDateTime.day;
	array_month[total_events]=currentDateTime.month;
	array_year[total_events]=currentDateTime.year;
	array_hour[total_events]=currentDateTime.hour;
	array_minute[total_events]=currentDateTime.minute;
	array_flowrate_no[total_events]=MOT.flowrate;
	array_infusedml_no[total_events]=INFUSED.ml;
	array_pressure_no[total_events]=Occl.PSI_VALUE;
	array_mode_no[total_events]=G_infusion_modes;
	array_drug_name[total_events]=dose.drug_select_no;
	array_event_no[total_events]=event_no;

//  	 Font_Select(ARIAL_14);
// 	 TFT_Number(50,70,array_date[total_events],YELLOW,RED);
//
// 	 Font_Select(ARIAL_14);
// 	 TFT_Number(50,100,array_month[total_events],YELLOW,RED);
//
// 	 Font_Select(ARIAL_14);
// 	 TFT_Number(50,130,array_year[total_events],YELLOW,RED);
//
// 	 Font_Select(ARIAL_14);
// 	 TFT_Number(50,160,array_hour[total_events],YELLOW,RED);
//
// 	 Font_Select(ARIAL_14);
// 	 TFT_Number(50,190,array_minute[total_events],YELLOW,RED);
//
//	 delay_secs(5);

	 event_eeprom_write(total_events);

	 total_events++;
	 EEPROM_datawrite(EEPROM_TOTAL_EVENTS,total_events,2);

}

void event_eeprom_write(uint16_t event_count)
{
		EEPROM_datawrite(EEPROM_EVT_DATE+(event_count),array_date[event_count],1);
		EEPROM_datawrite(EEPROM_EVT_MONTH+(event_count),array_month[event_count],1);
		EEPROM_datawrite(EEPROM_EVT_YEAR+(event_count*2),array_year[event_count],2);
		EEPROM_datawrite(EEPROM_EVT_HOUR+(event_count),array_hour[event_count],1);//30-03
		EEPROM_datawrite(EEPROM_EVT_MIN+(event_count),array_minute[event_count],1);
		EEPROM_datawrite(EEPROM_EVT_ID+(event_count),array_event_no[event_count],1);//29-03
		EEPROM_datawrite(EEPROM_EVE_FLOWRATE+(event_count*2),array_flowrate_no[event_count],2);//29-03
		EEPROM_datawrite(EEPROM_EVE_INFUSED+(event_count*2),array_infusedml_no[event_count],2);//29-03
		EEPROM_datawrite(EEPROM_EVE_PRESSURE+(event_count*2),array_pressure_no[event_count],2);//29-03
		EEPROM_datawrite(EEPROM_EVE_MODE+(event_count),array_mode_no[event_count],1);

		EEPROM_datawrite(EEPROM_EVE_DRUG+(event_count*2),array_drug_name[event_count],2);//29-03
}
void event_eeprom_read(uint16_t event_count)
{
	uint8_t loop_cnt=0;
	for( loop_cnt=0;loop_cnt<event_count;loop_cnt++)
	{
	array_date[loop_cnt]=EEPROM_dataread(EEPROM_EVT_DATE+(loop_cnt),1);
	array_month[loop_cnt]=	EEPROM_dataread(EEPROM_EVT_MONTH+(loop_cnt),1);
	array_year[loop_cnt]=	EEPROM_dataread(EEPROM_EVT_YEAR+(loop_cnt*2),2);
	array_hour[loop_cnt]=	EEPROM_dataread(EEPROM_EVT_HOUR+(loop_cnt),1);
	array_minute[loop_cnt]=	EEPROM_dataread(EEPROM_EVT_MIN+(loop_cnt),1);
	array_event_no[loop_cnt]=EEPROM_dataread(EEPROM_EVT_ID+(loop_cnt),1);//29-03
		array_flowrate_no[loop_cnt]=EEPROM_dataread(EEPROM_EVE_FLOWRATE+(loop_cnt*2),2);//29-0
		array_infusedml_no[loop_cnt]=EEPROM_dataread(EEPROM_EVE_INFUSED+(loop_cnt*2),2);//29-0
		array_pressure_no[loop_cnt]=EEPROM_dataread(EEPROM_EVE_PRESSURE+(loop_cnt*2),2);//29-0
		array_mode_no[loop_cnt]=EEPROM_dataread(EEPROM_EVE_MODE+(loop_cnt),1);//29-0
		array_drug_name[loop_cnt]=EEPROM_dataread(EEPROM_EVE_DRUG+(loop_cnt*2),2);//29-0
	}
}
void event_filter()
{
	uint8_t loop_count=0;

    Font_Select(ARIAL_12);
    tot_day=0;
	event_filter_day[0]=array_date[0];
	event_filter_month[0]=array_month[0];
	event_filter_year[0]=array_year[0];
	array_evnt_pos[0]=0;
	tot_day++;

		for(loop_count=1;loop_count<total_events;loop_count++)
		{
			if(((event_filter_day[tot_day-1])!=array_date[loop_count])||
						((event_filter_month[tot_day-1])!=array_month[loop_count])||
							((event_filter_year[tot_day-1])!=array_year[loop_count]))
			{
				event_filter_day[tot_day]=array_date[loop_count];
				event_filter_month[tot_day]=array_month[loop_count];
				event_filter_year[tot_day]=array_year[loop_count];
				array_evnt_pos[tot_day]=loop_count;
		        tot_day++;

			}

		}
		array_evnt_pos[tot_day]=loop_count;// 26-03
}
uint8_t event_display()
{
	uint8_t event_var=1;
    uint8_t option_no=0;
	uint8_t prev_option = 0;
    uint8_t page_no=0;
	uint8_t end_opt_no=0;
	//uint8_t page=0;
    uint8_t max_page=0,max_opt=0;

	auto_lock_time=AUTO_EXIT_TIME;//common_bug
	POW.power_status_flag=1;//27-03

    if(!total_events)
    {
		 events_split_screen();
		 TFT_ST7789V__rectangle_curve(30,65,110,255,0,LIGHT_GRAY);
		 Font_Select(ARIAL_BOLD_14);
		 TFT_String(85,110,(uint8_t *)"No Events",LIGHT_GRAY,BLACK);
		 delay_secs(2);
    }

    else
    {
		event_eeprom_read(total_events);
		event_filter();

		max_page=(tot_day-1)/4;
		max_opt=(tot_day-1)%4;

		page_no = max_page;
		option_no =max_opt;
		prev_option=max_opt;

		if(max_page==page_no)
		{
			end_opt_no=max_opt;
		}

		else
		{
			end_opt_no=3;
		}

	while(KEY.value);
	while(event_var)
	{
		Power_off();//27-03

		if(!ON_Flag)
		{
			break;//26-03
		}

			else if(POW.power_status_flag)//27-03
			{
				POW.power_status_flag=0;//27-03
				events_split_screen();

				events_option_frame1(page_no,option_no,end_opt_no);

				if(max_page)
				{
					scroll_bar(page_no,option_no,tot_day);//27-03

				}
			}
					if(KEY.value)//common_bug  //ZOOZ_1
					{
						auto_lock_time=AUTO_EXIT_TIME;
					}
					else if(!auto_lock_time)
					{
						break;
					}//common_bug

		switch(KEY.value)
		{
			case DOWN_KEY:

			       if(((option_no<3)&&(page_no<max_page))||(option_no<max_opt))
			       {
			    	   option_no++;
			       }

			       else
					{
					   if(page_no<max_page)
						{
							page_no++;
						}
						else
						{

							page_no=0;
						}

					if(max_page==page_no)
					    {
					    	end_opt_no=max_opt;
					    }
					    else
					    {
					    	end_opt_no=3;
					    }
//

					option_no = 0;
					prev_option = option_no;

					events_split_screen();
					events_option_frame1(page_no,option_no,end_opt_no);

					}

			       if(option_no!=prev_option)
					{
				//		if(option_no<3)
						{
							events_option_scroll(prev_option,option_no,page_no,event_filter_day,event_filter_month,
									event_filter_year);
						}
						prev_option = option_no;

					}
                   if(max_page)
				   {
					 scroll_bar(page_no,option_no,tot_day);//26-03
				   }

					 while(KEY.value);
					 break;
			case UP_KEY:

					 if(((option_no>0)))
								       {
								    	   option_no--;
								       }

								       else
										{
										if(page_no>0)
											{
												page_no--;
												option_no=3;
											}

											else
											{

												page_no=max_page;
												option_no=max_opt;
											}


										if(max_page==page_no)
										 {
										    end_opt_no=max_opt;
										 }
										else
										 {
										    end_opt_no=3;
										 }


						//				option_no = 3;
										prev_option = option_no;

										events_split_screen();
										events_option_frame1(page_no,option_no,end_opt_no);

										}

					 	 	 	 	 	if(option_no!=prev_option)
										{
										//	if(option_no>0)
											{
												events_option_scroll(prev_option,option_no,page_no,event_filter_day,event_filter_month,
																					event_filter_year);
											}
											prev_option = option_no;

										}
					                  			 //
//					                   TFT_Number(170,150,option_no,RED,BLACK);
//					               	   TFT_Number(70,50,page_no,GREEN,BLACK);
					               	if(max_page)
										{
											scroll_bar(page_no,option_no,tot_day);//26-03

										}
										 while(KEY.value);
										 break;
					case OK_KEY:

						event_info_display(((page_no*4)+option_no));
						POW.power_status_flag=1;//26-03

					    while(KEY.value);
					    break;


						case BACK_KEY:

						     event_var=0;

							 while(KEY.value);
							 break;



	}

	}
    }

}

void event_info_display(uint8_t date_1)
{
	    uint8_t option_no=0;
		uint8_t prev_option = 0;
	    uint8_t page_no=0;
		uint8_t end_opt_no=0;
		uint8_t max_page=0,max_opt=0;
		uint8_t event_count=0;
		uint8_t event_info=1;
        uint16_t  index;

		auto_lock_time=AUTO_EXIT_TIME;//common_bug
		POW.power_status_flag=1;//27-03

		event_count=array_evnt_pos[date_1+1]-array_evnt_pos[date_1];
		G_ending_event=event_count-1;
		starting_event=array_evnt_pos[date_1];
		delay_secs(2);
		max_page=(event_count-1)/4;//0
		max_opt=(event_count-1)%4;//3

		if(max_page==page_no)
		{
			end_opt_no=max_opt;
		}

		else
		{
			end_opt_no=3;
		}

		while(KEY.value)
		{

		}

		 while(event_info)
		 {
			 Power_off();//27-03


			if(!ON_Flag)
				{
					break;//26-03
				}

			else if(POW.power_status_flag)//27-03

		   {
				POW.power_status_flag=0;//27-03
				split_screen_info(date_1);
				event_info_option_frame(page_no,option_no,end_opt_no);

				if(max_page)
				{
					scroll_bar(page_no,option_no,event_count);//26-03

				}
		   }

				if(KEY.value)//common_bug  //ZOOZ_1
				{
				   auto_lock_time=AUTO_EXIT_TIME;
				}
				else if(!auto_lock_time)
				{

					break;

				}//common_bug


            switch(KEY.value)
            {
						case DOWN_KEY:
								   if(((option_no<3)&&(page_no<max_page))||(option_no<max_opt))
								   {
									   option_no++;
								   }
								   else
									{
									if(page_no<max_page)
										{
											page_no++;
										}

										else
										{

											page_no=0;

										}


									if(max_page==page_no)
										{
											end_opt_no=max_opt;
										}
										else
										{
											end_opt_no=3;
										}


									option_no = 0;
									prev_option = option_no;

									events_split_screen();
									event_info_option_frame(page_no,option_no,end_opt_no);

									}


								   if(option_no!=prev_option)
									{
								//		if(option_no<3)
										{
											events_info_scroll(prev_option,option_no,page_no,array_hour,array_minute,event_alarm1);

										}
										prev_option = option_no;

									}
								   if(max_page)
											{
												scroll_bar(page_no,option_no,event_count);//26-03

											}
											 //
									 while(KEY.value);
									 break;

								case UP_KEY:

										 if(((option_no>0)))
										   {
											   option_no--;
										   }

										   else
											{
											if(page_no>0)
												{
													page_no--;
													option_no=3;
												}
												else
												{

													page_no=max_page;
													option_no=max_opt;


												}


											if(max_page==page_no)
												{
													end_opt_no=max_opt;
												}
												else
												{
													end_opt_no=3;
												}


												prev_option = option_no;

												events_split_screen();
												event_info_option_frame(page_no,option_no,end_opt_no);

												}
//


											   if(option_no!=prev_option)
												{
												//	if(option_no>0)
													{
														events_info_scroll(prev_option,option_no,page_no,array_hour,array_minute,event_alarm1);

													}
													prev_option = option_no;

												}

											   if(max_page)
												{
													scroll_bar(page_no,option_no,event_count);//26-03

												}

												 while(KEY.value);
												 break;



								case OK_KEY:

//        if (strcmp((event_alarm1[array_event_no[(G_ending_event+starting_event)-((page_no*4)+option_no)]]),
//        		(event_alarm_up[array_event_no[(G_ending_event+starting_event)-((page_no*4)+option_no)]])) == 0)
							// if (strcmp(alarm1, alarm_up) == 0)
//	  if (strcmp((event_alarm1[array_event_no[(G_ending_event+starting_event)-((page_no*4)+option_no)]]),
//				    		 "Motor run     ") == 0)
//
//                                    {
//		                            G_Motor_flag=1;
//									event_info_display_2(((page_no*4)+option_no));
//									}
//	  if (strcmp((event_alarm1[array_event_no[(G_ending_event+starting_event)-((page_no*4)+option_no)]]),
//			  "Syr load Err  ") == 0)
//
//                                    {
//		                              G_Alarm_flag=1;
//									event_info_display_2(((page_no*4)+option_no));
//									}

									event_info_display_2((G_ending_event+starting_event)-((page_no*4)+option_no));

									POW.power_status_flag=1;//26-03


									while(KEY.value);
								    break;

								case BACK_KEY:

									 event_info=0;

									 while(KEY.value);
									 break;

}
}
}



void event_info_display_2(uint8_t date_1)
{
	    uint8_t option_no=0;
		uint8_t prev_option = 0;
	    uint8_t page_no=0;
		uint8_t end_opt_no=3;
		uint8_t max_page=0,max_opt=0;
		uint8_t event_count=0;
		uint8_t event_info_1=1;

		auto_lock_time=AUTO_EXIT_TIME;//common_bug
		POW.power_status_flag=1;//27-03

		 while(event_info_1)
		 {
			 Power_off();//27-03


			if(!ON_Flag)
				{
					break;//26-03
				}

			else if(POW.power_status_flag)//27-03

		   {
				POW.power_status_flag=0;//27-03
	     Event_update_1();
			Event_update_1_option_frame(date_1);


		   }

				if(KEY.value)//common_bug  //ZOOZ_1
				{
				   auto_lock_time=AUTO_EXIT_TIME;
				}

				else if(!auto_lock_time)
				{

					break;

				}//common_bug


            switch(KEY.value)
            {
				case BACK_KEY:

					 event_info_1=0;

					 while(KEY.value);
					 break;

}
}
}
