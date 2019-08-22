/*============================================================================
 * Autor: Luciano Perren <lgperren@gmail.com>
 * TP Final RTOS 1
 * Date: 2019/20/08
 * Docentes: Franco Bucafusco y Sergio Renato De Jesús Melean
 * tareas.c
 *===========================================================================*/

/*==================[inclusions]=============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"			// Motor del SO
#include "FreeRTOSConfig.h"
#include "task.h"				// API de control de tareas y temporizacion
#include "semphr.h"				// API de sincronización (sem y mutex)

#include "../../../mis_programas/TP_Final_RTOS1/inc/tares.h"		// <= own header
#include "sapi.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void task_show_values( void* taskParmPtr ){

	char *uartMessageToPrint;
	for( ;; ) {
	      /* Wait for a message to arrive. */
	      xQueueReceive( xi2cQueue, &uartMessageToPrint, portMAX_DELAY );
	      uartWriteString( UART_USB, uartMessageToPrint);
	      /* Now simply go back to wait for the next message. */
	}
}
// Funcion que inicializa la tarea
void task1_Init( int32_t pin ){
   gpioConfig(pin, GPIO_OUTPUT);
}

void task2_Init( int32_t pin ){
	gpioConfig(pin, GPIO_INPUT);
}

// Funcion que se ejecuta periodicamente cada 1 segundo
void task_read_measurement( void* taskParmPtr ){

	portTickType xPeriodicity =  1000 / portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

		while( TRUE ) {
			// Función que lee los registros del INA219
			ina219Read();
		}
}
void task1_Update( void* taskParam ){
   gpioToggle( (int32_t)taskParam );
}

/*==================[end of file]============================================*/


