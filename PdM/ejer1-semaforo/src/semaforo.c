/*
 * semaforo.c
 *
 *  Created on: 23 may. 2019
 *      Author: lgperren
 */
#include "../../../PdM/ejer1-semaforo/inc/semaforo.h"

#include "../../../PdM/ejer1-semaforo/inc/leds.h"

void semaforo_normal(){

	encender_1_led(ROJO, TIEMPO_ROJO);

	encender_2_leds(ROJO, AMARILLO, TIEMPO_ROJO_AMARILLO);

	encender_1_led(AMARILLO, TIEMPO_AMARILLO);

	encender_1_led(VERDE, TIEMPO_VERDE);
}
