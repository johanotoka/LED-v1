/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    lab10_LED.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC802.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define RED_LED (9) // Red LED
#define GREEN_LED (12) // Green LED
#define BUTTON_USER (8) // User Button



/*
 * @brief   Application entry point.
 */
int main(void) {

	// Set up general GPIO

	SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK | // GPIO is on
			SYSCON_SYSAHBCLKCTRL0_GPIO_INT_MASK); // GPIO Interrupt is on

	SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK |
			SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK); // reset GPIO and GPIO Interrupt

	SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK |
				SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK); // clear reset (bit = 1)

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;

    GPIO->DIRSET[0] = (1UL<<RED_LED); // Set Red LED as output
    GPIO->DIRSET[0] = (1UL<<GREEN_LED); // Set Green LED as output
    GPIO->DIRCLR[0] = (1UL<<BUTTON_USER); // Set User Button as input

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");

        if(GPIO->B[0][BUTTON_USER] && 1){
        	GPIO->SET[0] = (1UL<<GREEN_LED); // turn green LED off
        	GPIO->SET[0] = (1UL<<RED_LED); // turn red LED off
        	delay(); 						// delay
        	GPIO->CLR[0] = (1UL<<RED_LED); // turn red LED on
        	delay(); 						// delay
        }
        else{
        	GPIO->SET[0] = (1UL<<RED_LED); // turn red LED off
        	GPIO->SET[0] = (1UL<<GREEN_LED); // turn green LED off
        	delay();						// delay
        	GPIO->CLR[0] = (1UL<<GREEN_LED); // turn green LED on
        	delay();						// delay
        }
    }
    return 0 ;
}

void delay(){
	for(int i = 0; i < 300000; i++){
	   __NOP();
	}
}
