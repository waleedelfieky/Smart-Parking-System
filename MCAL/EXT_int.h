/*
 * EXT_int.h
 *
 *  Created on: Jun 2, 2022
 *      Author: Waleed
 */

#ifndef MCAL_EXT_EXT_INT_H_
#define MCAL_EXT_EXT_INT_H_

#define MEXTI0				0
#define MEXTI1				1
#define MEXTI2				2
#define MEXTI_ALL_ENABLE 	3

#define MEXTI_LOW			0
#define MEXTI_ON_CHANGE		1
#define MEXTI_FALLING		2
#define MEXTI_RAISING		3

void MEXTI_vInit(u8 A_u8TypeEXTI);
void MEXTI_vControlSense(u8 A_u8TypeEXTI, u8 A_u8SenseControl);
void MEXTI_vEnable(u8 A_u8TypeEXTI);
void MEXTI_vDisable(u8 A_u8TypeEXTI);
u8 MEXTI_vReadFlag(u8 A_u8TypeEXTI);
void MEXTI_vClearFlag(u8 A_u8TypeEXTI);

#endif /* MCAL_EXT_EXT_INT_H_ */
