#ifndef BIT_MATH_H
	#define BIT_MATH_H
	
#define SET_BIT(REG, BIT_NO) (REG |= (1 << (BIT_NO)))
#define CLR_BIT(REG, BIT_NO) (REG &= ~(1 << (BIT_NO)))
#define TOG_BIT(REG, BIT_NO) (REG ^= (1 << (BIT_NO)))
#define GET_BIT(REG, BIT_NO) (((REG) >> (BIT_NO)) & (0x01))
#define WRT_BIT(REG, BIT_NO, VAL) (REG = (REG & ~(1 << BIT_NO)) | (VAL << BIT_NO))
#endif
