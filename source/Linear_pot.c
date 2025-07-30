
#include "Linear_Header.h"
#include "Driver_ADC.h"
#include "Color.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "SD_Calib.h"
#include "Motor.h"
#include "Alarm.h"
#include "Bolus_Header.h"
#include "SD_Calib.h"
#include "Driver_Timer.h"
#include "Linear_pot.h"
uint8_t warn_disp_flag=0;
uint8_t warn_disp_count=0;

uint16_t adc_rd;
uint16_t rem_plot=0;
//volatile float rem_volume;
uint8_t end_flag=0;
uint16_t Sample_value=0;
LINEAR_POT LIN_POT;
extern uint16_t linear_temp_adc;
extern uint16_t sl_pullout_ADC;
extern volatile uint8_t normal_run;
SYRINGE SYR;
//extern UINT ssize;
extern volatile uint8_t normal_run;
//int remaining_volume_in_syringe();
float remaining_volume_in_syringe(void);
void read_eeprom_all_sizsyr();
extern uint16_t value_l[8];
extern uint16_t value_c[8];
extern uint8_t G_near_empty;
extern float __1_ml_ADC;
extern float __0_ml_ADC;
extern uint8_t sy_size;
uint8_t near_empty_duration[5]={3,5,10,15};

void plunger_based_o_ml_max_ml_calculate();
extern void semi_circle(uint8_t R_L_flag,uint8_t xpos,uint8_t y_pos,uint8_t radius,uint32_t bg_clr,uint32_t in_clr);
extern void draw_rem_frame();
extern uint8_t G_infusion_modes;
extern uint8_t war_status[3];

struct LIN_POT_sample{
	 uint16_t Raw_data[20];
	 uint8_t current_pos;
	 uint16_t Sample_data;
	 uint8_t Err_flag;
	 uint8_t Err_count;
	 uint8_t Fail_count;
	 uint32_t cumm_data;
	 uint8_t count;
}LINEAR_sample;

/****************************************************************************
* Function Name : float syringe_nearempty_calculation(uint16_t flowarte)
* Arguments     : flowarate
* Return type   : void
* Description   : these function calculate near empty adc value for all size syringes

*****************************************************************************/
float syringe_nearempty_calculation(uint16_t flowarte)
{
		float volume_for_1_min;
		float for_1_ml_adc;
		volatile float Near_Emty_ml_for_a_minute;
		float  syringe_volume[7]={0,2.0,5.0,10.0,20.0,30.0,50.0};
		uint8_t ssize=0;

		ssize=detect_syringe_size(1);

		 volume_for_1_min        = flowarte/MINUTE_60;

		 for_1_ml_adc              = (float)(LIN_POT.syr_max_pos[ssize]-LIN_POT.syr_end_pos[ssize])/syringe_volume[ssize];

		Near_Emty_ml_for_a_minute =  volume_for_1_min*near_empty_duration[G_near_empty];

		LIN_POT.adc_value_for_a_Minute_ml    =  (unsigned int)((Near_Emty_ml_for_a_minute * for_1_ml_adc) +  LIN_POT.syr_end_pos[ssize]);

		return 0;
}

void display_funtion()
{
		   /* Rectangle(0,0,320,240,MAIN_FRAME_COL);
		    Rectangle(0,0,320,35,NEW_HDR_COL);
		    Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Accuracy Calibration");

		    Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press         to Calibrate");
		    TFT_ST7789V_lcd_drawcircle1(221+1,150-11,15,MAIN_FRAME_COL,MAIN_FRAME_COL);

		    Font_Select(ARIAL_9);
		    TFT_String(216+1,141-11,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);


		    TFT_ST7789V__rectangle_curve(45,7,305,150,0,WHITE); // SCREEN

			Font_Select(ARIAL_14);
			TFT_String(80+15,30-10,(uint8_t*)" 1.  Ensure the plunger is",WHITE,BLACK);

			if(disp_1)
			{
				TFT_String(105+15,43,(uint8_t*)" fully in",WHITE,BLACK);
			}
			else
			{
				TFT_String(105+15,43,(uint8_t*)" fully out",WHITE,BLACK);
			}

		    TFT_String(140+15,30-10,(uint8_t*)" 2. Ensure no Syringe",WHITE,BLACK);

		     TFT_String(53,130,(uint8_t*)"Warning",WHITE,BLACK);

		     TFT_ST7789V__rectangle_curve(50,100,25,25,0,YELLOW);
		     TFT_String(53,112,(uint8_t*)"!",YELLOW,BLACK);  */

}

void motor_stop()
{

	normal_run=0;
}

/*void linear_pot_calibration(void)
{
   int   syringe_volume=50;
   float one_ml_adc_resolution;
//	//printf("\n place the syringe  ");
//	//printf("\n place the syringe start positon ");
	LIN_POT.syr_max_pos[ssize] = samplings_LINEAR_POT(adcch0); // 50ml pos syr
//	//printf("\n press ok to store adc value ");
//  //printf("\n place the syringe end positon ");
    LIN_POT.syr_end_pos[ssize] = samplings_LINEAR_POT(adcch0); // 0ml pos syr
//	//printf("\n press ok to store adc value ");
    one_ml_adc_resolution = (float)(LIN_POT.syr_max_pos[ssize]-LIN_POT.syr_end_pos[ssize])/syringe_volume;
    // load in eeprom "one_ml_adc_resolution"
}*/


void read_eeprom_all_sizsyr()
{
	 for(int a=1;a<7;a++)
	 {
		 value_l[a]=a+800;
	     value_c[a]=a+100;
	 }

}
/****************************************************************************
* Function Name : void syringe_end_check()
* Arguments     : void
* Return type   : void
* Description   : these function checks the syringe end adc value for all size syringes

*****************************************************************************/
void syringe_end_check(uint16_t plunger_ADC_val,uint8_t syr_size)
{


    if( (LIN_POT.syr_end_pos[syr_size]+300)  >=    plunger_ADC_val )
    {
    	LIN_POT.syr_end_verify_count++;
    }
    else if(plunger_ADC_val > (LIN_POT.syr_end_pos[syr_size]+300))
    {
    	LIN_POT.syr_end_verify_count =0;
    	LIN_POT.syr_end_flag=0;
    }


      if(LIN_POT.syr_end_verify_count >= 5)
    {

    	LIN_POT.syr_end_flag=1;
    	LIN_POT.syr_end_verify_count=5;
    }
    if(LIN_POT.syr_end_flag==1 )
    {
    	if((normal_run||BOL.run_mode))                 // && (!end_flag))
    	{
    		Alarm(SYR_END);
    	}

    	else if(!end_flag )
    	{
//    		warning_screen((uint8_t *)"Syringe end",0);
          Warning_screen((uint8_t *)"Syringe end",0);
          end_flag=1;
    	}
    }

    else
    {
    	if(end_flag)
    	{
    		end_flag=0;
        	Warning_Scr_Erase(end_flag);
    	}
    }
}

/****************************************************************************
* Function Name : void near_empty_check(void)
* Arguments     : void
* Return type   : void
* Description   : these function checks the near empty  adc value for all size syringes

*****************************************************************************/
void near_empty_check(uint16_t plunger_ADC_val)   //REW 1
{

	if ( (LIN_POT.adc_value_for_a_Minute_ml   >= plunger_ADC_val) && (!LIN_POT.syr_end_flag))
	{

		LIN_POT.nr_empty_verify_count++ ; // have to verify the initialization
	}

	else
	{

		LIN_POT.nr_empty_verify_count=0;

		if(LIN_POT.nr_empty_alarm_flag)
		{
			if((LIN_POT.adc_value_for_a_Minute_ml) < plunger_ADC_val)
			{
//				Warning_Scr_Erase();

				LIN_POT.nr_empty_alarm_flag=0;
			}
		}
	}

	if(LIN_POT.nr_empty_verify_count >= 5)
	{
		if(!LIN_POT.nr_empty_alarm_flag )
		{
			war_status[0]=1;

//			if(warn_disp_count==0)
//			{
//				if((warn_disp_flag==0))
//				{
//					Warning_screen((uint8_t *)"Near to empty",0);  //REW 1
//					warn_disp_flag=1;
//					warn_disp_count=8;
//				}
//				else
//				{
//					Warning_Scr_Erase(G_infusion_modes);
////					bottom_para_frame(0);
//					warn_disp_flag=0;
//					warn_disp_count=4;
//				}

//			}
		}

		LIN_POT.nr_empty_verify_count=5;
//		LIN_POT.near_empty_count++;
	}

}

//int remaining_volume_in_syringe()
//{
//	float remaining_volume;
//	float one_ml_adc_resolution=0;
////  read data  from eeprom for size and brand automaticalyy
////  remaining_volume = ((ADC_read(3) - LIN_POT.syr_end_pos[ssize] )/one_ml_adc_resolution);
//
//    return remaining_volume;
//}

float remaining_volume_in_syringe(void)
{
	 static float rem_prev_value=0;
	 char local_buffer[6];
	 float rem_volume=0.0;
	 uint16_t xpos=0;

		adc_rd=ADC_read(ADC_CH_PLUNGER_POT);
		sy_size=detect_syringe_size(1);
		if(sy_size==5)
		{
			adc_rd=0;
		}
		plunger_based_o_ml_max_ml_calculate();

		Font_Select(ARIAL_14);
		adc_rd=(adc_rd/10)*10;
		adc_rd=LIN_POT_sample(adc_rd, 500,10 );
//		TFT_Number(100,40,adc_rd,RED,WHITE);


		rem_volume = (((adc_rd - __0_ml_ADC )/__1_ml_ADC)*10.0);//one_ml[sy_size])*10);  //one_ml value
		//TFT_writenumber_float_1d(100,10,(float)rem_volume,RED,WHITE);





		if((int)rem_prev_value!=(int)rem_volume)
		{
			if(!sy_size ||rem_volume<0.0)
			{
				rem_volume=0.0;
			}

			Font_Select(ARIAL_12);
			sprintf(local_buffer,"%.1f",((float)rem_prev_value/10.0));    //store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,289);                 //(uint8_t*)letter
			TFT_writenumber_float_1d(xpos,148,(float)rem_prev_value,LIGHT_GRAY_5,LIGHT_GRAY_5);   //Infused_ml

			sprintf(local_buffer,"%.1f",((float)rem_volume/10.0));    //store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,289);                 //(uint8_t*)letter
			TFT_writenumber_float_1d(xpos,148,(float)rem_volume,LIGHT_GRAY_5,DARK_TEAL);   //Infused_ml

			Font_Select(ARIAL_9);
			TFT_String(292,153,"ml",LIGHT_GRAY_5,DARK_TEAL);

			rem_prev_value=rem_volume;
		}


			rem_plot=((int)rem_volume);

//			draw_rem_frame();
//			if(normal_run)

			 syringe_live_status(sy_size,rem_plot);


//			if(normal_run)
//				{
////				//rem_plot=ADC_read(ADC_CH_PLUNGER_POT);
////				//if(sl_pullout_ADC>=adc_rd)
//
//				for(rem=syr_val[sy_size];rem>rem_plot;rem--)
//				  {
//
//					 var=(int)(syr_pix[sy_size]*rem);
//					 rem_vol_frame(238,14+(var),1);
//
//
//				  }

//				}
			// rem_vol_frame(238,(238),0);
//			 for(int rem=1;rem<=50;rem++)
//			 {
//				 rem_vol_frame(238,(rem),1);
//				 delay_ms(700);
//			 }


//			Font_Select(ARIAL_14);
//		 TFT_Number(10,50,rem_plot,YELLOW,BLACK);
//		 Font_Select(ARIAL_14);
//		 			 	TFT_Number(10,130,syr_pix[sy_size],WHITE,BLACK);
//		 			 	Font_Select(ARIAL_14);
//		 				TFT_Number(10,160,syr_val[sy_size],WHITE,BLACK);
//		 				Font_Select(ARIAL_14);
//		 				TFT_Number(10,180,var,WHITE,BLACK);
	//	 TFT_Number(100,100,__0_ml_ADC,YELLOW,BLACK);
	////	 TFT_Number(10,100,zerp_ml_ADC,YELLOW,BLACK);
	//	 TFT_Number(200,100,__1_ml_ADC,YELLOW,BLACK);


//   return rem_volume;
}

void syringe_live_status(uint8_t sy_size,uint16_t rem_vol)
{
	uint8_t draw_pos=0;


	float syr_pix[8]={0.0,11.2,4.48,2.24,1.12,0.75,0.45,0.0};
	uint16_t syr_vol[8]={0,20,50,100,200,300,500,0};

	static uint16_t prev_rem_vol=0;

	rem_vol=(rem_vol/2)*2;

	if(rem_vol>syr_vol[sy_size])
	{
		rem_vol=syr_vol[sy_size];
	}

	if(sy_size!=5)
	{


	draw_pos=rem_vol*syr_pix[sy_size];
	if(draw_pos>224)
	{
		draw_pos=224;
	}

	 Font_Select(ARIAL_14);
	 if(rem_vol!=prev_rem_vol)
	 {
		 if(prev_rem_vol<rem_vol)
		 	{
		 		for(uint8_t mov=0;mov<draw_pos;mov++)
		 		{

		 		semi_circle(1,14+mov,156,6,ROYAL_BLUE,ROYAL_BLUE);

		 //		Rectangle(15,151,mov,10,0xD9D9D9);
		 		//semi_circle(1,mov+1,156,6,0X00BAD1,0X00BAD1);

		 		}
		 	}
		 	else if(prev_rem_vol>rem_vol)
		 	{

		 		for(uint8_t mov=224;mov>draw_pos;mov--)
					{

					semi_circle(0,14+mov+4,156,6,ASH,ASH);

			//		Rectangle(15,151,mov,10,0xD9D9D9);
					//semi_circle(1,mov+1,156,6,0X00BAD1,0X00BAD1);

			//			delay_ms(50);
					}

		 	}
		 prev_rem_vol=rem_vol;

	 }
	}

}

/****************************************************************************
* Function Name : void syringe_position_check()
* Arguments     : void
* Return type   : void
* Description   : these function checks the near empty  adc ,syringe end  value for all size syringes

*****************************************************************************/
void syringe_position_check()
{
	uint16_t ADC_plunger=0;
	uint8_t ssize=0;
	ADC_plunger = ADC_read(ADC_CH_PLUNGER_POT);
	Sample_value = ADC_plunger;

	ssize=detect_syringe_size(1);
	if(!LIN_POT.syr_end_flag)            //&& end_flag==0)
	{

		near_empty_check(ADC_plunger);

	}
	syringe_end_check(ADC_plunger,ssize);

}


uint16_t LIN_POT_sample(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples )
{
	LINEAR_sample.cumm_data=0;
	if(!LINEAR_sample.current_pos)
	{
		for(LINEAR_sample.count=0;LINEAR_sample.count<samples;LINEAR_sample.count++)
		{
			LINEAR_sample.Raw_data[LINEAR_sample.count]=Raw_val;
		}
		LINEAR_sample.current_pos=samples-1;
		LINEAR_sample.Err_flag=0;
		LINEAR_sample.Fail_count=0;
		LINEAR_sample.Sample_data=Raw_val;
	}

	else if((Raw_val>=Data_diff && Raw_val>(LINEAR_sample.Raw_data[LINEAR_sample.current_pos]-Data_diff) &&

	Raw_val<(LINEAR_sample.Raw_data[LINEAR_sample.current_pos]+Data_diff) )
				||	( Raw_val<=Data_diff && Raw_val>0 && Raw_val<(LINEAR_sample.Raw_data[LINEAR_sample.current_pos]+Data_diff) ))

	{

	for(LINEAR_sample.count=0;LINEAR_sample.count<samples-1;LINEAR_sample.count++)
	{
		LINEAR_sample.Raw_data[LINEAR_sample.count]=LINEAR_sample.Raw_data[LINEAR_sample.count+1];
	}

	LINEAR_sample.Raw_data[LINEAR_sample.current_pos]=Raw_val;


	for(LINEAR_sample.count=0;LINEAR_sample.count<samples;LINEAR_sample.count++)
	{
		LINEAR_sample.cumm_data+=LINEAR_sample.Raw_data[LINEAR_sample.count];
	}

	LINEAR_sample.Sample_data=LINEAR_sample.cumm_data/samples;

	}
	else
	{
		LINEAR_sample.current_pos	=	0;
	}
	return LINEAR_sample.Sample_data;

}
