/**********************************************************************/ 
/* AUTHOR      : Waleed Sharkawy                                      */
/* DATE        : 7 Sep 2022                                           */
/* Description : Private file for OS module                           */
/* Version     : V01                                                  */
/**********************************************************************/
#ifndef _RTOS_PRIVATE_H
#define _RTOS_PRIVATE_H

typedef enum
{
	Suspended,
	Running
}RunState;


typedef struct
{
	void (*TaskHandler)(void)    ;
	u8        Periodicity        ;
	u8        FirstDelay         ;
	RunState  TaskRunState       ;
}Task;

static void Scheduler(void);

#endif
