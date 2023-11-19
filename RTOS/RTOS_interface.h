/**********************************************************************/ 
/* AUTHOR      : Waleed Sharkawy                                      */
/* DATE        : 7 Sep 2022                                           */
/* Description : Interface file for OS module                         */
/* Version     : V01                                                  */
/**********************************************************************/
#ifndef RTOS_INTERFACE_H
#define RTOS_INTERFACE_H


u8 RTOS_u8CreateTask (  void(*Copy_Handler)(void) , u8 Copy_u8Periodicity, u8 Copy_u8Priority , u8 Copy_FirstD  );
void RTOS_voidStartOS(void);
u8 RTOS_u8DeleteTask(u8 Copy_u8Priority);
u8 RTOS_u8SuspendTask(u8 Copy_u8Priority);
u8 RTOS_u8ResumeTask(u8 Copy_u8Priority);


#endif
