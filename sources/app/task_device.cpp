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
#include "task_device.h"

#include "utils.h"

#define TAG	"TaskControl"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskDeviceMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Function implementation ---------------------------------------------------*/
void* TaskDeviceEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_DEVICE_ID Entry\n");

	while (1) {
		msg = ak_msg_rev(AK_TASK_DEVICE_ID);

		switch (msg->header->sig) {
		case AK_DEVICE_SETTING_ACCESS_CONTROL_REQ: {
			APP_DBG_SIG("AK_DEVICE_SETTING_ACCESS_CONTROL_REQ\n");

		}
		break;

		case AK_SETTING_SETTING_CAMERA_CAPTURE_REQ: {
			APP_DBG_SIG("AK_SETTING_SETTING_CAMERA_CAPTURE_REQ\n");

		}
		break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}
