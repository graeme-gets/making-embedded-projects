// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.

#include "consoleIo.h"
#include <stdio.h>
#include "cBuffer.h"
#include "string.h"
#include "stringHelpers.h"
uint8_t cliBuffer[CLI_BUFFER_LENGTH];
cBuffer_t cliCB ; // Circular Buffer for the CLI
uint8_t cliRX;
extern UART_HandleTypeDef huart1;



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(&huart1, &cliRX, 1);
    if (CONSOLE_ECHO == 1)
    {
    	HAL_UART_Transmit(&huart1, &cliRX, 1, 100);

    }
    // TODO : This is not a great solution!
    if (cliRX != '\n') // ignore a 'new line'
    {
    	cBuffer_Write(&cliCB, cliRX);

    }
}

eConsoleError ConsoleIoInit(void)
{
	cBuffer_init(&cliCB,cliBuffer, CLI_BUFFER_LENGTH);
	HAL_UART_Receive_IT(&huart1, &cliRX, 1);
	return CONSOLE_SUCCESS;
}



eConsoleError ConsoleIoReceive(uint8_t *buffer)
{
	uint8_t len;

	if (cBuffer_GetString(&cliCB, buffer, '\r',&len) == CBUFFER_OK)
	{
		// clean up string
		buffer[len]  = 0x0; //Terminate String in C Style
		manageBackSpace((char*)buffer);
		return CONSOLE_SUCCESS;
	}
	else if (cBuffer_isFull(&cliCB) == CBUFFER_FULL)
	{
		// KILL The Buffer
		cBuffer_Kill(&cliCB);
		return CONSOLE_ERROR;
	}
	return CONSOLE_NO_STRING;

}

eConsoleError ConsoleIoSendString(const char *buffer)
{
	HAL_UART_Transmit(&huart1, (const uint8_t*)buffer, strlen(buffer), 100);
	return CONSOLE_SUCCESS;
}

