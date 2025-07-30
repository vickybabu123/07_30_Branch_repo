#ifndef UART_H_
#define UART_H_







#include "LPC55S16.h"
#include "pin_mux.h"
#include "board.h"
//#include "fsl_debug_console.h"
//#include "fsl_usart.h"

#include <stdbool.h>
#include "fsl_power.h"

#define DEMO_USART          USART7
#define DEMO_USART_CLK_SRC  kCLOCK_Flexcomm7
#define DEMO_USART_CLK_FREQ CLOCK_GetFlexCommClkFreq(7U)
#define MR3_VALUE    1500000

//void USART_UserCallback(USART_Type *base, usart_handle_t *handle, status_t status, void *userData);
void UART_init(uint32_t baudrate_Bps, uint8_t RX_enable);
void txdata_transfer(USART_Type *base,char data);
void uart_string_transmit(USART_Type *base,char *str);
void delay_us(unsigned int data);
void FLEXCOMM7_IRQHandler(void);
void CTIMER3_IRQHandler(void);
void Timer3_Init();
void _10_ms_fun();
void UART7_init(uint32_t baudrate_Bps, uint8_t RX_enable);
void uart_frame_func(char* start_str,uint16_t data_var,char* end_str);

#endif /* PWM_H_ */
