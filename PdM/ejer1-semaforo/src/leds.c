/*
 * leds.c
 *
 *  Created on: 23 may. 2019
 *      Author: lgperren
 */

#include "../../../PdM/ejer1-semaforo/inc/leds.h"

#include "sapi.h"

void encender_1_led(gpioMap_t led, uint32_t milisec){
	/* Prendo el led azul */
	      gpioWrite( led, ON );

	      delay(milisec);

	      /* Apago el led azul */
	      gpioWrite( led, OFF );

	      delay(milisec);
}

void encender_2_leds(gpioMap_t led1, gpioMap_t led2, uint32_t milisec){
	/* Prendo el led azul */
	      gpioWrite( led1, ON );
	      gpioWrite( led2, ON );

	      delay(milisec);

	      /* Apago el led azul */
	      gpioWrite( led1, OFF );
	      gpioWrite( led2, OFF );

	      delay(milisec);
}
