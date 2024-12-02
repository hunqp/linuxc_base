#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>

#include "BaseMQTT.h"

#define PRIVATE_LOG_EN 		(1)

#if PRIVATE_LOG_EN
#define PRI_LOG(fmt, ...)	printf("\x1B[31m[BaseMQTT] \x1B[34m" fmt "\x1B[0m\r\n", ##__VA_ARGS__)
#else
#define PRI_LOG(fmt, ...)
#endif

BaseMQTT::BaseMQTT() : mosquittopp() {
	mConnected = false;
	mMutex = PTHREAD_MUTEX_INITIALIZER;
	mosqpp::lib_init();
	PRI_LOG("mosqpp::lib_init()");
}

BaseMQTT::~BaseMQTT() {
	mosquittopp::disconnect();
	mosquittopp::loop_stop();
    pthread_mutex_destroy(&mMutex);
}

void BaseMQTT::setUsernameAndPassword(std::string user, std::string pass) {
	if (user.length()) mUser.assign(user);
	if (pass.length()) mPass.assign(pass);
}

void BaseMQTT::setCertificate(std::string certificate, std::string tlsVersion) {
	if (certificate.length()) {
		tls.certificate.assign(certificate);
		tls.version.assign(tlsVersion);
	}
}

void BaseMQTT::setLastWillAndTestament(std::string topic, std::string payload, eQoS qos, bool retain) {
	if (topic.length()) {
		lwt.topic.assign(topic);
		lwt.payload.assign(payload);
		lwt.qos = qos;
		lwt.retain = retain;
	}
}

int BaseMQTT::performConnect(std::string host, int port, std::string id, int keepalive) {
	int ret = MOSQ_ERR_SUCCESS;

	/* -- Set TLS Certificate -- */
	if (!tls.certificate.empty()) {
		ret = tls_set(tls.certificate.c_str());
		if (ret != MOSQ_ERR_SUCCESS) {
			PRI_LOG("Can't use tls_set() %d", ret);
			return BMQ_ERR_FAILURE;
		}
		else PRI_LOG("Success tls_set() %d", ret);

		tls_insecure_set(false);
		ret = tls_opts_set(1, tls.version.c_str());
		if (ret != MOSQ_ERR_SUCCESS) {
			PRI_LOG("Can't use tls_opts_set() %d", ret);
			return BMQ_ERR_FAILURE;
		}
		else PRI_LOG("Success tls_opts_set() %d", ret);
	}
	
	/* -- Set last will and testament -- */
	if (lwt.topic.length() && lwt.payload.length()) {
		PRI_LOG("Last will topic {%s} message {%s}", lwt.topic.c_str(), lwt.payload.c_str());
		ret = mosquittopp::will_set(lwt.topic.data(), lwt.payload.length(), lwt.payload.data(), (int)lwt.qos, lwt.retain);
		if (ret != MOSQ_ERR_SUCCESS) {
			PRI_LOG("Can't use will_set() %d", ret);
		}
		else PRI_LOG("Success will_set() %d", ret);
	}

	/* -- Set username and password -- */
	if (mUser.length()) {
		PRI_LOG("Username {%s} Password {%s}", mUser.c_str(), mPass.c_str());
		ret = mosquittopp::username_pw_set(mUser.c_str(), mPass.length() ? mPass.c_str() : NULL);
		if (ret != MOSQ_ERR_SUCCESS) {
			PRI_LOG("Can't use username_pw_set() %d", ret);
		}
		else PRI_LOG("Success username_pw_set() %d", ret);
	}

	/* -- Perform connect to broker -- */
	PRI_LOG("Try connecting to %s on port %d", host.c_str(), port);
	ret = mosquittopp::connect_async(host.c_str(), port, keepalive);	
	if (ret == MOSQ_ERR_SUCCESS) {
		mosquittopp::loop_start();
	}
	else PRI_LOG("Can't connect_async(), %d", ret);

	return (ret == MOSQ_ERR_SUCCESS ? BMQ_ERR_SUCCESS : BMQ_ERR_FAILURE);
}

void BaseMQTT::forceDisconnect()  {
	mosquittopp::will_clear();
	mosquittopp::disconnect();
	mosquittopp::loop_stop();
}

bool BaseMQTT::isConnected() {
    pthread_mutex_lock(&mMutex);
    auto ret = mConnected;
    pthread_mutex_unlock(&mMutex);
	
    return ret;
}

int BaseMQTT::performPublish(std::string topic, std::string payload, eQoS qos, bool retain) {
	int ret = BMQ_ERR_FAILURE;

	if (!mConnected) {
		return false;
	}

	pthread_mutex_lock(&mMutex);
	ret = mosquittopp::publish(NULL, topic.c_str(), payload.length(), payload.c_str(), (int)qos, retain);
	pthread_mutex_unlock(&mMutex);

	return (ret == MOSQ_ERR_SUCCESS ? BMQ_ERR_SUCCESS : BMQ_ERR_FAILURE);
}

int BaseMQTT::performSubscribe(std::string topic, eQoS qos) {
	int ret = BMQ_ERR_FAILURE;
	auto bConnection = BaseMQTT::isConnected();

	if (!bConnection) {
		return false;
	}

	pthread_mutex_lock(&mMutex);
	ret = (mosquittopp::subscribe(NULL, topic.c_str(), (int)qos) == MOSQ_ERR_SUCCESS);
	pthread_mutex_unlock(&mMutex);

	return (ret == MOSQ_ERR_SUCCESS ? BMQ_ERR_SUCCESS : BMQ_ERR_FAILURE);
}

int BaseMQTT::performUnsubscribe(std::string topic) {
	int ret = BMQ_ERR_FAILURE;
	auto bConnection = isConnected();

	if (!bConnection) {
		return false;
	}

	pthread_mutex_lock(&mMutex);
	ret = (unsubscribe(NULL, topic.c_str()) == MOSQ_ERR_SUCCESS);
	pthread_mutex_unlock(&mMutex);

	return (ret == MOSQ_ERR_SUCCESS ? BMQ_ERR_SUCCESS : BMQ_ERR_FAILURE);
}