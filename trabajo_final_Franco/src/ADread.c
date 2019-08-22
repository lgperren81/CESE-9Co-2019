/*
 * ADreadaer.c
 *
 *  Created on: 24 jun. 2019
 *      Author: franco
 */



#include "ADread.h"
#include "FMSdebounce.h"
#include "sapi.h"        // <= sAPI header


typedef enum{
    InitState,
    AcqValues,
    Average,
    ShowAverage,
} States_t;


static States_t State;

void fmsInit (void){

	// Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
	lcdInit( 16, 2, 5, 8 );
	State= InitState;

	}

void fmsValue (void) {
	  int x=0, lecturas [9], suma=0;
	  float promedio;

		switch (State){
		case InitState:
			lcdGoToXY( 0, 0 ); // Poner cursor en 0, 0
			lcdSendStringRaw( "Press Tec1" );
			fsmButtonUpdate(BUTTON0);

			if (fsmButtonState ==BUTTONdown){
			State = AcqValues;
			lcdGoToXY( 0, 0 ); // Poner cursor en 0, 0
			lcdSendStringRaw( "Imput Data" );
			}
			break;

		case AcqValues:
        while (x<=9){
        	lecturas[x]=adcRead (CH1);
        	x++;
        	delay (TIEMPO_ESPERA);
                }
        	if (x==10){
            	State= Average;
                    }
        	break;

		case Average:
			x=0;
			for (x=0;x<=9;x++){
				suma=suma+ lecturas[x];
			}
			promedio =suma/10;
			char mystring[20];
			sprintf (mystring,"%f",promedio);

		    State=ShowAverage;
			break;

		case ShowAverage:
			lcdGoToXY( 0, 0 ); // Poner cursor en 0, 0
			lcdSendStringRaw( "Average Data" );
			lcdData(0);
			lcdGoToXY( 0, 1 );


			lcdSendStringRaw( mystring);
			delay (TIEMPO_ESPERA);
			State=InitState;
			break;
		}

}



