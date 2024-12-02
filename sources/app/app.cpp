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
}
