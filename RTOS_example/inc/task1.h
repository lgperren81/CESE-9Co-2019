/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

#ifndef _TASK1_H_
#define _TASK1_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"   // <= sAPI header

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// Funcion que inicializa las tareas
void task1_Init( int32_t pin );

void task2_Init( int32_t pin);

// Funcion que se ejecuta periodicamente
void task1_Update( void* taskParam );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _TASK1_H_ */
