/*============================================================================
 * Autor: Luciano Perren <lgperren@gmail.com>
 * TP Final RTOS 1
 * Date: 2019/20/08
 * Docentes: Franco Bucafusco y Sergio Renato De Jesús Melean
 * tareas.h
 *===========================================================================*/

#ifndef MIS_PROGRAMAS_TP_FINAL_RTOS1_INC_TAREAS_H_
#define MIS_PROGRAMAS_TP_FINAL_RTOS1_INC_TAREAS_H_

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

// Funcion encargada de enviar por la UART la información a mostrar. Es la unica
// funcion que puede acceder a este recurso.
void task_show_values( void* taskParmPtr );

// Función que se ejecuta periodicamente cada 1 segundo y es la encargada de leer los
// registros "shunt voltage", "bus voltage", "power" y "current" del INA219
void task_read_measurement( void* taskParmPtr );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef MIS_PROGRAMAS_TP_FINAL_RTOS1_INC_TAREAS_H_ */



