
#include"lpc55s16.h"
#include "Linear_Header.h"
#include "Driver_ADC.h"
#include "Color.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "SD_Calib.h"
#include "Motor.h"
#include "Alarm.h"
#include"Linear_pot.h"
#include"FR_CALC.h"
pot_clamp CLAM;


void linear_pot(uint16_t adc_read)
{
//    static uint16_t pre_current = 0;
//    static uint8_t repeat_count = 0;
    uint16_t adc_current = adc_read / 100;

    if (adc_current % 2 != 0) {
        adc_current = adc_current - 1;



    if ((adc_current % 2) == 0)
    {
//    	        TFT_Number(100, 120, CLAM.linear_pot_pulse, RED, YELLOW);
//    	        TFT_Number(100, 40, CLAM.pre_current, RED, YELLOW);
//    	        TFT_Number(100, 80, adc_current, RED, YELLOW);
//    	        TFT_Number(50, 50, CLAM.repeat_count, RED, YELLOW);
        if (CLAM.linear_pot_pulse >= 4000)
        {

            if (adc_current != CLAM.pre_current)
            {
            	CLAM.pre_current = adc_current;
            	CLAM.repeat_count = 1;
            }
            else
            {


                if (RAMP.RPM_control <=10)
                {
                    Alarm(LIN_POT_ERR);
                    CLAM.repeat_count = 0;
                }
                else if (++CLAM.repeat_count >= 2)
                {
                    Alarm(LIN_POT_ERR);
                    CLAM.repeat_count = 0;
                }
            }

            CLAM.linear_pot_pulse = 0;
        }
    }
    }
}


Reset_linear_pot()
{
	  CLAM.linear_pot_pulse = 0;
	   CLAM.repeat_count = 0;
	   CLAM.pre_current=0;
}

