#include "timer.h"
#include "task_list.h"

ak_task_t task_list[] = {
    {	AK_TASK_TIMER_ID,			TASK_PRI_LEVEL_1,	TaskTimerEntry			,	&timerMailbox				,	"TIMER SERVICE"					},
	{	AK_TASK_SYSTEM_ID,			TASK_PRI_LEVEL_1,	TaskSystemEntry			,	&taskSystemMailbox			,	"SYSTEM"						},
	{	AK_TASK_SETTING_ID,			TASK_PRI_LEVEL_1,	TaskSettingEntry		,	&taskSettingMailbox			,	"SETTING"						},
	{	AK_TASK_CONTROL_ID,			TASK_PRI_LEVEL_1,	TaskControlEntry		,	&taskControlMailbox			,	"CONTROL"						},
	{	AK_TASK_SENSOR_ID,			TASK_PRI_LEVEL_1,	TaskSensorEntry			,	&taskSensorMailbox			,	"SENSOR"						},
	{	AK_TASK_CLOUD_ID,			TASK_PRI_LEVEL_1,	TaskCloudEntry			,	&taskCloudMailbox			,	"CLOUD"							}
};


                                
     
                                 
                                
