/*
 * GIE_prg.c
 *
 *  Created on: Jun 2, 2022
 *      Author: Waleed
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GIE_int.h"

void GIE_vEnable(void) {
	SET_BIT(SREG, 7);
}

void GIE_vDisable(void) {
	CLR_BIT(SREG, 7);
}

