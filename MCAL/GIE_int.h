/*
 * GIE_int.h
 *
 *  Created on: Jun 2, 2022
 *      Author: Waleed
 */

#ifndef MCAL_GIE_GIE_INT_H_
#define MCAL_GIE_GIE_INT_H_

#define SREG (*((volatile u8*)(0x5F)))

void GIE_vEnable(void);
void GIE_vDisable(void);

#endif /* MCAL_GIE_GIE_INT_H_ */
