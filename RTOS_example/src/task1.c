/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "../../RTOS_example/inc/task1.h"  // <= own header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

// Funcion que inicializa la tarea
void task1_Init( int32_t pin ){
   gpioConfig(pin, GPIO_OUTPUT);
}

void task2_Init( int32_t pin ){
	gpioConfig(pin, GPIO_INPUT);
}

// Funcion que se ejecuta periodicamente
void task1_Update( void* taskParam ){   
   gpioToggle( (int32_t)taskParam );
}

/*==================[end of file]============================================*/
