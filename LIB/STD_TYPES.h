#ifndef STD_TYPES_H
	#define STD_TYPES_H
	
typedef unsigned char 				 u8;
typedef unsigned short int 			u16;
typedef unsigned int 			    u32;
typedef unsigned long long int 	 	u64;

typedef signed char 				 s8;
typedef signed short int 	   		s16;
typedef signed int 			   		s32;
typedef signed long long int   		s64;

typedef float 				   		f32;
typedef double 						f64;
typedef long double 		   	   f128;

#define STD_OK 1
#define STD_NOK 0

#define NULL ((void*)0)

#endif