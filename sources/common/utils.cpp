#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <vector>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <fstream>

#include "base64.h"
#include "utils.h"

namespace utls {

void bytetoHexChar(uint8_t ubyte, uint8_t *uHexChar) {
	uHexChar[1] = ((ubyte & 0x0F) < 10) ? ((ubyte & 0x0F) + '0') : (((ubyte & 0x0F) - 10) + 'a');
	uHexChar[0] = ((ubyte >> 4 & 0x0F) < 10) ? ((ubyte >> 4 & 0x0F) + '0') : (((ubyte >> 4 & 0x0F) - 10) + 'a');
}

void bytestoHexChars(uint8_t *ubyte, int32_t len, uint8_t *uHexChar) {
	for (int8_t i = 0; i < len; i++) {
		bytetoHexChar(ubyte[i], (uint8_t *)&uHexChar[i * 2]);
	}
}

void hexChartoByte(uint8_t *uHexChar, uint8_t *ubyte) {
	*ubyte = 0;
	*ubyte = ((uHexChar[0] <= '9' && uHexChar[0] >= '0') ? ((uHexChar[0] - '0') << 4) : *ubyte);
	*ubyte = ((uHexChar[0] <= 'f' && uHexChar[0] >= 'a') ? ((uHexChar[0] - 'a' + 10) << 4) : *ubyte);

	*ubyte = ((uHexChar[1] <= '9' && uHexChar[1] >= '0') ? *ubyte | (uHexChar[1] - '0') : *ubyte);
	*ubyte = ((uHexChar[1] <= 'f' && uHexChar[1] >= 'a') ? *ubyte | ((uHexChar[1] - 'a') + 10) : *ubyte);
}

void hexCharsToBytes(uint8_t *uHexChar, int32_t len, uint8_t *ubyte) {
	for (int8_t i = 0; i < len; i += 2) {
		hexChartoByte((uint8_t *)&uHexChar[i], (uint8_t *)&ubyte[i / 2]);
	}
}

UTILS_RET writeRawFile(const std::string& url, const std::string& in) {
	FILE *f;
	
	f = fopen(url.c_str(), "w");
	if (f == NULL) {
		return UTILS_RET_ERR_ACCESS;
	}

	fwrite(in.data(), 1, in.length(), f);
	fsync(fileno(f));
	fclose(f);

	return UTILS_RET_SUCCESS;
}

UTILS_RET writeJSONFile(const std::string& url, const json& in) {
	return writeRawFile(url, in.dump());
}

UTILS_RET readRawFile(const std::string url, std::string& ou) {
	struct stat STAT;
	int fd = -1;
	char *buffer;

	fd = open(url.c_str(), O_RDONLY);
	if (fd < 0) {
		return UTILS_RET_ERR_ACCESS;
	}

	fstat(fd, &STAT);
	buffer = (char *)malloc(STAT.st_size);
	if (buffer == NULL) {
		close(fd);
		return UTILS_RET_ERR_NO_MEM;
	}

	ssize_t n = pread(fd, buffer, STAT.st_size, 0);
	(void)n;
	close(fd);

	ou.assign(std::string((const char*)buffer, STAT.st_size));
	free(buffer);

	return UTILS_RET_SUCCESS;
}

UTILS_RET readJSONFile(const std::string url, json& ou) {
	std::string content;

	if (UTILS_RET_SUCCESS == readRawFile(url, content)) {

		try {
			ou = json::parse(content);
			return UTILS_RET_SUCCESS;
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}

	return UTILS_RET_ERR_ACCESS;
}

bool doesExisted(const std::string url) {
	bool ret = false;

	if (access(url.c_str(), F_OK) != -1) {
		ret = true;
	}

	return ret;
}

std::vector<std::string> splitString(std::string &s, std::string delimiter) {
	size_t start = 0, end, delimiterLength = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((end = s.find(delimiter, start)) != std::string::npos) {
		token	  = s.substr(start, end - start);
		start = end + delimiterLength;
		if (token.length()) {
			res.push_back(token);
		}
	}

	res.push_back(s.substr(start));
	return res;
}

size_t fileSize(const std::string url) {
	struct stat STAT;
	int fd = -1;

	fd = open(url.c_str(), O_RDONLY);
	
	if (fd < 0) {
		return UTILS_RET_ERR_ACCESS;
	}

	fstat(fd, &STAT);
	close(fd);

	return STAT.st_size;
}

uint64_t currentTimeInMicroSeconds() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return uint64_t(time.tv_sec) * 1000 * 1000 + time.tv_usec;
}

uint32_t currentUnixTimestamp() {
	time_t t = time(NULL);
	return (uint32_t)t;
}

int cpuStatus() {
	char cmd[64];

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "top -b -n 1 | grep %d | awk 'NR==1 {print $7}'", getpid());
	FILE *fp;
    char percent[8];

    fp = popen(cmd, "r");
    if (fp == NULL) {
        return 0;
    }
	while (fgets(percent, sizeof(percent), fp) != NULL);
	pclose(fp);

    return atoi(percent);
}

uint64_t memoryUsage() {
	FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        return 0;
    }

    char buffer[256];
    uint64_t totalMemory = 0, freeMemory = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer, "MemTotal: %lu kB", &totalMemory);
        }
		else if (strncmp(buffer, "MemFree:", 8) == 0) {
            sscanf(buffer, "MemFree: %lu kB", &freeMemory);
        }
    }

    fclose(file);

    return totalMemory - freeMemory; /* Return used memory in kB */
}

std::string getIPString(const char *cmd) {
	(void)cmd;
	return "";
}

std::string getMACString() {
	char chars[32];
	const char *url = (const char*)"/sys/class/net/eth0/address";

    FILE *f = fopen(url, "r");
    if (f == NULL) {
        return "";
    }

	memset(chars, 0, sizeof(chars));
    if (fgets(chars, sizeof(chars), f) == NULL) {
        return NULL;
    }

    size_t len = strlen(chars);
    if (len > 0 && chars[len - 1] == '\n') {
        chars[len - 1] = '\0';
    }

    fclose(f);
    
    return std::string(chars, strlen(chars));
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    // Calculate the number of bytes received
    size_t total_size = size * nmemb;

	printf("total_size: %ld\r\n", total_size);

    // Reallocate memory to store the response
    data = (char*)realloc(data, total_size + 1);

    // Copy the received data into the buffer
    if (data) {
        strncat(data, (char*)ptr, total_size);
    }

    // Return the number of bytes handled
    return total_size;
}

int HTTP_GET_DIGEST(const char *url) {
	CURL *curl;
    CURLcode res;
    char *response_data = (char*)malloc(1);  // Initialize response buffer (1 byte for null terminator)
    response_data[0] = '\0';          // Ensure it's empty initially

    // Initialize the CURL library
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Enable digest authentication and provide credentials
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:epcb2020");

        // Set up the callback to handle the response content
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);

        // Perform the GET request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Response:\n%s\n", response_data);  // Print the response content
        }

        // Clean up the CURL handle
        curl_easy_cleanup(curl);
    }

    // Global curl cleanup
    curl_global_cleanup();

    // Free the response buffer
    free(response_data);

    return 0;
}

size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

// Base64 encoding table
const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode binary data to Base64
void encode_base64(FILE *input_file, FILE *output_file) {
    unsigned char in[3];
    unsigned char out[4];
    int i, j;
    size_t input_size;

    while ((input_size = fread(in, 1, 3, input_file)) > 0) {
        // Output the Base64 encoding
        out[0] = (in[0] >> 2) & 0x3F;
        out[1] = ((in[0] << 4) & 0x30) | ((in[1] >> 4) & 0x0F);
        out[2] = ((in[1] << 2) & 0x3C) | ((in[2] >> 6) & 0x03);
        out[3] = in[2] & 0x3F;

        // Handle padding
        for (i = input_size; i < 3; i++) {
            out[i + 1] = 64;  // Padding index in Base64 table is 64 ('=')
        }

        // Write the encoded data to the output file
        for (i = 0; i < 4; i++) {
            fputc(base64_table[out[i]], output_file);
        }
    }
}

// Function to perform HTTP GET request and download the image
int HTTP_GET(const char *url) {
    CURL *curl;
    CURLcode res;
    FILE *fp;

    // URL to download the snapshot from
    const char *JPG = "snapshot.jpg";
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Open file to save the downloaded snapshot
        fp = fopen(JPG, "wb");
        if (fp == NULL) {
            perror("Failed to open file");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        // Set Digest authentication
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:epcb2020");

        // Perform the download
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    FILE *input_file = fopen("snapshot.jpg", "rb");  // Open the image in binary mode
    if (input_file == NULL) {
        fprintf(stderr, "Error opening file!\n");
        return 1;
    }

    FILE *output_file = fopen("image_base64.txt", "w");  // Open an output file to store the base64 string
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file!\n");
        fclose(input_file);
        return 1;
    }

    // Encode the image to base64
    encode_base64(input_file, output_file);

    printf("Base64 encoding of 'image.jpeg' completed.\n");

    fclose(input_file);
    fclose(output_file);

    return 0;
}

}