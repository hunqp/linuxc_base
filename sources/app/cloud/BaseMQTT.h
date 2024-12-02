#ifndef _BASE_MQTT_H_
#define _BASE_MQTT_H_

#include <string>
#include <stdio.h>
#include <stdint.h>
#include <mosquitto.h>
#include <mosquittopp.h>

#include "cloud_api.h"

enum {
    BMQ_ERR_FAILURE = 0,
    BMQ_ERR_SUCCESS,
};

class BaseMQTT : public mosqpp::mosquittopp {
public:
    enum class eQoS {
        QoS0 = 0,
        QoS1,
        QoS2,
    };

	BaseMQTT();
	~BaseMQTT();

	void setUsernameAndPassword(std::string user="", std::string pass="");
    void setLastWillAndTestament(std::string topic, std::string payload, eQoS qos=eQoS::QoS0, bool retain=false);
    void setCertificate(std::string certificate, std::string tlsVersion);

    void forceDisconnect();
    bool isConnected();

    int performConnect(std::string host, int port, std::string id="anoymous-01", int keepalive=60);
    int performSubscribe(std::string topic, eQoS qos=eQoS::QoS0);
    int performUnsubscribe(std::string topic);
    int performPublish(std::string topic, std::string payload, eQoS qos=eQoS::QoS0, bool retain=false);

private:
    std::string mId;
	std::string mUser;
    std::string mPass;

    bool mConnected;

    struct {
        std::string certificate;
        std::string version;
    } tls;

    struct {
        std::string topic;
        std::string payload;
        eQoS qos;
        bool retain;
    } lwt;

    pthread_mutex_t	mMutex;

public:
	void on_connect(int rc) override;
	void on_disconnect(int rc) override;
	void on_message(const struct mosquitto_message *message) override;
};

#endif /* _BASE_MQTT_H_ */