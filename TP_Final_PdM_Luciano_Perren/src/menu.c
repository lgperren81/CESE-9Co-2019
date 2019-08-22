/*
 * menu.c
 *
 *  Created on: 18 jun. 2019
 *  Author: Luciano Perren
 */

#include "../../../mis_programas/TP_Final/inc/menu.h"

#include "sapi.h"

// Carácter "ú"
const char u2[8] = {
   0b00010,
   0b00100,
   0b10001,
   0b10001,
   0b10001,
   0b10011,
   0b01101,
   0b00000,
};

//Carácter "ó"
const char o2[8] = {
   0b00010,
   0b00100,
   0b00000,
   0b01110,
   0b10001,
   0b10001,
   0b01110,
   0b00000,
};
//Carácter "ñ"
const char n2[8] = {
   0b01101,
   0b10010,
   0b00000,
   0b10110,
   0b11001,
   0b10001,
   0b10001,
   0b00000,
};

// FUNCION QUE CREA LOS DIFERENTES MENUES.

void myMenuCreator( uint8_t show_Menu ){

	lcdCreateChar( 0, u2);	// Genera carácter "ú"
	lcdCreateChar( 1, o2);	// Genera carácter "ó"
	lcdCreateChar( 2, n2);	// Genera carácter "ñ"

	switch( show_Menu ){

	case Menu_Principal:
		lcdGoToXY( 4, 1 );
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " Principal" );
		lcdGoToXY( 2, 2);
		lcdSendStringRaw( "Compactaci" );
		lcdData( 1 );
		lcdSendStringRaw( "n" );
		lcdGoToXY( 2, 3);
		lcdSendStringRaw( "GPS" );
		lcdGoToXY( 2, 4);
		lcdSendStringRaw( "3G/GPRS" );
		break;

	case Menu_Compactacion:
		lcdClear();
		lcdGoToXY( 3, 1);
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " Compactaci");
		lcdData( 1 );
		lcdSendStringRaw( "n");
		lcdGoToXY( 2, 2);
		lcdSendStringRaw( "Activar" );
		lcdGoToXY( 2, 3);
		lcdSendStringRaw( "Medir nivel" );
		lcdGoToXY( 2, 4 );
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " anterior");
		break;

	case Menu_GPS:
		lcdClear();
		lcdGoToXY( 7, 1 );
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " GPS");
		lcdGoToXY( 2, 2);
		lcdSendStringRaw( "Localizaci" );
		lcdData( 1 );
		lcdSendStringRaw( "n");
		lcdGoToXY( 2, 3);
		lcdSendStringRaw( "Nivel de se" );
		lcdData( 2 );
		lcdSendStringRaw( "al");
		lcdGoToXY( 2, 4 );
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " anterior" );
		break;

	case Menu_3G:
		lcdClear();
		lcdGoToXY( 5, 1);
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " 3G/GPRS");
		lcdGoToXY( 2, 2);
		lcdSendStringRaw( "Enviar mensaje" );
		lcdGoToXY( 2, 3);
		lcdSendStringRaw( "Nivel de se" );
		lcdData( 2 );
		lcdSendStringRaw( "al" );
		lcdGoToXY( 2, 4 );
		lcdSendStringRaw( "Men" );
		lcdData( 0 );
		lcdSendStringRaw( " anterior");
		break;

	default:
		break;
    }
	return;
}

// FUNCIÓN QUE UBICA EL SEÑALADOR "->".

void myCursor(int8_t pos){

	lcdGoToXY( 1, pos);			// Ubica el cursor en la primer columna y X fila
	lcdData( 0x7E );			// Muestra "->"
	lcdCommand( 0xC	);			// Display ON, cursor OFF, cursor blink OFF

	if( pos == 2 ){				// Ubico en la posición 2 y limpio la 3 y 4
		lcdGoToXY( 1, 3 );
		lcdData( 0x20 );
		lcdGoToXY( 1, 4 );
		lcdData( 0x20 );
	}
	if( pos == 3 ){				// Ubico en la posición 3 y limpio la 2 y 4
		lcdGoToXY( 1, 2 );
		lcdData( 0x20 );
		lcdGoToXY( 1, 4 );
		lcdData( 0x20);
	}
	if( pos == 4 ){				// Ubico en la posición 4 y limpio la 3 y 2
		lcdGoToXY( 1, 3 );
		lcdData( 0x20 );
		lcdGoToXY( 1, 2);
		lcdData( 0x20);
	}
}
