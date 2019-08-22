/*
 * leds.h
 *
 *  Created on: 23 may. 2019
 *      Author: lgperren
 */

#ifndef MIS_PROGRAMAS_PDM_EJER1_SEMAFORO_INC_LEDS_H_
#define MIS_PROGRAMAS_PDM_EJER1_SEMAFORO_INC_LEDS_H_

#include "sapi.h"

#define ROJO LED2
#define AMARILLO LED1
#define VERDE LED3

#define TIEMPO_ROJO 1000
#define TIEMPO_ROJO_AMARILLO 500
#define TIEMPO_AMARILLO 1000
#define TIEMPO_VERDE 1000

void encender_1_led(gpioMap_t led, uint32_t milisec);
void encender_2_led(gpioMap_t led1, gpioMap_t led2, uint32_t milisec);

#endif /* MIS_PROGRAMAS_PDM_EJER1_SEMAFORO_INC_LEDS_H_ */
