/*=============================================================================
 * Author: Luciano Perren <lgperren@gmail.com>
 * Date: 2019/06/20
 * Version: 1.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "Test_Encoder.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

bool_t valor_1;
bool_t in_A;
bool_t in_B;

int main( void )
{
   // ----- Setup -----------------------------------
   //boardInit();
   boardConfig();
   gpioConfig( GPIO7, GPIO_INPUT );
   gpioConfig( GPIO8, GPIO_INPUT);
   gpioConfig( GPIO2, GPIO_INPUT);

   // ----- Repeat for ever -------------------------
   while( true ) {
	   valor_1 = gpioRead( GPIO2 );
	   gpioWrite( LED3, valor_1 );

	   in_A = gpioRead( GPIO7 );
	   gpioWrite( LED2, in_A );

	   in_B = gpioRead( GPIO8 );
	   gpioWrite( LED1, in_B );

	   if (( in_A == 0 ) && ( in_B == 1 )){
		   gpioWrite( LEDB, ON );
		   gpioWrite( LEDR, OFF);
	   }
	   if (( in_A == 1 ) && ( in_B == 0 )){
		   gpioWrite( LEDR, ON );
		   gpioWrite( LEDB, OFF);
	   }

	   //if (( in_A == 0 ) && ( in_B == 0 )){
		 //  gpioWrite (LEDR, OFF );
		  // gpioWrite (LEDB, OFF );
	   //}
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

/*void fsmEncoderUpdate( bool_t in_A, bool_t in_B )
{
	switch( fsmEncoderState ){

      case NONE:
         if( gpioRead(input_1 == 0) && gpioRead(input_2 == 1) ){
        	 fsmEncoderState = LEFT;
            }
         if( gpioRead(input_1 == 1) && gpioRead(input_2 == 0) ){
        	 fsmEncoderState = RIGHT;
        	 }
      break;

      case LEFT:
         /* ENTRY
    	 gpioWrite(LED1, ON);
    	 gpioWrite(LED2, OFF);

    	 if(gpioRead (input_1 == 0) && gpioRead(input_2 == 0) ){
        	fsmEncoderState = BOTH;
        	 }
         if( gpioRead (input_1 == 1) && gpioRead(input_2 == 1) ){
        	fsmEncoderState = NONE;
        		 }
      break;

      case RIGHT:
         /* ENTRY
    	 gpioWrite(LED1, OFF);
    	 gpioWrite(LED2, ON);
    	 if(gpioRead(input_1 == 0) && gpioRead(input_2 == 0) ){
    			 fsmEncoderState = BOTH;
    			 }
    	 if( gpioRead(input_1 == 1) && gpioRead(input_2 == 1) ){
    	    	 fsmEncoderState = NONE;
    	    	 }
      break;

      case BOTH:
       	 gpioWrite(LED1, ON);
         gpioWrite(LED2, ON);

         /* CHECK TRANSITION CONDITIONS
         if( gpioRead(input_1 == 0) && gpioRead(input_2 == 1) ){
        	 fsmEncoderState = LEFT;
             }
         if( gpioRead(input_1 == 1) && gpioRead(input_2 == 0) ){
        	 fsmEncoderState = RIGHT;
             }
      break;

      default:
         fsmEncoderError();
      break;
   }
}
*/
