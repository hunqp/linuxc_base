#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"

#include "utils.h"

/* Extern variables ----------------------------------------------------------*/
BaseMQTT ourPubSubClient;
DeviceStatus_t ourDeviceStatus;
AccessControlSetting_t ourACSetting;

const char *INTRODUCTION = (const char*)"\r\n"                                                                           
    "    _/    _/  _/_/_/_/  _/        _/          _/_/        _/          _/    _/_/    _/_/_/    _/        _/_/_/    \r\n"
    "   _/    _/  _/        _/        _/        _/    _/      _/          _/  _/    _/  _/    _/  _/        _/    _/   \r\n"
    "  _/_/_/_/  _/_/_/    _/        _/        _/    _/      _/    _/    _/  _/    _/  _/_/_/    _/        _/    _/    \r\n"
    " _/    _/  _/        _/        _/        _/    _/        _/  _/  _/    _/    _/  _/    _/  _/        _/    _/     \r\n"
    "_/    _/  _/_/_/_/  _/_/_/_/  _/_/_/_/    _/_/            _/  _/        _/_/    _/    _/  _/_/_/_/  _/_/_/        \r\n"
    "\r\n"
    ;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Function implementation ---------------------------------------------------*/
std::string attributesDeviceStatusPayload(DeviceStatus_t *deviceStatus) {
    json attributes = {
        {"Model"	,	ourDeviceStatus.model           },
        {"ProductID",	ourDeviceStatus.productId       },
        {"Hardware"	,	ourDeviceStatus.hardware        },
        {"Firmware"	,	ourDeviceStatus.firmware        },
        {"Status"	,	ourDeviceStatus.status          },
        {"Startup"	,	ourDeviceStatus.startup         },
        {"Timestamp",	ourDeviceStatus.lastConnected   },
    };
    return attributes.dump();
}

std::string telemetrySensorStatusPayload() {
    json telemetry;
    
    telemetry["type"] = "SENSOR_STATUS";
    telemetry["values"]["Doors"][0] = "DISCONNECT";
    telemetry["values"]["Doors"][1] = "DISCONNECT";
    telemetry["values"]["Flood"] = "DISCONNECT";
    telemetry["values"]["Heat"] = "DISCONNECT";
    telemetry["values"]["Smoke"] = "DISCONNECT";

    return telemetry.dump();
}

std::string telemetrySensorValuesPayload() {
    json telemetry = {
        {"Model"	,	ourDeviceStatus.model           },
        {"ProductID",	ourDeviceStatus.productId       },
        {"Hardware"	,	ourDeviceStatus.hardware        },
        {"Firmware"	,	ourDeviceStatus.firmware        },
        {"Status"	,	ourDeviceStatus.status          },
        {"Startup"	,	ourDeviceStatus.startup         },
        {"Timestamp",	ourDeviceStatus.lastConnected   },
    };
    return telemetry.dump();
}

std::string telemetryCameraCapturePayload() {
    json telemetry = {
        {"Model"	,	ourDeviceStatus.model           },
        {"ProductID",	ourDeviceStatus.productId       },
        {"Hardware"	,	ourDeviceStatus.hardware        },
        {"Firmware"	,	ourDeviceStatus.firmware        },
        {"Status"	,	ourDeviceStatus.status          },
        {"Startup"	,	ourDeviceStatus.startup         },
        {"Timestamp",	ourDeviceStatus.lastConnected   },
    };
    return telemetry.dump();
}