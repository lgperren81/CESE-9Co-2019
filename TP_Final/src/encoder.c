/*
 * encoder.c
 *
 *  Created on: 21 jun. 2019
 *      Author: lgperren
 */

/*==================[inlcusiones]============================================*/

#include "../../../mis_programas/TP_Final/inc/encoder.h"
#include "sapi.h"

/*==================[definiciones de datos internos]=========================*/

    typedef enum { NONE, LEFT, RIGHT, BOTH } fsmEncoderState_t;
    static fsmEncoderState_t fsmEncoderState;

    typedef enum { BUTTON_UP, BUTTON_DOWN, BUTTON_FALLING, BUTTON_RISING } fsmButtonState_t;
    static fsmButtonState_t fsmButtonState;

    static bool_t in_A;
	static bool_t in_B;
	static bool_t right_flag;
	static bool_t left_flag;

	static bool_t flagFalling = 0;
	static bool_t flagRising = 0;
	static uint8_t contFalling = 0;
	static uint8_t contRising = 0;

/*==================[definiciones de datos externos]=========================*/

	//int8_t pul_Encoder

/*==================[declaraciones de funciones internas]====================*/

	static void fsmEncoderError( void )
	{
	    fsmEncoderState = NONE;	// En caso de caer en un estado no definido
	}

	static void fsmButtonError( void )
	{
		fsmButtonState = BUTTON_UP;
	}

/*==================[definiciones de funciones externas]=====================*/

	void fsmEncoderInit( void )
	{
	   fsmEncoderState = NONE;  // Determina el estado inicial
	}

	// MÁQUINA DE ESTADOS QUE LEE EL ENCODER.
	// Secuencia 00 10 11 01 izquierda
	// Secuencia 00 01 11 10 derecha

	uint8_t fsmEncoderUpdate( gpioMap_t pin_A, gpioMap_t pin_B )
	{
	   static uint8_t pul_Encoder = 2;
	   in_A = gpioRead( pin_A );
	   in_B = gpioRead( pin_B );

	   switch( fsmEncoderState ){

       case NONE:

    	   if( (in_A == 0) && (in_B == 1) ){
        	  fsmEncoderState = LEFT;
              }
           if( (in_A == 1) && (in_B == 0) ){
        	  fsmEncoderState = RIGHT;
        	  }
       break;

       case LEFT:

    	   if( (in_A == 0) && (in_B == 0) ){
    		  left_flag = 1;
    		  fsmEncoderState = BOTH;
        	  }
           if( (in_A == 1) && (in_B == 1) ){
        	  fsmEncoderState = NONE;
        	  }
       break;

       case RIGHT:

    	   if( (in_A == 0) && (in_B == 0) ){
    	 	  right_flag = 1;
    		  fsmEncoderState = BOTH;
    		  }
    	   if( (in_A == 1) && (in_B == 1) ){
    	      fsmEncoderState = NONE;
    	      }
       break;

       case BOTH:

           if( ( in_A == 1 ) && ( in_B == 0) && ( left_flag == 1 ) ){
        	  left_flag = 0;
        	  pul_Encoder++;
              if( pul_Encoder > 4 ){
            	  pul_Encoder = 2;
              }
        	  fsmEncoderState = NONE;
           }
           if( ( in_A == 0 ) && ( in_B == 1 ) && ( right_flag == 1 ) ){
        	  right_flag = 0;
        	  pul_Encoder--;
              if( pul_Encoder < 2 ){
            	  pul_Encoder = 4;
              }
              fsmEncoderState = NONE;
           }
           if( ( in_A == 0 ) && ( in_B == 1 ) && ( left_flag == 1 ) ){
        	  left_flag = 0;
        	  fsmEncoderState = LEFT;
           }
           if( ( in_A == 1 ) && ( in_B == 0 ) && ( right_flag == 1 ) ){
        	  right_flag = 0;
        	  fsmEncoderState = RIGHT;
           }
       break;

       default:
          fsmEncoderError();
       break;
    }
	 return pul_Encoder;
 }

	void fsmButtonInit( void )
	{
		fsmButtonState = BUTTON_UP;  // Set initial state
	}

	// MÁQUINA DE ESTADOS ANTIREBOTE.

	bool_t fsmButtonUpdate( gpioMap_t tecla )
	{
		static bool_t pressed = 0;

		switch( fsmButtonState ){

		case BUTTON_UP:
			/* CHECK TRANSITION CONDITIONS */
			pressed = 0;
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
			if( flagFalling == 0 ){
				flagFalling = 1;
			}
			/* CHECK TRANSITION CONDITIONS */
			if( contFalling >= 40 ){
				if( !gpioRead(tecla) ){
					fsmButtonState = BUTTON_DOWN;
					} else {
					fsmButtonState = BUTTON_UP;
				}
				contFalling = 0;
			}
			contFalling++;
			/* LEAVE */
			if( fsmButtonState != BUTTON_FALLING ){
				flagFalling = 0;
			}
			break;

		case BUTTON_RISING:
			/* ENTRY */
			if( flagRising == 0 ){
				flagRising = 1;
			}
			/* CHECK TRANSITION CONDITIONS */

			if( contRising >= 40 ){
				if( gpioRead(tecla) ){
					pressed = 1;
					fsmButtonState = BUTTON_UP;
				} else {
					fsmButtonState = BUTTON_DOWN;
				}
				contRising = 0;
			}
			contRising++;

			/* LEAVE */
			if( fsmButtonState != BUTTON_RISING ){
				flagRising = FALSE;
			}
			break;

		default:
			fsmButtonError();
			break;
		}
		return pressed;
	}
