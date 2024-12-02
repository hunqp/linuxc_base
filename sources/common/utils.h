#ifndef __UTILS_H
#define __UTILS_H

#include <ctime>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <stdint.h>

#include "json.hpp"

using json = nlohmann::json;

typedef enum {
	UTILS_RET_ERR_UNKNOWN 	= -1,
    UTILS_RET_ERR_ACCESS	= -2,
    UTILS_RET_ERR_ARG		= -3,
	UTILS_RET_ERR_NO_MEM	= -4,
	UTILS_RET_SUCCESS 		= 0,
} UTILS_RET;

namespace utls {

extern void bytetoHexChar(uint8_t ubyte, uint8_t *uHexChar);
extern void bytestoHexChars(uint8_t *ubyte, int32_t len, uint8_t *uHexChar);
extern void hexChartoByte(uint8_t *uHexChar, uint8_t *ubyte);
extern void hexCharsToBytes(uint8_t *uHexChar, int32_t len, uint8_t *ubyte);

extern std::vector<std::string> splitString(std::string &s, std::string delimiter);
extern bool doesExisted(const std::string url);
extern size_t fileSize(const std::string url);
extern UTILS_RET writeRawFile(const std::string& url, const std::string& in);
extern UTILS_RET writeJSONFile(const std::string& url, const json& in);
extern UTILS_RET readRawFile(const std::string url, std::string& ou);
extern UTILS_RET readJSONFile(const std::string url, json& ou);

extern uint64_t currentTimeInMicroSeconds();
extern uint32_t currentUnixTimestamp();

template<class T>
std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) {
	return ptr;
}

extern int cpuStatus();
extern uint64_t memoryUsage();
extern std::string getIPString(const char *cmd);
extern std::string getMACString();

extern int HTTP_GET(const char *url);
extern int HTTP_GET_DIGEST(const char *url);

}

#endif /* __UTILS_H */
