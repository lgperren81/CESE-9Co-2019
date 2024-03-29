/*
 * BLE4.0_HM10.c
 *
 *  Created on: 26 jul. 2019
 *      Author: lgperren
 */

#include "sapi.h"
#include <string.h>

#define UART_PC			UART_USB
#define UART_BLUETOOTH	UART_232

bool_t hm10bleTest( int32_t uart);
void hm10blePrintATCommands( int32_t uart);

int main( void )
{
	boardConfig();
	uartConfig( UART_PC, 9600 );
	uartWriteString( UART_PC, "UART_PC configurada.\r\n" );

	uartConfig( UART_BLUETOOTH, 9600 );
	uartWriteString( UART_PC, "UART_BLUETOOTH para modulo Bluetooth configurada.\r\n" );

	uint8_t data = 0;

	uartWriteString( UART_PC, "Testeo si el modulo esta conectado enviando: AT\r\n" );
	if( hm10bleTest( UART_BLUETOOTH ) ){
		uartWriteString( UART_PC, "Modulo conectado correctamente." );
	}

	while( TRUE ){

	// Si leo un dato de una UART lo envio a la otra (bridge)
	if( uartReadByte( UART_PC, &data ) ){
		uartWriteByte( UART_BLUETOOTH, data );
	}
	if( uartReadByte( UART_BLUETOOTH, &data ) ){
		if( data == 'h' ){
			gpioWrite( LEDB, ON );
		}
		if( data == 'l' ){
			gpioWrite( LEDB, OFF);
		}
		uartWriteByte( UART_PC, data );
	}

	// Si presiono TEC1 imprime la lista de comandos AT
	if( !gpioRead( TEC1 ) ){
		hm10blePrintATCommands( UART_BLUETOOTH );
	}

	// Si presiono TEC3 enciende el led de la pantalla de la app
	if( !gpioRead( TEC3 ) ){
		uartWriteString( UART_BLUETOOTH, "LED_ON\r\n");
		delay( 500 );
	}

	//Si presiono TEC4 apaga el led de la pantalla de la app
	if( !gpioRead( TEC4 ) ){
		uartWriteString( UART_BLUETOOTH, "LED_OFF\r\n" );
		delay( 500);
	}
	}
	return(0);
}

bool_t hm10bleTest( int32_t uart )
{
	uartWriteString( uart, "AT\r\n" );
	return waitForReceiveStringOrTimeoutBlocking( uart, "OK\r\n", strlen( "OK\r\n" ), 50);
}

void hm10blePrintATCommands( int32_t uart)
{
	delay( 500 );
	uartWriteString( uart, "AT+HELP\r\n");
}


