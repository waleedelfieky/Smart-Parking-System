#include "../LIB/BIT_MATH.h"
#include "../LIB/STD_TYPES.h"
#include "EXT_int.h"

#include <avr/io.h>
void MEXTI_vControlSense(u8 A_u8TypeEXTI, u8 A_u8SenseControl) {
	switch(A_u8TypeEXTI) {
		case MEXTI0:
			switch(A_u8SenseControl) {
				case MEXTI_LOW:
					CLR_BIT(MCUCR, ISC00);
					CLR_BIT(MCUCR, ISC01);
					break;

				case MEXTI_ON_CHANGE:
					SET_BIT(MCUCR, ISC00);
					CLR_BIT(MCUCR, ISC01);
					break;

				case MEXTI_FALLING:
					CLR_BIT(MCUCR, ISC00);
					SET_BIT(MCUCR, ISC01);
					break;

				case MEXTI_RAISING:
					SET_BIT(MCUCR, ISC00);
					SET_BIT(MCUCR, ISC01);
					break;
			}
			break;

		case MEXTI1:
			switch(A_u8SenseControl) {
				case MEXTI_LOW:
					CLR_BIT(MCUCR, ISC10);
					CLR_BIT(MCUCR, ISC11);
					break;

				case MEXTI_ON_CHANGE:
					SET_BIT(MCUCR, ISC10);
					CLR_BIT(MCUCR, ISC11);
					break;

				case MEXTI_FALLING:
					CLR_BIT(MCUCR, ISC10);
					SET_BIT(MCUCR, ISC11);
					break;

				case MEXTI_RAISING:
					SET_BIT(MCUCR, ISC10);
					SET_BIT(MCUCR, ISC11);
					break;
			}
			break;

		case MEXTI2:
			switch(A_u8SenseControl) {
				case MEXTI_FALLING:
					CLR_BIT(MCUCSR, ISC2);
					break;

				case MEXTI_RAISING:
					SET_BIT(MCUCSR, ISC2);
					break;
			}
			break;
	}
}

void MEXTI_vEnable(u8 A_u8TypeEXTI) {
	switch(A_u8TypeEXTI) {
		case MEXTI0: SET_BIT(GICR, INT0); break;
		case MEXTI1: SET_BIT(GICR, INT1); break;
		case MEXTI2: SET_BIT(GICR, INT2); break;
	}
	SET_BIT(SREG, 7);
}

void MEXTI_vDisable(u8 A_u8TypeEXTI) {
	switch(A_u8TypeEXTI) {
		case MEXTI0: CLR_BIT(GICR, INT0); break;
		case MEXTI1: CLR_BIT(GICR, INT1); break;
		case MEXTI2: CLR_BIT(GICR, INT2); break;
	}
	CLR_BIT(SREG, 7);
}

void MEXTI_vClearFlag(u8 A_u8TypeEXTI) {
	switch(A_u8TypeEXTI) {
	case MEXTI0: SET_BIT(GIFR, 6); break;
	case MEXTI1: SET_BIT(GIFR, 7); break;
	case MEXTI2: SET_BIT(GIFR, 5); break;
	}
}

u8 MEXTI_vReadFlag(u8 A_u8TypeEXTI) {
	u8 retValue;
	switch(A_u8TypeEXTI) {
	case MEXTI0: retValue = GET_BIT(GIFR, 6); break;
	case MEXTI1: retValue = GET_BIT(GIFR, 7); break;
	case MEXTI2: retValue = GET_BIT(GIFR, 5); break;
	}

	return retValue;
}
