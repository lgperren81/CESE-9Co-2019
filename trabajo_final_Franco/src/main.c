/*=============================================================================
 * Copyright (c) 2019, Franco Blatter <fblatter19@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/06/24
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/
#include "ADread.h"
#include "main.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
	boardInit();
	fmsInit();

	while(1) {
		fmsValue();
		delay (1000);
	}

	return(0);
}

