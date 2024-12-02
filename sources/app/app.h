#ifndef __APP_H
#define __APP_H

#include "ak.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: AK_TASK_DEVICE_ID
 *  Note: Message signals
 *----------------------------------------------------------------------------*/
/* Define timer */

/* Define signal */
enum {
    AK_DEVICE_INIT = AK_USER_DEFINE_SIG,
    AK_DEVICE_SETTING_ACCESS_CONTROL_REQ,
    AK_SETTING_SETTING_CAMERA_CAPTURE_REQ,
};

/*----------------------------------------------------------------------------*
 *  DECLARE: AK_TASK_CLOUD_ID
 *  Note: Message signals
 *----------------------------------------------------------------------------*/
/* Define timer */
#define AK_CLOUD_SERVER_TRY_CONNECT_INTERVAL    (5000)
#define AK_CLOUD_PERIOD_KEEP_ALIVE_INTERVAL     (60000)

/* Define signal */
enum {
	AK_CLOUD_TRY_CONNECT = AK_USER_DEFINE_SIG,
	AK_CLOUD_ON_CONNECTED,
    AK_CLOUD_ON_DISCONNECTED,
    AK_CLOUD_FORCE_DISCONNECTED,
    AK_CLOUD_SEND_TELEMETRY,
    AK_CLOUD_SEND_REMOTE_PROCEDURE_CALL,
    AK_CLOUD_SEND_ATTRIBUTES,
    AK_CLOUD_ON_MESSAGE,
};

/*----------------------------------------------------------------------------*
 *  DECLARE: AK_TASK_SYSTEM_ID
 *  Note: Message signals
 *----------------------------------------------------------------------------*/
/* Define timer */
#define AK_SYSTEM_KEEP_ALIVE_INTERVAL    (2000)

/* Define signal */
enum {
    AK_SYSTEM_KEEP_ALIVE = AK_USER_DEFINE_SIG,

};

/*----------------------------------------------------------------------------*
 *  DECLARE: AK_TASK_SENSOR_ID
 *  Note: Message signals
 *----------------------------------------------------------------------------*/
/* Define timer */
#define AK_SENSOR_UPDATE_INTERVAL (2000)

/* Define signal */
enum {
    AK_SENSOR_STATUS_UPDATE = AK_USER_DEFINE_SIG,
    AK_SENSOR_VALUE_UPDATE,
};

/*----------------------------------------------------------------------------*
 *  DECLARE: Common defines
 *  Note: 
 *----------------------------------------------------------------------------*/
#define APP_MODEL                   "OM-BTS"
#define APP_PRODUCT_ID              "00155df2ec4b"
#define APP_HARDWARE_VERSION        "1.0"
#define APP_FIRMWARE_VERSION        "1.0.0"

#define APP_CONNECTED_STATUS        "Connected"
#define APP_DISCONNECTED_STATUS     "Disconnected"
#define APP_UPGRADE_SUCCESS_STATUS  "Upgrade success"
#define APP_UPGRADE_FAILRE_STATUS   "Upgrade failure"

#endif /* __APP_H */


