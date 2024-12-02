#ifndef __TASK_SETTING_H
#define __TASK_SETTING_H

#include "message.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern q_msg_t taskSettingMailbox;

/* Function prototypes -------------------------------------------------------*/
extern void* TaskSettingEntry(void*);

#endif /* __TASK_SETTING_H */
