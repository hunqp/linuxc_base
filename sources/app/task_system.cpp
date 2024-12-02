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

#include "if_console.h"
#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "task_list.h"
#include "task_system.h"

#include "utils.h"

#define TAG	"TaskSystem"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskSystemMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Function implementation ---------------------------------------------------*/
void* TaskSystemEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_SYSTEM_ID Entry\n");

	pthread_create(&csInterfaceThreadId, NULL, csInterfaceCb, NULL);

	while (1) {
		msg = ak_msg_rev(AK_TASK_SYSTEM_ID);

		switch (msg->header->sig) {
        case AK_SYSTEM_KEEP_ALIVE: {
           
        }
        break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}
