#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "app_config.h"

#include "task_list.h"
#include "task_setting.h"

#include "utils.h"

#define TAG	"TaskSetting"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskSettingMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Function implementation ---------------------------------------------------*/
void* TaskSettingEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_SETTING_ID Entry\n");

	while (1) {
		msg = ak_msg_rev(AK_TASK_SETTING_ID);

		switch (msg->header->sig) {
		case AK_SETTING_SET_ACCESS_CONTROL_REQ: {
			APP_DBG_SIG("AK_SETTING_SET_ACCESS_CONTROL_REQ\n");

			bool bSuccess = false;
			std::string payload = std::string((char*)msg->header->payload, msg->header->len);
			try {
				json JSON = json::parse(payload);

				ourACSetting.password = JSON["params"]["Password"].get<std::string>();
				ourACSetting.beginTime = JSON["params"]["BeginTime"].get<uint32_t>();
				ourACSetting.endTime = JSON["params"]["EndTime"].get<uint32_t>();

				APP_DBG(TAG, "%s", JSON["method"].dump().c_str());
				APP_DBG(TAG, "Password  : %s", ourACSetting.password.c_str());
				APP_DBG(TAG, "Begin time: %d", ourACSetting.beginTime);
				APP_DBG(TAG, "End time  : %d", ourACSetting.endTime);
			}
			catch (const std::exception& e) {
				APP_PRINT("%s\r\n", e.what());
			}
		}
		break;

    	case AK_SETTING_SET_CAMERA_CAPTURE_REQ: {
			APP_DBG_SIG("AK_SETTING_SET_CAMERA_CAPTURE_REQ\n");

		}
		break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}
