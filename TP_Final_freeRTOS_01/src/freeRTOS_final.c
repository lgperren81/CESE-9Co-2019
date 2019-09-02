/*============================================================================
 * Autor: Luciano Perren <lgperren@gmail.com>
 * TP Final RTOS 1
 * Date: 2019/20/08
 * Docentes: Franco Bucafusco y Sergio Renato De Jesús Melean
 * freeRTOS_final.c
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"			// Motor del SO
#include "FreeRTOSConfig.h"
#include "task.h"				// Api de control de tareas y temporización
#include "semphr.h"				// Api de sincronización (sem y mutex)
#include "queue.h"				// Api de colas

// sAPI header
#include "sapi.h"
// INA219 header
#include "ina219.h"
#include "board.h"


/*==================[definiciones y macros]==================================*/

enum Teclas_t {Tecla1, Tecla2, Tecla3, Tecla4}; //Índices de teclas para el vector de estructura

/*==================[definiciones de datos internos]=========================*/

// INA219 Address
INA219_address_t addr = INA219_ADDRESS_0; // If INA219 A0 y A1 pin is connected to GND

static QueueHandle_t INA219Queue;
static QueueHandle_t teclaQueue;

uint8_t teclaPress;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

//Función de inicialización de interrupciones
void My_IRQ_Init (void);

/*==================[declaraciones de funciones externas]====================*/

// Tarea que realiza la lectura de los registros del INA219 y los pasa a la cola
void task_read_measurement( void* taskParmPtr );
// Tarea que recibe los valores de la cola y los convierte de "float" a "string", luego los envia
// por la UART y al display LCD
void task_show_values( void* taskParmPtr );
// Tarea que muestra en el LCD que tecla se oprimio
void task_show_interrupt ( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
	uint8_t Error_state = 0;

	//----------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   //Iniciamos las interrupciones
   My_IRQ_Init();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Final RTOS 1" );
   gpioWrite( LED3, ON );		// Its Alive!!!

   // Se crea cola capaz de contener 5 array de 4 valores del tipo float
   if ( NULL == ( INA219Queue = xQueueCreate( 5, sizeof( float[ 4 ] ) ) ) ) {
	   Error_state = 1;
   }
   // Se crea cola capaz de contener 4 valores del tipo uint8_t
   if ( NULL == ( teclaQueue = xQueueCreate( 4, sizeof( uint8_t ) ) ) ) {
	   Error_state = 1;
   }
  	   // Crear tareas en freeRTOS
       xTaskCreate(
          task_read_measurement,      		 // Funcion de la tarea a ejecutar
          (const char *)"readINA219",        // Nombre de la tarea como String amigable para el usuario
          configMINIMAL_STACK_SIZE*5,        // Cantidad de stack de la tarea
          0,                                 // Parametros de tarea
          tskIDLE_PRIORITY+3,                // Prioridad de la tarea
          0                                  // Puntero a la tarea creada en el sistema
       );

       xTaskCreate(
          task_show_values,      		     // Funcion de la tarea a ejecutar
          (const char *)"printUART",         // Nombre de la tarea como String amigable para el usuario
          configMINIMAL_STACK_SIZE*10,       // Cantidad de stack de la tarea
          0,                                 // Parametros de tarea
          tskIDLE_PRIORITY+2,                // Prioridad de la tarea
          0                                  // Puntero a la tarea creada en el sistema
       );

       xTaskCreate(
          task_show_interrupt,     		     // Funcion de la tarea a ejecutar
          (const char *)"printInterrupt",    // Nombre de la tarea como String amigable para el usuario
          configMINIMAL_STACK_SIZE*5,       // Cantidad de stack de la tarea
          0,                                 // Parametros de tarea
          tskIDLE_PRIORITY+1,                // Prioridad de la tarea
          0                                  // Puntero a la tarea creada en el sistema
       );

   // Iniciar scheduler
   if ( 0 == Error_state) {
   vTaskStartScheduler();
   } else {
	   uartWriteString( UART_USB, "No se pudo iniciar el sistema correctamente\r\n" );
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
//Función de inicialización de IRQs
void My_IRQ_Init (void){
		//Inicializamos las interrupciones (LPCopen)
		Chip_PININT_Init(LPC_GPIO_PIN_INT);

		// Inicializamos de cada evento de interrupción (LPCopen)
		// Mapeo del pin donde ocurrirá el evento y el canal al que lo va a enviar
		// Canal 0 a 7, Puerto GPIO, Pin GPIO)
		Chip_SCU_GPIOIntPinSel(0, 0, 4); 		// TEC_1
		//Se configura el canal para que se active por flanco
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH0);
		//Se configura para que el flanco sea el de subida
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH0);

		Chip_SCU_GPIOIntPinSel(1, 0, 8);		// TEC_2
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH1);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH1);

		Chip_SCU_GPIOIntPinSel(2, 0, 9);		// TEC_3
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH2);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH2);

		Chip_SCU_GPIOIntPinSel(3, 1, 9);		// TEC_4
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH3);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH3);

		// Una vez que se han configurado los eventos para cada canal de interrupcion
		// se activan las interrupciones para que comiencen a llamar al handler
		NVIC_SetPriority(PIN_INT0_IRQn, 7);
		NVIC_EnableIRQ(PIN_INT0_IRQn);
		NVIC_SetPriority(PIN_INT1_IRQn, 7);
		NVIC_EnableIRQ(PIN_INT1_IRQn);
		NVIC_SetPriority(PIN_INT2_IRQn, 7);
		NVIC_EnableIRQ(PIN_INT2_IRQn);
		NVIC_SetPriority(PIN_INT3_IRQn, 7);
		NVIC_EnableIRQ(PIN_INT3_IRQn);
}

/*==================[definiciones de funciones externas]=====================*/

/* Implementacion de las funciones de cada tarea */

/* Tarea periodica que realiza la Lectura de los registros del INA219 cada 200ms
 * y carga los valores en una cola  */
void task_read_measurement( void* taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------

	bool_t status;				// Si "status" = TRUE se inicializo correctamente
	float dataInBuffer[ 4 ];	// Array donde se almacenan las lecturas del INA219

	/* Inicializar INA219 */
   uartWriteString( UART_USB, "Inicializando INA219...\r\n" );
   vTaskDelay( 1000 / portTICK_RATE_MS );

   status = ina219Init( addr );			// Inicializo INA219
   	   if( status == FALSE ){
	   	   uartWriteString( UART_USB, "INA219 no inicializado, verifique las conexiones:\r\n\r\n" );
	   	   uartWriteString( UART_USB, "Se detiene el programa.\r\n" );
	   	   }
   uartWriteString( UART_USB, "INA219 inicializado correctamente\r\n" );

   /* Tarea periodica cada 200 ms */
   portTickType xPeriodicity =  200 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while(TRUE) {
      ina219Read();			// Funcion que reliza la lectura de los registros del INA219 (ina219.h)

      /* Almaceno la lectura de los registros */
	  dataInBuffer[ 0 ] = ina219ShowShuntVoltage();		// Shunt voltage
	  dataInBuffer[ 1 ] = ina219ShowBusVoltage();		// Bus voltage
	  dataInBuffer[ 2 ] = ina219ShowPower();			// Power
	  dataInBuffer[ 3 ] = ina219ShowCurrent();			// Current

	  xQueueSend( INA219Queue, (void *) dataInBuffer, 0 );

	  // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
	  vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}

/* Esta tarea recibe los valores de la cola y los pasa de "float" a "string"
 * para luego enviarlos por la UART y al display LCD */
void task_show_values( void* taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar LCD de 20x4 (caracteres x lineas) con cada caracter de 5x8 pixeles
   lcdInit( 20, 4, 5, 8 );
   lcdClear();

   float dataReceive[ 4 ];
   char shuntVoltageString[ 6 ];
   char busVoltageString[ 6 ];
   char powerString[ 6 ];
   char currentString[ 6 ];

   // Tarea periodica cada 1000 ms
   portTickType xPeriodicity =  1000 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();

   // ---------- REPETIR POR SIEMPRE --------------------------
   /* Para NO utilizar "printf" convierto de "float" a "string" y utilizo la función
    * "uartWriteString" de la SAPI */
   while(TRUE) {
	   if (xQueueReceive( INA219Queue, (void *)dataReceive, portMAX_DELAY )){
		  lcdClear();
		  // Paso de "float" a "string"
		  floatToString( dataReceive[0], shuntVoltageString, 3 );
		  floatToString( dataReceive[1], busVoltageString, 3 );
		  floatToString( dataReceive[2], powerString, 3 );
		  floatToString( dataReceive[3], currentString, 3 );
		  // Muestro valor por la UART
		  uartWriteString( UART_USB, "INA219 Shunt Voltage [mV]:\r\n" );
		  uartWriteString( UART_USB, shuntVoltageString );
		  uartWriteString( UART_USB, "\r\n" );
		  // Muestro valor por el LCD
		  lcdGoToXY( 0, 0 );
		  lcdSendStringRaw( "Shunt Voltage: " );
		  lcdSendStringRaw( shuntVoltageString );
		  // Muestro valor por la UART
		  uartWriteString( UART_USB, "INA219 Bus Voltage [mV]:\r\n" );
		  uartWriteString( UART_USB, busVoltageString );
		  uartWriteString( UART_USB, "\r\n" );
		  // muestro valor por el LCD
		  lcdGoToXY( 0, 1 );
		  lcdSendStringRaw( "Bus Voltage: " );
		  lcdSendStringRaw( busVoltageString );
		  // Muestro valor por la UART
		  uartWriteString( UART_USB, "INA219 Power [mW]:\r\n");
		  uartWriteString( UART_USB, powerString );
		  uartWriteString( UART_USB, "\r\n" );
		  // Muestro valor por el LCD
		  lcdGoToXY( 0, 2 );
		  lcdSendStringRaw( "Power: " );
		  lcdSendStringRaw( powerString );
		  // Muestro valor por la UART
		  uartWriteString( UART_USB, "INA219 Current [mA]:\r\n");
		  uartWriteString( UART_USB, currentString );
		  uartWriteString( UART_USB, "\r\n" );
		  // Muestro valor por el LCD
		  lcdGoToXY( 0, 3 );
		  lcdSendStringRaw( "Current: " );
		  lcdSendStringRaw( currentString );
		  lcdCursorSet( LCD_CURSOR_OFF ); // Apaga el cursor
	   }
	   // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
	   vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}

/* Esta tarea recibe de la cola de interrupción que tecla fue
 * oprimida y la muestra por el display LCD */
void task_show_interrupt( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------

	// Inicializar LCD de 20x4 (caracteres x lineas) con cada caracter de 5x8 pixeles
	lcdInit( 20, 4, 5, 8 );
	lcdClear();

	uint8_t tecla;

	// ---------- REPETIR POR SIEMPRE --------------------------
	while (TRUE){
		//Espero evento de Lectura completa
		if (xQueueReceive( teclaQueue, &tecla, portMAX_DELAY )){
			// Muestro en el display LCD que tecla se oprimio
			if( tecla == Tecla1 ){
			   lcdClear();
			   lcdGoToXY( 0, 0 );
			   lcdSendStringRaw( "Se oprimio TEC 1" );
			   lcdCursorSet( LCD_CURSOR_OFF );
			   vTaskDelay( 1000 / portTICK_RATE_MS );
		   }

		   if( tecla == Tecla2 ){
			   lcdClear();
			   lcdGoToXY( 0, 0 );
			   lcdSendStringRaw( "Se oprimio TEC 2" );
			   lcdCursorSet( LCD_CURSOR_OFF );
			   vTaskDelay( 1000 / portTICK_RATE_MS );
		   }

		   if( tecla == Tecla3 ){
			   lcdClear();
			   lcdGoToXY( 0, 0 );
			   lcdSendStringRaw( "Se orpimio TEC 3" );
			   lcdCursorSet( LCD_CURSOR_OFF );
			   vTaskDelay( 1000 / portTICK_RATE_MS );
		   }

		   if( tecla == Tecla4 ){
			   lcdClear();
			   lcdGoToXY( 0, 0 );
			   lcdSendStringRaw( "Se orpimio TEC 4");
			   lcdCursorSet( LCD_CURSOR_OFF );
			   vTaskDelay( 1000 / portTICK_RATE_MS );
		   }
		}
	}
}

void GPIO0_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH0){ //Verificamos que la interrupción es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0);  //Borramos el flag de interrupción
		//codigo a ejecutar si ocurrió la interrupción
		teclaPress = Tecla1;
		xQueueSendFromISR( teclaQueue, &teclaPress, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO1_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH1){ //Verificamos que la interrupción es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);  //Borramos el flag de interrupción
		//codigo a ejecutar si ocurrió la interrupción
		teclaPress = Tecla2;
		xQueueSendFromISR( teclaQueue, &teclaPress, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO2_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH2){ //Verificamos que la interrupción es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH2);  //Borramos el flag de interrupción
		//codigo a ejecutar si ocurrió la interrupción
		teclaPress = Tecla3;
		xQueueSendFromISR( teclaQueue, &teclaPress, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO3_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH3){ //Verificamos que la interrupción es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH3);  //Borramos el flag de interrupción
		//codigo a ejecutar si ocurrió la interrupción
		teclaPress = Tecla4;
		xQueueSendFromISR( teclaQueue, &teclaPress, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/*==================[fin del archivo]========================================*/
