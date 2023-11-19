/*
 * Timer0.h
 *
 *  Created on: Jan 1, 2023
 *      Author: Waleed
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#define PRESCALLER_1024		5

void TIMERZERO_vInit(void);
void TIMER_ZERO_SetInterval_CTC_Asynch(u8 A_u8CompareValue,void (*f_ptr)(void));

#endif /* TIMER0_H_ */
