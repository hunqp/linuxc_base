#ifndef __APP_DATA_H
#define __APP_DATA_H

#include <stdio.h>
#include <stdint.h>
#include <string>

#include "BaseMQTT.h"

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Enumarics -----------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/
typedef struct {
    std::string model;
    std::string productId;
    std::string hardware;
    std::string firmware;
    std::string status;
    uint32_t startup;
    uint32_t lastConnected;
} DeviceStatus_t;

typedef struct {
    std::string password;
    uint32_t beginTime;
    uint32_t endTime;
} AccessControlSetting_t;

/* Extern variables ----------------------------------------------------------*/
extern BaseMQTT ourPubSubClient;
extern DeviceStatus_t ourDeviceStatus;
extern AccessControlSetting_t ourACSetting;

/* Function prototypes -------------------------------------------------------*/
extern std::string attributesDeviceStatusPayload(DeviceStatus_t *deviceStatus);
extern std::string telemetrySensorStatusPayload();
extern std::string telemetrySensorValuesPayload();
extern std::string telemetryCameraCapturePayload();

#endif /* __APP_DATA_H */
