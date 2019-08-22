/*============================================================================
 * Autor: Luciano Perren <lgperren@gmail.com>
 * TP Final Protocolos de Comunicación en Sistemas Embebidos
 * Date: 2019/08/07
 * Docentes: Pablo Gomez y Eric Pernia
 * example_ina219.c
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "sapi.h"               // <= sAPI header
#include "../../../mis_programas/ina219/inc/ina219.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

// INA219 Address
INA219_address_t addr = INA219_ADDRESS_0; // If INA219 A0 y A1 pin is connected to GND

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){
   /* ------------- INICIALIZACIONES ------------- */

   boardConfig();
   uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB

   // Inicializar INA219
   printf("Inicializando INA219...\r\n" );
   bool_t status;
   status = ina219Init( addr );

   if( status == FALSE ){
      printf( "INA219 no inicializado, verifique las conexiones:\r\n\r\n" );
      printf( "Se detiene el programa.\r\n" );
      while(1);
   }
   printf("INA219 inicializado correctamente.\r\n\r\n" );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE){

      //Leer registros del INA219
	  ina219Read();

	  // Imprimir los resultados
	  printf( "INA219 shunt voltage: %1.2f [mV]\r\n", ina219ShowShuntVoltage() );
	  printf( "INA219 bus voltage: %.3f [V]\r\n", ina219ShowBusVoltage() );
	  printf( "INA219 power: %.2f [mW]\r\n", ina219ShowPower() );
	  printf( "INA219 current: %.2f [mA]\r\n\r\n", ina219ShowCurrent () );

	  delay(500);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0;
}

/*==================[end of file]============================================*/


