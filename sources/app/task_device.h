#ifndef __TASK_DEVICE_H
#define __TASK_DEVICE_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskDeviceMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskDeviceEntry(void*);

#endif /* __TASK_CONTROL_H */
