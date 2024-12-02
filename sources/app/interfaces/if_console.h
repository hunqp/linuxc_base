#ifndef __IF_CONSOLE_H
#define __IF_CONSOLE_H

#include <stdint.h>
#include <pthread.h>

/*----------------------------------------------------------------------------*
 *  DECLARE: Public definitions
 *  Note: 
 *----------------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

/* Enumarics -----------------------------------------------------------------*/
enum {
    COMMAND_LIST = 0,
};

/* Extern variables ----------------------------------------------------------*/
extern pthread_t csInterfaceThreadId;

/* Function prototypes -------------------------------------------------------*/
extern void* csInterfaceCb(void * argv);

#endif /* __IF_CONSOLE_H */