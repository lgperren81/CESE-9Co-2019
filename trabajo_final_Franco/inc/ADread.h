/*
 * ADread.h
 *
 *  Created on: 24 jun. 2019
 *      Author: franco
 */

#ifndef PDM2_TRABAJO_FINAL_INC_ADREAD_H_
#define PDM2_TRABAJO_FINAL_INC_ADREAD_H_

#include "sapi.h"
#include "FMSdebounce.h"

#define BUTTON0 TEC1
#define TIEMPO_ESPERA 5000


void fmsInit (void);
void fmsValue (void);

#endif /* PDM2_TRABAJO_FINAL_INC_ADREAD_H_ */
