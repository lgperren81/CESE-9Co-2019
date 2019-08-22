/*
 * menu.h
 *
 *  Created on: 18 jun. 2019
 *      Author: lgperren
 */

#ifndef MIS_PROGRAMAS_TP_FINAL_INC_MENU_H_
#define MIS_PROGRAMAS_TP_FINAL_INC_MENU_H_

#include "sapi.h"

/*==================[macros]=================================================*/

#define Menu_Principal 1
#define Menu_Compactacion 2
#define Menu_GPS 3
#define Menu_3G 4

/*==================[external functions declaration]=========================*/

void myMenuCreator(uint8_t show_Menu);
void myCursor(int8_t pos);

#endif /* MIS_PROGRAMAS_TP_FINAL_INC_MENU_H_ */
