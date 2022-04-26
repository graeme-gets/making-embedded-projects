/*
 * consoleCommands.h
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
 */

#ifndef CLI_SRC_CONSOLECOMMANDS_H_
#define CLI_SRC_CONSOLECOMMANDS_H_

#include <stdint.h>
#include "console.h"

#define CONSOLE_COMMAND_MAX_COMMAND_LENGTH 10		// command only
#define CONSOLE_COMMAND_MAX_LENGTH 100				// whole command with argument
#define CONSOLE_COMMAND_MAX_HELP_LENGTH 64			// if this is zero, there will be no  help (XXXOPT: RAM reduction)

#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
	#define HELP(x)  (x)
#else
	#define HELP(x)	  0
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH

typedef eCommandResult_T(*ConsoleCommand_T)(const char buffer[]);

typedef struct sConsoleCommandStruct
{
    const char* name;
    ConsoleCommand_T execute;
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
	char help[CONSOLE_COMMAND_MAX_HELP_LENGTH];
#else
	uint8_t junk;
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH
} sConsoleCommandTable_T;

#define CONSOLE_COMMAND_TABLE_END {NULL, NULL, HELP("")}

const sConsoleCommandTable_T* ConsoleCommandsGetTable(void);

#endif /* CLI_SRC_CONSOLECOMMANDS_H_ */
