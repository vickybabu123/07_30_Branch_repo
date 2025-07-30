

#include "LPC55S16.h"
#include"FR_CALC.h"
#include <infused_calculate.h>
//#include "Last_infusion.h"
#include "Power_off.h"


void last_infused_data(uint8_t modes);
extern uint8_t  previous_data(uint8_t mode);
extern void ramp_para_reset(void);

void last_infused_data(uint8_t modes)
{

	uint8_t last_infusion_flag=0;
	last_infusion_flag=previous_data(1);


		if(last_infusion_flag)
		{
			if(modes==0)
			{
				MOT.volume=0;
				INFUSED.ml=0;
				INFUSED.sec=0;
			}
			else if(modes==1)
			{
				INFUSED.ml=0;
				MOT.volume=0;
				INFUSED.sec=0;
				MOT.flowrate=1;

			}
			else if(modes==2)
			{
				INFUSED.ml=0;
			}
			else if(modes==3)
			{
				//ramp_para_reset();
				INFUSED.ml=0;
				INFUSED.sec=0;
			}


//		POW.power_status_flag=1;

	}

}



