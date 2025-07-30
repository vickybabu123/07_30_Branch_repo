#include <Boot_up.h>
#include "LPC55S16.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Main_screen.h"
#include "fsl_clock.h"
void PWM_Init(void);

extern uint8_t led;
void PWM_Init()
{
    SYSCON->CTIMERCLKSELX[0] = 0x03;       // 96 clock
    CLOCK_AttachClk(kPLL0_to_CTIMER3);   // Attach FRO_HF to CTIMER3
    IOCON->PIO[1][19] = 0x003;              // CTIMER3_MAT1
    SYSCON->AHBCLKCTRLX[2] |= 1 << 21;     // Enable clock for Timer 3
    CTIMER3->PWMC=(1<<3);
    CTIMER3->PR = 0x00;                    // No prescaler
    CTIMER3->PWMC |= 1 << 1;               // PWM mode for MAT1

    CTIMER3->EMR=500;//130             // Set MR0 to 130
    CTIMER3->MR[3] = 1400;//130             // Set MR0 to 130
    CTIMER3->MR[2] = 500;//130             // Set MR0 to 130

    CTIMER3->MCR		|= 1U<<0;		   // Interrupt on Match0 compare
    CTIMER3->MCR		|= 1U<<1;		   // Reset timer on Match 0.
    CTIMER3->IR |= (1 << 0);               // Clear interrupt flag
    NVIC_SetPriority(CTIMER3_IRQn,0);



//    NVIC_EnableIRQ(CTIMER3_IRQn);
    CTIMER3->TCR = 0x1U;                  //start
}


void CTIMER3_IRQHandler(void)
{
//	static uint8_t count=0;
	if (CTIMER3->IR& (1<<3))
	{
		CTIMER3->IR|=(1 <<3); // Clear interrupt flag

		if(led==0)
		{
			GPIO->CLR[1]|=1<<19;
			led=1;
		}
		else
		{
			GPIO->SET[1]|=1<<19;
			led=0;
		}
	  }
}

