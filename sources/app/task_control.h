#ifndef __TASK_CONTROL_H
#define __TASK_CONTROL_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskControlMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskControlEntry(void*);

#endif /* __TASK_CONTROL_H */
