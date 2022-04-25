/*
 * consileIO.h
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
 */

#ifndef CLI_INC_CONSOLEIO_H_
#define CLI_INC_CONSOLEIO_H_

#include <stdint.h>


typedef enum {CONSOLE_SUCCESS = 0u, CONSOLE_ERROR = 1u } eConsoleError;

eConsoleError ConsoleIoInit(void);

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength);
eConsoleError ConsoleIoSendString(const char *buffer); // must be null terminated

#endif /* CLI_INC_CONSOLEIO_H_ */
