#ifndef __TASK_SENSOR_H
#define __TASK_SENSOR_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Enumarics -----------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskSensorMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskSensorEntry(void*);

#endif /* __TASK_SYSTEM_H */
