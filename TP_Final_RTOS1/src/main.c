/*============================================================================
 * Autor: Luciano Perren <lgperren@gmail.com>
 * TP Final RTOS 1
 * Date: 2019/20/08
 * Docentes: Franco Bucafusco y Sergio Renato De Jesús Melean
 * main.c
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
//#include "FreeRTOS.h"			// Motor del SO
#include "FreeRTOSConfig.h"
#include "task.h"				// API de control de tareas y temporizacion
#include "semphr.h"				// API de sincronización (sem y mutex)

// sAPI header
#include "sapi.h"

// INA219 header
#include "../../../mis_programas/TP_Final_RTOS1/inc/ina219.h"

// tareas header
#include "../../../mis_programas/TP_Final_RTOS1/inc/tareas.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/* Declara una variable del tipo QueueHandle_t. Esta variable es usada para
enviar mensajes desde  "" a "task_show_values"      */

static QueueHandle_t i2cQueue;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// Funciones declaradas en "tareas.h"

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();

    // UART for debug messages
    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "TP Final con freeRTOS y sAPI." );

    uint8_t Error_state = 0;

    /* Antes de que la cola sea usada debe ser creada. La cola es creada para
       almacenar un máximo de 5 punteros a caracteres. */
    if ( NULL == ( i2cQueue = xQueueCreate( 5, sizeof( char * ) ) ){
    		Error_state = 1;
    }
    // Se crea la "tarea_print_measurement" en freeRTOS y se instancia 4 veces
    // 1st instance
    xTaskCreate(
    		tarea_print_measurement,                     // Funcion de la tarea a ejecutar
    		(const char *)"Print_measurement_1",	     // Nombre de la tarea como String amigable para el usuario
    		configMINIMAL_STACK_SIZE*2, 				 // Cantidad de stack de la tarea
    		1,                          				 // Parametros de tarea
    		tskIDLE_PRIORITY+1,         				 // Prioridad de la tarea
    		0                           				 // Puntero a la tarea creada en el sistema
            );

    // 2nd instance
    xTaskCreate(
      		tarea_print_measurement,
    		(const char *)"Print_measurement_2",
    		configMINIMAL_STACK_SIZE*2,
    		1,
    		tskIDLE_PRIORITY+2,
    		0
           	);

    // 3rd instance
    xTaskCreate(
       		tarea_print_measurement,
    		(const char *)"Print_measurement_3",
    		configMINIMAL_STACK_SIZE*2,
    		1,
    		tskIDLE_PRIORITY+3,
    		0
           	);

    // 4th instance
    xTaskCreate(
       		tarea_print_measurement,
    		(const char *)"Print_measurement_4",
    		configMINIMAL_STACK_SIZE*2,
    		1,
    		tskIDLE_PRIORITY+4,
    		0
           	);

    // Se crea la "tarea_portero" y es la unica que tiene permitido escribir en la UART
    xTaskCreate(
       		tarea_portero,
       		(const char *)"Gatekeeper",
       		configMINIMAL_STACK_SIZE*2,
       		0,
       		tskIDLE_PRIORITY+4,
       		0
       	    );

    // Iniciar scheduler
       if (0 == Error_state){
      	  vTaskStartScheduler();
       }
       else {
    	  printf("Error al iniciar el sistema !!!!!!!!!!!!!!");
       }

// ---------- REPETIR POR SIEMPRE --------------------------
       while( TRUE ) {
          // Si cae en este while 1 significa que no pudo iniciar el scheduler
       }

       // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
       // directamenteno sobre un microcontroladore y no es llamado por ningun
       // Sistema Operativo, como en el caso de un programa para PC.
       return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// Funciones definidas en "tareas.c"







