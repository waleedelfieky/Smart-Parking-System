#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../LIB/Timer0.h"
#include <avr/io.h>

static void (*G_TIM0_CTC_Callback) (void );
/*============================================================================*/


void TIMERZERO_vInit(void) {
	/*
	 * giving ZERO CLOCk
	 */
	CLR_BIT(TCCR0, CS00);
	CLR_BIT(TCCR0, CS01);
	CLR_BIT(TCCR0, CS02);

	/*
	 * choose wave generation mode
	 */
	CLR_BIT(TCCR0, WGM00);
	SET_BIT(TCCR0, WGM01);
	/*
	 *INTERRUPTs ENABLE CTC
	 */
	SET_BIT(TIMSK,OCIE0);
	/*
	 * enable global interrupt
	 */
}

void TIMER_ZERO_SetInterval_CTC_Asynch(u8 A_u8CompareValue,void (*f_ptr)(void)) {
	/*
	 * set call back
	 */
	G_TIM0_CTC_Callback = f_ptr;
	/*
	 * set output compare value
	 */
	OCR0 = A_u8CompareValue;
	/*
	 * set presecaller to start the system
	 */
	TCCR0= (TCCR0 & 0b11111000) | PRESCALLER_1024;

}




/* Timer 0 CTC ISR */
void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{

	G_TIM0_CTC_Callback();
}

