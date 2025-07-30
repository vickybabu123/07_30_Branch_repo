#include "pin_mux.h"
#include "board.h"
//#include "fsl_debug_console.h"
//#include "fsl_usart.h"

#include <stdbool.h>
#include "fsl_power.h"
#include"UART.h"
#include "core_cm33.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "Driver_Timer.h"
char rx[100];
char send_str[100]={0};
char uart_data[100];
char data_array[50]={0};

uint16_t rx_data;
uint8_t inc=0;
uint8_t rec;
uint8_t UART_read_flag=0;
uint8_t current_data;
uint8_t receive=0;
uint16_t led_1=0xEEEE;


extern uint8_t rec_delay;
extern uint16_t comm_delay;
extern uint8_t data_delay;
extern uint8_t ALS_data;
extern uint16_t Auto_bright_level;
extern uint8_t brightness_mode;
extern uint8_t bright_count;
extern uint8_t bright_ctrl[5];

void UART_req();
void UART7_init(uint32_t baudrate_Bps, uint8_t RX_enable)
 {
	    uint16_t osrval;
	    uint32_t brgval;

	    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
//	    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

	    SYSCON->AHBCLKCTRLSET[1]|= 1<<18;
	   	SYSCON->PRESETCTRLSET[1]|= 1<<18;

//	   	SYSCON->AHBCLKCTRLSET[1]|= 1<<11;
//	   	SYSCON->PRESETCTRLSET[1]|= 1<<11;
//	   	             /* wait until it reads 0b1 */
	   		 while (0u == (SYSCON->PRESETCTRLX[1] & (1<<18)))
	   			{
	   			}

	   	SYSCON->PRESETCTRLCLR[1]|= 1<<18;
	   		 /* wait until it reads 0b0 */
	   		 while ((1<<18) == (SYSCON->PRESETCTRLX[1] & (1<<18)))
	   			 {
	   			 }

//		 while (0u == (SYSCON->PRESETCTRLX[1] & (1<<11)))
//	   			{
//	   			}
//
//	   	SYSCON->PRESETCTRLCLR[1]|= 1<<11;
	   		 /* wait until it reads 0b0 */
//	   		 while ((1<<11) == (SYSCON->PRESETCTRLX[1] & (1<<11)))
//	   			 {
//	   			 }
	   		FLEXCOMM7->PSELID|= 0x11;
	   		USART7->FIFOCFG   = 0x03;
	   		USART7->CFG       = 0x04;
	   		//USART7->CFG      |= (1<<0);
	   		USART7->FIFOTRIG = 0X1U;

			if(baudrate_Bps==9600)
			{
				osrval=9;
				brgval=124;
			}
			else if(baudrate_Bps==115200)
			{
			    osrval=12;
			    brgval=7;
			}
			else if(baudrate_Bps==57600)
			{
				osrval=15;
				brgval=12;
			}
			else if(baudrate_Bps==4800)
			{
				osrval=9;
				brgval=249;
			}
			USART7->OSR = osrval;//12
			USART7->BRG =brgval;//7

		if(RX_enable==1)
		{

		 USART7->INTENSET     = 0x00;
		 USART7->FIFOINTENSET = 0x08;
		 NVIC_SetPriority(FLEXCOMM7_IRQn,1);
		 NVIC_EnableIRQ(FLEXCOMM7_IRQn);
		}
		USART7->CFG |=1<<0;
 }

void txdata_transfer(USART_Type *base,char data)
 {

	 while ((0U == (base->FIFOSTAT & (1<<4))))
	     {

	     }
     base->FIFOWR = data;

     while ((!(1<<3) == (base->STAT & (1<<3))))
     {

     }

 }

void uart_string_transmit(USART_Type *base,char *str)
 {
 	while(*str!='\0')
 	txdata_transfer(base,*str++);
 }




void FLEXCOMM7_IRQHandler(void)
{
	 receive++;
     /* Receive data */
     if (1 && ((USART7->FIFOSTAT & USART_FIFOSTAT_RXNOTEMPTY_MASK) != 0U))
     {
         /* Clear address detect when RXFIFO has data. */
         USART7->CTL &= ~(uint32_t)USART_CTL_ADDRDET_MASK;
         {
        	 if(!UART_read_flag)
        	 {
        		 rx[inc]         = (uint8_t)USART7->FIFORD;

             inc++;

				 if(inc>=10 || rx[inc-1]==' ')
				 {
					 UART_read_flag=1;
					 inc = 0;
				 }
        	 }
        	 else
        	 {
        		 rec=(uint8_t)USART7->FIFORD;
        	 }
         }
      }
 }

void UART_req()
{
	uint16_t led_1=0x0001;

	 if(comm_delay==0)
	  {
//		  comm_delay=100;                         //delay for continuous transmit
//
//		  uart_string_transmit(USART7,"!");       //Auto brightness mode
//		  printf("%s\n","REQ");
//		  comm_delay=100;

		  uart_string_transmit(USART7,"#");   //LED_DRIVER
//		  printf("%s\n","REQ_1");
		  comm_delay=100;
		  while(comm_delay);

		  txdata_transfer(USART7,(led_1>>8));
//		  comm_delay=100;
//		  while(comm_delay);

 //		   printf("%s\n","REQ_2");
		   txdata_transfer(USART7,(led_1>>0));
		   comm_delay=100;
		  while(comm_delay);
//		  printf("%s\n","REQ_1");
//		  uart_string_transmit(USART7,KEY_LED);
//		 txdata_transfer(USART7,0xFF);
//		  comm_delay=100;
//		  while(comm_delay);
		  data_delay=2;                           //delay for the transmit failure
	  }
//	 if(UART_read_flag==1)
//	 	{
//	 		if(rx[0]>48)
//	 		{
//	 			Auto_bright_level=rx[0]-48;                       //For receiving Integer value from the char
//	 		}
//
//	 		TFT_String(180,150,(uint8_t*)Auto_bright_level,RED,YELLOW);
//	 		UART_read_flag=0;
//	 	}
//	 if(!brightness_mode)                        //brightness off state
//	 {
//		 {
//			 GPIO->CLR[1]|=(1<<4);
//			 Auto_bright_level=0;
//			 comm_delay=100;
//			 bright_ctrl[5]=0;
//		 }
//	 }
   }

void uart_frame_func(char* start_str,uint16_t data_var,char* end_str)
{
		Font_Select(ARIAL_14);
		memset(send_str,'\0',100);       //START CHAR FRAME
		memset(uart_data,'\0',100);
		strncpy(send_str,start_str,1);
		strcat(uart_data,send_str);

		memset(send_str,'\0',100);
		sprintf(send_str,"%d",data_var);        //INTEGER FRAME
		strcat(uart_data,send_str);

		memset(send_str,'\0',100);         //END CHAR FRAME
		strncpy(send_str,end_str,1);
		strcat(uart_data,send_str);
		//strcat(uart_data,data_array);
		//memset(uart_data,'\0',100);
		uart_string_transmit(USART7,uart_data); //SEND COMPLETE FRAME

//		TFT_String(150,150,uart_data,RED,BLACK);
//		delay_secs(1);
//	//  delay_1secs();
//		TFT_String(150,150,uart_data,RED,RED);
}
//
