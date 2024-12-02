#ifndef __TASK_SYSTEM_H
#define __TASK_SYSTEM_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Enumarics -----------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskSystemMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskSystemEntry(void*);

#endif /* __TASK_SYSTEM_H */
