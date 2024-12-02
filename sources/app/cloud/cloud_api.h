#ifndef __CLOUD_API_H
#define __CLOUD_API_H

#include <string>

extern const std::string telemetryPublishTopic();
extern const std::string attributesSubscribeTopic();
extern const std::string attributesPublishTopic();
extern const std::string remoteProcedureCallsPublishTopic(std::string reqId);
extern const std::string remoteProcedureCallsSubscribeTopic();

#endif /* __CLOUD_API_H */