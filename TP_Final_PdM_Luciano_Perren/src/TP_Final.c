/*=============================================================================
 * Author: Luciano Perren <lgperren@gmail.com>
 * Date: 24/06/2019
 * Version: 1.0
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

#include "../../../mis_programas/TP_Final/inc/menu.h"
#include "../../../mis_programas/TP_Final/inc/encoder.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   uint8_t pos_cursor;
   bool_t enter;
   uint8_t menu = 1;

   lcdInit( 20, 4, 5, 8 );		//20 caracteres x 4 lineas - caracter de 5 x 8 pixeles
   lcdClear();
   fsmEncoderInit();
   fsmButtonInit();
   myMenuCreator( Menu_Principal );

// ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	   pos_cursor = fsmEncoderUpdate( GPIO7, GPIO5 );
	   myCursor(pos_cursor);
	   enter = fsmButtonUpdate ( GPIO3 );
	   gpioWrite( LED3, enter );

	   if( ( enter == 1 ) && ( pos_cursor == 2 ) && ( menu == Menu_Principal ) ){
		   menu = Menu_Compactacion;
		   myMenuCreator( Menu_Compactacion );
	   }

	   if( ( enter == 1 ) && ( pos_cursor == 3 ) && ( menu == Menu_Principal ) ){
		   menu = Menu_GPS;
		   myMenuCreator( Menu_GPS );
	   }

	   if( ( enter == 1) && ( pos_cursor == 4 ) && (menu == Menu_Principal ) ){
		   menu = Menu_3G;
		   myMenuCreator( Menu_3G );
		}
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[fin del archivo]========================================*/
