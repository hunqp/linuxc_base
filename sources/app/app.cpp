#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"
#include "app_config.h"

/* Extern variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static void sigProc(int sig) {
	static bool collapsed = false;
	if (!collapsed) {
		collapsed = true;
	}
	(void)sig;
	APP_PRINT("%d is terminated\r\n", getpid());
	exit(EXIT_SUCCESS);
}

/* Private function prototypes -----------------------------------------------*/
static void unitTest(void);

/* Function implementation ---------------------------------------------------*/
void task_init() {
	signal(SIGSEGV, sigProc); /// Catch Segmentation Fault
	signal(SIGABRT, sigProc); /// Catch Abort
	signal(SIGBUS, 	sigProc); /// Catch Bus Error
	signal(SIGFPE, 	sigProc); /// Catch Floating Point Exception
	signal(SIGILL, 	sigProc); /// Catch Illegal Instruction
	signal(SIGTERM, sigProc); /// Catch Termination Signal
	signal(SIGINT, 	sigProc); /// Catch Interrupt Signal
	signal(SIGQUIT, sigProc); /// Catch Quit Signal

	ourDeviceStatus.model = APP_MODEL;
	ourDeviceStatus.productId = APP_PRODUCT_ID;
	ourDeviceStatus.hardware = APP_HARDWARE_VERSION;
	ourDeviceStatus.firmware = APP_FIRMWARE_VERSION;
	ourDeviceStatus.startup = time(NULL);
	ourDeviceStatus.startup = time(NULL);

	if (0) {
		unitTest();
		exit(EXIT_SUCCESS);
	}
}

#include "base64.h"

static const char BASE64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *convertJPG2BASE64(const char *JPG) {
    unsigned char in[3];
    unsigned char ou[4];
    int nread, i, j = 0;

    FILE *fp = fopen(JPG, "rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    const long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* Allocate memory for base64 encoded string (4/3 of file size + padding) */
    char *b64 = (char *)malloc((fileSize * 4 / 3) + 4);
    if (!b64) {
        fclose(fp);
        return NULL;
    }

    /* Process the file in chunks of 3 bytes */
    while ((nread = fread(in, 1, 3, fp)) > 0) {
        ou[0] = in[0] >> 2;
        ou[1] = ((in[0] & 0x03) << 4) | (in[1] >> 4);
        ou[2] = ((in[1] & 0x0F) << 2) | (in[2] >> 6);
        ou[3] = in[2] & 0x3F;

        for (i = 0; i < nread + 1; i++) {
            b64[j++] = BASE64_TABLE[ou[i]];
        }

        while (nread++ < 3) {
            b64[j++] = '=';
        }
    }
    b64[j] = '\0';
    fclose(fp);

    return b64;
}

void unitTest(void) {
    const char *JPG = "snapshot.jpg";
    const char *BASE64 = "snapshot.txt";
    // if (BASE64) {
    //     printf("%s\n", BASE64);  // Print the Base64 string
    //     free(BASE64);  // Don't forget to free the memory!
    // } else {
    //     printf("Error: Failed to convert the JPEG to Base64.\n");
    // }

	FILE *fp = fopen(JPG, "rb");
    fseek(fp, 0, SEEK_END);
    const long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

	char *bin = (char*)malloc(fileSize);
	fread(bin, sizeof(char), fileSize, fp);
	fclose(fp);

	int b64Len = BASE64_EncodeLen(fileSize);
	char *b64 = (char*)malloc(b64Len);
	memset(b64, 0, b64Len);
	BASE64_Encode(b64, bin, fileSize);

	printf("%s\r\n", b64);
	fp = fopen(BASE64, "w");
	fwrite(b64, sizeof(char), b64Len, fp);
	fclose(fp);

	free(b64);
}