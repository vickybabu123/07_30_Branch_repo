/*
 * Copyright 2017-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********/

/**
 * @file    pin_mux.c
 * @brief   Board pins file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include "pin_mux.h"
#include"LPC55S16.h"
#include"fsl_clock.h"
/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */


__STATIC_INLINE void IOCON_PinMuxSet(IOCON_Type *base, uint8_t port, uint8_t pin, uint32_t modefunc)
{
    base->PIO[port][pin] = modefunc;
}

void BOARD_InitBootPins(void) {
	/* The user initialization should be placed here */
	CLOCK_EnableClock(kCLOCK_Iocon);

		/*For LPCXpresso EVK pins*/
		    IOCON->PIO[1][24] = ((IOCON->PIO[1][24] &
		                          /* Mask bits to zero which are setting */
		                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

		                         /* Selects pin function.
		                          * : PORT124 (pin 3) is configured as FC2_RXD_SDA_MOSI_DATA. */
		                         | IOCON_PIO_FUNC(PIO1_24_FUNC_ALT1)

		                         /* Select Digital mode.
		                          * : Enable Digital mode.
		                          * Digital input is enabled. */
		                         | IOCON_PIO_DIGIMODE(PIO1_24_DIGIMODE_DIGITAL));
		 /*For 1100 controller Rx pin*/
		    IOCON->PIO[1][29] = ((IOCON->PIO[1][29] &
		                                    /* Mask bits to zero which are setting */
		                                    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

		                                   /* Selects pin function.
		                                    * : PORT124 (pin 3) is configured as FC2_RXD_SDA_MOSI_DATA. */
		                                   | IOCON_PIO_FUNC(PIO1_29_FUNC_ALT1)

		                                   /* Select Digital mode.
		                                    * : Enable Digital mode.
		                                    * Digital input is enabled. */
		                                   | IOCON_PIO_DIGIMODE(PIO1_29_DIGIMODE_DIGITAL));
		    /*For 1100 controller Tx pin*/
		    const uint32_t port1_pin30_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
		                                             IOCON_PIO_FUNC1 |
		                                             /* No addition pin function */
		                                             IOCON_PIO_MODE_INACT |
		                                             /* Standard mode, output slew rate control is enabled */
		                                             IOCON_PIO_SLEW_STANDARD |
		                                             /* Input function is not inverted */
		                                             IOCON_PIO_INV_DI |
		                                             /* Enables digital function */
		                                             IOCON_PIO_DIGITAL_EN |
		                                             /* Open drain is disabled */
		                                             IOCON_PIO_OPENDRAIN_DI);
		        /* PORT0 PIN30 (coords: 94) is configured as FC0_TXD_SCL_MISO_WS */
		        IOCON_PinMuxSet(IOCON, 1U, 30U, port1_pin30_config);

}
