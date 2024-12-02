#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
#include <sys/stat.h>

#include "ak.h"

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "task_list.h"
#include "if_console.h"

#include "utils.h"

#define TAG "InterfaceConsole"

/* Extern variables ----------------------------------------------------------*/
pthread_t csInterfaceThreadId;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void parserInputCs(uint8_t num);

/* Function implementation ---------------------------------------------------*/
void* csInterfaceCb(void* argv) {
	char ch;
	for (;;) {
		ch = getchar();
		if (ch >= '0' && ch <= '9') {
			parserInputCs(atoi(&ch));
			while ((ch = getchar()) == ('\n' || '\r') && ch != EOF); /* Flush input buffer */
		}
		else {
			parserInputCs(COMMAND_LIST);
		}
		usleep(100000);
	}
	return (void*)0;
}

void parserInputCs(uint8_t num) {
	APP_PRINT("> %d\n", num);

	switch (num) {
	case COMMAND_LIST: {
	#if 0
		APP_PRINT("\r\n"
				"     _/_/_/  _/        _/_/_/  \r\n"
				"  _/        _/          _/     \r\n"
				" _/        _/          _/      \r\n"
				"_/        _/          _/       \r\n"
				" _/_/_/  _/_/_/_/  _/_/_/ 		\r\n"
			);
		APP_PRINT("\t-[%2d]. COMMANDS LIST\r\n", COMMAND_LIST);
	#endif
	}
	break;

	case 1: {
		utls::HTTP_GET_DIGEST("http://192.168.1.3/cgi-bin/recordFinder.cgi?action=getQuerySize&name=AccessControlCustomPassword");
		sleep(1);
		utls::HTTP_GET_DIGEST("http://192.168.1.3/cgi-bin/recordUpdater.cgi?action=insert&name=AccessControlCustomPassword&UserID=105&OpenDoorPassword=111&Doors[0]=0&Doors[1]=1");
	}
	break;

	case 2: {
		// http://admin:epcb2020@192.168.1.234/cgi-bin/snapshot.cgi

		// utls::HTTP_GET("http://admin:epcb2020@192.168.1.234/cgi-bin/snapshot.cgi?chn=1");
		utls::HTTP_GET("http://192.168.1.234/cgi-bin/snapshot.cgi?chn=1");
	}
	break;

	case 3: {
		const char *JPG = "snapshot.jpg";
		const char *B64 = utls::convertJPG2BASE64(JPG);
		const int B64Len = strlen(B64);
		const std::string payload = std::string(B64, B64Len);
		json JSON;
		JSON["type"] = "CAMERA_CAPTURE";
		JSON["values"] = payload;

		int ret = ourPubSubClient.publishTelemetryTimeseries(JSON.dump());
		printf("Send telemetry timeseries size %d\r\n", ret);
		free((void*)B64);
	}
	break;

	case 4: {
		static bool b = false;
		
		if (b) {
			
		}
		else {
			
		}
		
		APP_PRINT("Set event type %s\r\n", b ? "MOTION" : "HUMAN");
		b = !b;
	}
	break;

	case 5: {
		static bool b = false;
		if (b) {
			
		}
		else {
			
		}
		b = !b;
	}
	break;

	default: {
		APP_PRINT("1.\tTEST STREAM WITH PREVIOUS 5 SECONDS.\n");
		APP_PRINT("2.\tTEST AUDIO OUTPUT\n");
		APP_PRINT("3.\tTEST FLOOD LIGHT CONTROL\n");
		APP_PRINT("4.\tCOMMING SOON.\n");
	}
	break;
	}
}