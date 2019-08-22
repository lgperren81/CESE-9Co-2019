/*=============================================================================
 * Author: Luciano Perren <lgperren@gmail.com>
 * Date: 24/06/2019
 * Version: 1.0
 *===========================================================================*/
/*
 * encoder.h
 *
 */

#ifndef MIS_PROGRAMAS_TP_FINAL_INC_ENCODER_H_
#define MIS_PROGRAMAS_TP_FINAL_INC_ENCODER_H_

#include "sapi.h"

/*==================[external functions declaration]=========================*/

uint8_t fsmEncoderUpdate( gpioMap_t pin_A, gpioMap_t pin_B );
void fsmEncoderInit( void );
void fsmButtonInit( void );
bool_t fsmButtonUpdate( gpioMap_t tecla);

#endif /* MIS_PROGRAMAS_TP_FINAL_INC_ENCODER_H_ */
