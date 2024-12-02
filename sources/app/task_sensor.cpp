#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ak.h"

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "task_list.h"
#include "task_sensor.h"

#include "utils.h"

#define TAG	"TaskSystem"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskSensorMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int randomValueSensor(int min, int max) {
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	return min + std::rand() % (max - min + 1);
}

/* Function implementation ---------------------------------------------------*/
void* TaskSensorEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_SENSOR_ID Entry\n");

	while (1) {
		msg = ak_msg_rev(AK_TASK_SENSOR_ID);

		switch (msg->header->sig) {
        case AK_SENSOR_UPDATE: {
			APP_DBG_SIG("AK_SENSOR_UPDATE\n");

           	json JSON;
			JSON["temperature"] = randomValueSensor(20, 97);
			JSON["humidity"] = randomValueSensor(35, 99);
			TaskPostDynamicMsg(AK_TASK_CLOUD_ID, AK_CLOUD_SEND_TELEMETRY, 
						(uint8_t*)JSON.dump().c_str(), JSON.dump().length());
        }
        break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}
