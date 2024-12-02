#ifndef _TASK_LIST_H_
#define _TASK_LIST_H_

#include "ak.h"

#include "task_system.h"
#include "task_device.h"
#include "task_sensor.h"
#include "task_cloud.h"

#define AK_APP_TYPE_IF 101

/*---------------------------------------------------------------------------*
 *  DECLARE: Internal Task ID
 *  Note: Task id MUST be increasing order.
 *---------------------------------------------------------------------------*/
enum {
    /* SYSTEM TASKS */
    AK_TASK_TIMER_ID,

    /* APP TASKS */
    AK_TASK_SYSTEM_ID,
    AK_TASK_DEVICE_ID,
    AK_TASK_SENSOR_ID,
    AK_TASK_CLOUD_ID,

    /* EOT task ID */
    AK_TASK_LIST_LEN
};

/* Typedef -------------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern ak_task_t task_list[];

/* Function prototypes -------------------------------------------------------*/

#endif /* _TASK_LIST_H_ */
