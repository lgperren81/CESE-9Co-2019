/*=============================================================================
 * Author: Luciano Perren <lgperren@gmail.com>
 * Date: 2019/06/06
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "FSM_Debounce.h"
#include "sapi.h"

typedef enum{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_FALLING,
	BUTTON_RISING
} fsmButtonState_t;

void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
void buttonPressed( void );
void buttonReleased( void );

fsmButtonState_t fsmButtonState;

int main(void){
	boardConfig();
	fsmButtonInit();
	while(TRUE){
		fsmButtonUpdate(TEC1);
		delay(1);
	}
	return 0;
}

void buttonPressed( void )
{
	gpioWrite(LEDR, OFF);
	gpioWrite(LEDG, OFF);
	gpioWrite(LEDB, ON);
}

void buttonReleased( void )
{
	gpioWrite(LEDR, ON);
	gpioWrite(LEDG, OFF);
	gpioWrite(LEDB, OFF);
}

void fsmButtonError( void )
{
	fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( void )
{
	fsmButtonState = BUTTON_UP;  // Set initial state
}

// FSM Update Sate Function
void fsmButtonUpdate( gpioMap_t tecla )
{
	static bool_t flagFalling = FALSE;
	static bool_t flagRising = FALSE;

	static uint8_t contFalling = 0;
	static uint8_t contRising = 0;

	switch( fsmButtonState ){

	case BUTTON_UP:
		/* CHECK TRANSITION CONDITIONS */
		gpioWrite(LEDR, ON);
		if( !gpioRead(tecla) ){
			fsmButtonState = BUTTON_FALLING;
		}
		break;

	case BUTTON_DOWN:
		/* CHECK TRANSITION CONDITIONS */
		if( gpioRead(tecla) ){
			fsmButtonState = BUTTON_RISING;
		}
		break;

	case BUTTON_FALLING:
		/* ENTRY */
		if( flagFalling == FALSE ){
			flagFalling = TRUE;
			gpioWrite(LED1, ON);
		}
		/* CHECK TRANSITION CONDITIONS */
		if( contFalling >= 40 ){
			if( !gpioRead(tecla) ){
				fsmButtonState = BUTTON_DOWN;
				buttonPressed();
			} else{
				fsmButtonState = BUTTON_UP;
			}
			contFalling = 0;
		}
		contFalling++;
		/* LEAVE */
		if( fsmButtonState != BUTTON_FALLING ){
			flagFalling = FALSE;
			gpioWrite(LED1, OFF);
		}
		break;

	case BUTTON_RISING:
		/* ENTRY */
		if( flagRising == FALSE ){
			flagRising = TRUE;
			gpioWrite(LED2, ON);
		}
		/* CHECK TRANSITION CONDITIONS */

		if( contRising >= 40 ){
			if( gpioRead(tecla) ){
				fsmButtonState = BUTTON_UP;
				buttonReleased();
			} else{
				fsmButtonState = BUTTON_DOWN;
			}
			contRising = 0;
		}
		contRising++;

		/* LEAVE */
		if( fsmButtonState != BUTTON_RISING ){
			flagRising = FALSE;
			gpioWrite(LED2, OFF);
		}
		break;

	default:
		fsmButtonError();
		break;
	}
}

