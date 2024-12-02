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
        case AK_SENSOR_STATUS_UPDATE: {
			APP_DBG_SIG("AK_SENSOR_STATUS_UPDATE\n");

			/*
				{
					"type": "SENSOR_STATUS",
					"values": {
						"Doors": ["DISCONNECT", "DISCONNECT"],
						"Flood": "DISCONNECT",
						"Heat": "DISCONNECT",
						"Smoke": "DISCONNECT",
						"Inputs": [1, 1, 1, 1, 1]
					},
					"timestamp": 1722742646
				}
			*/

           	json JSON;
			JSON["type"] = "SENSOR_STATUS";
			JSON["values"]["Doors"][0] = "DISCONNECT";
			JSON["values"]["Doors"][1] = "DISCONNECT";
			JSON["values"]["Flood"] = "DISCONNECT";
			JSON["values"]["Heat"] = "DISCONNECT";
			JSON["values"]["Smoke"] = "DISCONNECT";
			for (uint8_t id = 0; id < 4; ++id) {
				JSON["values"]["Inputs"][id] = 1;
			}			

			ourPubSubClient.publishTelemetryTimeseries(JSON.dump());
        }
        break;

		case AK_SENSOR_VALUE_UPDATE: {
			APP_DBG_SIG("AK_SENSOR_VALUE_UPDATE\n");

			/*
				{
					"type": "SENSOR_VALUE",
					"values": {
						"ACCurrent_mA": [0, 0],
						"SmokeCurrent_mA": 0,
						"TemperatureCelsius": [0.0, 0.0]
					},
					"timestamp": 1722742646
				}
			*/
			json JSON;
			JSON["type"] = "SENSOR_VALUE";
			JSON["values"]["ACCurrent_mA"][0] = 0;
			JSON["values"]["ACCurrent_mA"][1] = 0;
			JSON["values"]["SmokeCurrent_mA"] = 0;
			JSON["values"]["TemperatureCelsius"][0] = 0.0;
			JSON["values"]["TemperatureCelsius"][1] = 0.0;

			ourPubSubClient.publishTelemetryTimeseries(JSON.dump());
		}
		break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}
