#include "cloud_api.h"

const std::string attributesSubscribeTopic() {
    return std::string("v1/devices/me/attributes");
}

const std::string attributesPublishTopic() {
    return std::string("v1/devices/me/attributes");
}

const std::string telemetryPublishTopic() {
    return std::string("v1/devices/me/telemetry");
}

const std::string remoteProcedureCallsPublishTopic(std::string reqId) {
    return std::string("v1/devices/me/rpc/response/" + reqId);
}

const std::string remoteProcedureCallsSubscribeTopic() {
    return std::string("v1/devices/me/rpc/request/+");
}
