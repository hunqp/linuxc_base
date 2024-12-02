#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <pthread.h>
#include <sys/un.h>
#include <iostream>

#include "ak.h"

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "app_config.h"
#include "task_list.h"
#include "task_cloud.h"

#include "utils.h"

#define TAG	"TaskCloud"

/* Extern variables ----------------------------------------------------------*/
q_msg_t taskCloudMailbox;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void onMessage(std::string& topic, std::string &payload);

/* Function implementation ---------------------------------------------------*/
void* TaskCloudEntry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

    APP_PRINT("[STARTED] AK_TASK_CLOUD_ID Entry\n");

	const std::string host = "broker.hivemq.com";
	const int port = 1883;
	const std::string username = "t1n0kdn2mgvq8ac1fw8g";
	const std::string password = "";
	const std::string lwTopic = attributesPublishTopic();
	const json lwMessage = {{"Status", "Disconnected"}};

	APP_PRINT("MQTT CONNECTION\r\n");
	APP_PRINT("\t-Broker host: %s\r\n", host.c_str());
	APP_PRINT("\t-Broker port: %d\r\n", port);
	APP_PRINT("\t-Username: %s\r\n", username.c_str());
	APP_PRINT("\t-Password: %s\r\n", password.c_str());

	// ourPubSubClient.setUsernameAndPassword(username, password);
	// ourPubSubClient.setLastWillAndTestament(lwTopic, lwMessage.dump(), BaseMQTT::eQoS::QoS1, true);

	// TaskPostPureMsg(AK_TASK_CLOUD_ID, AK_CLOUD_TRY_CONNECT);

	while (1) {
		msg = ak_msg_rev(AK_TASK_CLOUD_ID);

		switch (msg->header->sig) {
		case AK_CLOUD_TRY_CONNECT: {
			APP_DBG_SIG("AK_CLOUD_TRY_CONNECT\n");
			if (!ourPubSubClient.performConnect(host, port)) {
				TimerSet(AK_TASK_CLOUD_ID, 
						 AK_CLOUD_TRY_CONNECT, 
						 AK_CLOUD_SERVER_TRY_CONNECT_INTERVAL, 
						 TIMER_ONE_SHOT);
			}
		}
		break;

		case AK_CLOUD_ON_CONNECTED: {
			APP_DBG_SIG("AK_CLOUD_ON_CONNECTED\n");

			ourPubSubClient.performSubscribe(attributesSubscribeTopic());
			ourPubSubClient.performSubscribe(remoteProcedureCallsSubscribeTopic());
			ourDeviceStatus.lastConnected = time(NULL);
			
			std::string attributes = attributesDeviceStatusPayload(&ourDeviceStatus);
			TaskPostDynamicMsg(AK_TASK_CLOUD_ID, AK_CLOUD_SEND_ATTRIBUTES, (uint8_t*)attributes.c_str(), attributes.length());
		}
		break;

		case AK_CLOUD_ON_DISCONNECTED: {
			APP_DBG_SIG("AK_CLOUD_ON_DISCONNECTED\n");

			TaskPostPureMsg(AK_TASK_CLOUD_ID, AK_CLOUD_TRY_CONNECT);
		}
		break;

		case AK_CLOUD_FORCE_DISCONNECTED: {
			APP_DBG_SIG("AK_CLOUD_FORCE_DISCONNECTED\n");

			ourPubSubClient.forceDisconnect();
		}
		break;

		case AK_CLOUD_SEND_TELEMETRY: {
			APP_DBG_SIG("AK_CLOUD_SEND_TELEMETRY\n");

			std::string telemetry = std::string((char*)msg->header->payload, msg->header->len);
			ourPubSubClient.performPublish(telemetryPublishTopic(), telemetry);
		}
		break;

		case AK_CLOUD_SEND_REMOTE_PROCEDURE_CALL: {
			APP_DBG_SIG("AK_CLOUD_SEND_REMOTE_PROCEDURE_CALL\n");

		}
		break;

		case AK_CLOUD_SEND_ATTRIBUTES: {
			APP_DBG_SIG("AK_CLOUD_SEND_ATTRIBUTES\n");

			std::string attributes = std::string((char*)msg->header->payload, msg->header->len);
			ourPubSubClient.performPublish(attributesPublishTopic(), attributes);
		}
		break;

		case AK_CLOUD_ON_MESSAGE: {
			APP_DBG_SIG("AK_CLOUD_ON_MESSAGE\n");

			struct mosquitto_message *mosqmsg = (mosquitto_message*)msg->header->payload;
			std::string topic = std::string(mosqmsg->topic, strlen(mosqmsg->topic));
			std::string payload = std::string((char*)mosqmsg->payload, strlen((char*)mosqmsg->payload));
			free(mosqmsg->topic);
			free(mosqmsg->payload);
			onMessage(topic, payload);
		}
		break;

		default:
        break;
		}
		
		ak_msg_free(msg);
	}

	return (void*)0;
}

void BaseMQTT::on_disconnect(int rc) {
	pthread_mutex_lock(&mMutex);
	mConnected = false;
	pthread_mutex_unlock(&mMutex);

	APP_DBG(TAG, "[rc=%d] %s : %s", rc, __func__, mosqpp::connack_string(rc));
	// TaskPostPureMsg(AK_TASK_CLOUD_ID, AK_CLOUD_ON_DISCONNECTED);
}

void BaseMQTT::on_connect(int rc) {
	pthread_mutex_lock(&mMutex);
	mConnected = true;
	pthread_mutex_unlock(&mMutex);
	
	APP_DBG(TAG, "[rc=%d] %s : %s", rc, __func__, mosqpp::connack_string(rc));
	TaskPostPureMsg(AK_TASK_CLOUD_ID, AK_CLOUD_ON_CONNECTED);
}

void BaseMQTT::on_message(const struct mosquitto_message *message) {
	struct mosquitto_message mosqmsg;

	mosqmsg.mid = message->mid;
	mosqmsg.topic = (char *)malloc(strlen(message->topic) + 1);
	memset(mosqmsg.topic, 0, strlen(message->topic) + 1);
	strcpy(mosqmsg.topic, message->topic);

	mosqmsg.payload = (char *)malloc(message->payloadlen + 1);
	memset(mosqmsg.payload, 0, message->payloadlen + 1);
	strncpy((char *)mosqmsg.payload, (char *)message->payload, message->payloadlen);
	mosqmsg.payloadlen = message->payloadlen;
	mosqmsg.qos = message->qos;
	mosqmsg.retain = message->retain;

	TaskPostDynamicMsg(AK_TASK_CLOUD_ID, AK_CLOUD_ON_MESSAGE, (uint8_t*)&mosqmsg, sizeof(struct mosquitto_message));
}

void onMessage(std::string& topic, std::string &payload) {
	APP_DBG(TAG, "TOPIC  : %s", topic.c_str());
	APP_DBG(TAG, "PAYLOAD: %s", payload.c_str());

	try {
		json JSON = json::parse(payload);
		auto strings = utls::splitString(topic, "/");
		/* 	Remote Procedure Calls Topic: 'v1/devices/me/rpc/request/+'
		*/
		if (strings.size() == 6) {
			JSON["request_id"] = strings[5];
			auto method = JSON["method"].get<std::string>();

			APP_DBG(TAG, "%s", JSON.dump(4).c_str());

			if (method == "CONTROL_IO") {
				
			}
			else if (method == "CONTROL_MODBUS") {
				
			}
			else if (method == "REBOOT") {
				
			}
			else if (method == "GENERIC_SETTING") {
				
			}
			else if (method == "PASSWORD_ACCESS_CONTROL") {
				TaskPostDynamicMsg(AK_TASK_SETTING_ID, AK_SETTING_SET_ACCESS_CONTROL_REQ, (uint8_t*)JSON.dump().c_str(), JSON.dump().length());
			}
			else if (method == "CAMERA_SETTING") {
				TaskPostDynamicMsg(AK_TASK_SETTING_ID, AK_SETTING_SET_CAMERA_CAPTURE_REQ, (uint8_t*)JSON.dump().c_str(), JSON.dump().length());
			}
			else {
				/* Put other RPC methods request here ... */
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << "onMessage\t" << e.what() << std::endl;
	}
}
