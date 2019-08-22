/*
 * lcd.c
 *
 *  Created on: 24 jun. 2019
 *      Author: franco
 */


#include "FMSdebounce.h"
#include "sapi.h"


typedef enum{

	BUTTONdown,
	BUTTONup,
	BUTTONfalling,
	BUTTONrising
} fsmButtonState_t;

fsmButtonState_t fsmButtonState;
//buttonFsmState_t fsmButtonState;<-------

int main(void){

	fsmButtonInit();
	while(TRUE){
		fsmButtonUpdate(TEC1);
		delay(1);
	}
	return 0;
}



void fsmButtonInit( void )
{
	fsmButtonState = BUTTONup;  // Set initial state
}

// FSM Update Sate Function
void fsmButtonUpdate( gpioMap_t tecla )
{
	static bool_t flagFalling = FALSE;
	static bool_t flagRising = FALSE;

	static uint8_t contFalling = 0;
	static uint8_t contRising = 0;

	switch( fsmButtonState ){

	case BUTTONup:
		/* CHECK TRANSITION CONDITIONS */
		gpioWrite(LEDR, ON);
		if( !gpioRead(tecla) ){
			fsmButtonState = BUTTONfalling;
		}
		break;

	case BUTTONdown:
		/* CHECK TRANSITION CONDITIONS */
		if( gpioRead(tecla) ){
			fsmButtonState = BUTTONrising;
		}
		break;

	case BUTTONfalling:
		/* ENTRY */
		if( flagFalling == FALSE ){
			flagFalling = TRUE;
			gpioWrite(LED1, ON);
		}
		/* CHECK TRANSITION CONDITIONS */
		if( contFalling >= 40 ){
			if( !gpioRead(tecla) ){
				fsmButtonState = BUTTONdown;
				buttonPressed();
			} else{
				fsmButtonState = BUTTONup;
			}
			contFalling = 0;
		}
		contFalling++;
		/* LEAVE */
		if( fsmButtonState != BUTTONfalling ){
			flagFalling = FALSE;
			gpioWrite(LED1, OFF);
		}
		break;

	case BUTTONrising:
		/* ENTRY */
		if( flagRising == FALSE ){
			flagRising = TRUE;
			gpioWrite(LED2, ON);
		}
		/* CHECK TRANSITION CONDITIONS */

		if( contRising >= 40 ){
			if( gpioRead(tecla) ){
				fsmButtonState = BUTTONup;
				buttonReleased();
			} else{
				fsmButtonState = BUTTONdown;
			}
			contRising = 0;
		}
		contRising++;

		/* LEAVE */
		if( fsmButtonState != BUTTONrising ){
			flagRising = FALSE;
			gpioWrite(LED2, OFF);
		}
		break;

	}
}
