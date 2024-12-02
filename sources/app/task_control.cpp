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
#include "task_control.h"

#include "utils.h"

#define TAG	"TaskControl"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskControlMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Function implementation ---------------------------------------------------*/
void* TaskControlEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_CONTROL_ID Entry\n");

	while (1) {
		msg = ak_msg_rev(AK_TASK_CONTROL_ID);

		switch (msg->header->sig) {
		case AK_CONTROL_LED_REQ: {
			APP_DBG_SIG("AK_CONTROL_LED_REQ\n");

			uint8_t *state = (uint8_t*)msg->header->payload;

			APP_DBG(TAG, "CONTROL LED STATE %d", *state);

			json JSON;
			JSON["status"] = *state ? true : false;
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
