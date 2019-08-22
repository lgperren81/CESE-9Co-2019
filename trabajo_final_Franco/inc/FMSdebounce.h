/*
 * lcd.h
 *
 *  Created on: 24 jun. 2019
 *      Author: franco
 */

#ifndef PDM2_TRABAJO_FINAL_INC_FMSDEBOUNCE_H_
#define PDM2_TRABAJO_FINAL_INC_FMSDEBOUNCE_H_


#include "ADread.h"
#include "FMSdebounce.h"


void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
void buttonPressed( void );
void buttonReleased( void );





#endif /* PDM2_TRABAJO_FINAL_INC_FMSDEBOUNCE_H_ */
