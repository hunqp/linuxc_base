#ifndef __TASK_CLOUD_H
#define __TASK_CLOUD_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskCloudMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskCloudEntry(void*);

#endif /* __TASK_CLOUD_H */
