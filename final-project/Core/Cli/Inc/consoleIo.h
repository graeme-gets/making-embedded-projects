/*
 * consileIO.h
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
 */

#ifndef CLI_INC_CONSOLEIO_H_
#define CLI_INC_CONSOLEIO_H_

#include <stdint.h>
#define CLI_BUFFER_LENGTH 64
#define CONSOLE_ECHO	0

typedef enum {	CONSOLE_SUCCESS = 0u,
				CONSOLE_ERROR = 1u,
				CONSOLE_NO_STRING = 2u,
				CONSOLE_BUFFER_FULL = 3u
} eConsoleError;

eConsoleError ConsoleIoInit(void);

eConsoleError ConsoleIoReceive(uint8_t *buffer);
eConsoleError ConsoleIoSendString(const char *buffer); // must be null terminated

#endif /* CLI_INC_CONSOLEIO_H_ */
