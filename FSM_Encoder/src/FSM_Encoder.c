/*=============================================================================
 * Author: Luciano Perren <lgperren@gmail.com>
 * Date: 2019/06/03
 *===========================================================================*/

#include "sapi.h"        // <= Biblioteca sAPI

typedef enum {
   NONE,
   LEFT,
   RIGHT,
   BOTH
} fsmEncoderState_t;

void fsmEncoderError( void );
void fsmEncoderInit( void );
void fsmEncoderUpdate( bool_t in_A, bool_t in_B );

fsmEncoderState_t fsmEncoderState;

bool_t in_A;
bool_t in_B;
bool_t enter;
bool_t right_flag;
bool_t left_flag;
bool_t inp_A;
bool_t inp_B;

int main(void){
   boardConfig();
   gpioConfig( GPIO7, GPIO_INPUT );
   gpioConfig( GPIO8, GPIO_INPUT);
   gpioConfig( GPIO2, GPIO_INPUT);
   fsmEncoderInit();

   while(TRUE){

	  fsmEncoderUpdate( GPIO7, GPIO5 );

      enter = gpioRead( GPIO3 );
      gpioWrite( LED3, enter );

      inp_A = gpioRead( GPIO7 );
      gpioWrite( LED2, in_A );

      inp_B = gpioRead( GPIO5 );
      gpioWrite( LED1, in_B );

   }
   return 0;
}

void fsmEncoderError( void )
{
   fsmEncoderState = NONE;	// En caso de caer en un estado no definido
}

void fsmEncoderInit( void )
{
   fsmEncoderState = NONE;  // Determina el estado inicial
}

// FSM Update Sate Function
void fsmEncoderUpdate( gpioMap_t input_A, gpioMap_t input_B )
{
	in_A = gpioRead( input_A );
	in_B = gpioRead( input_B );

	switch( fsmEncoderState ){

      case NONE:
    	  left_flag = 0;
    	  right_flag = 0;
    	  gpioWrite ( LEDR, OFF );
    	  gpioWrite ( LEDB, OFF );

    	  if( ( in_A == 0 ) && ( in_B == 1 ) ){
        	 fsmEncoderState = LEFT;
             }
          if( ( in_A == 1 ) && ( in_B == 0 ) ){
        	 fsmEncoderState = RIGHT;
        	 }
      break;

      case LEFT:
    	    gpioWrite ( LEDB, OFF );
    	    gpioWrite ( LEDR, OFF );
    	  //left_flag = 0;

    	  if( ( in_A == 0 ) && ( in_B == 0 ) ){
    		 left_flag = 1;
    		 fsmEncoderState = BOTH;
        	 }
          if( ( in_A == 1 ) && ( in_B == 1 ) ){
        	 fsmEncoderState = NONE;
        		 }
      break;

      case RIGHT:
    	    gpioWrite ( LEDR, OFF );
    	    gpioWrite ( LEDB, OFF );
    	  //right_flag = 0;

    	 if( ( in_A == 0 ) && ( in_B == 0 ) ){
    		 right_flag = 1;
    		 fsmEncoderState = BOTH;
    		 }
    	 if( ( in_A == 1 ) && ( in_B == 1 ) ){
    	     fsmEncoderState = NONE;
    	    	 }
      break;

      case BOTH:

         if( right_flag == 1){
        	 gpioWrite ( LEDR, ON );
        	 gpioWrite ( LEDB, OFF );
         }
         if( left_flag == 1){
             gpioWrite ( LEDR, OFF );
             gpioWrite ( LEDB, ON );
         }
         if( ( in_A == 0 ) && ( in_B == 1 ) ){
        	 fsmEncoderState = LEFT;
         }
         if( ( in_A == 1 ) && ( in_B == 0 ) ){
        	 fsmEncoderState = RIGHT;
         }
      break;

      default:
         fsmEncoderError();
      break;
   }
}

