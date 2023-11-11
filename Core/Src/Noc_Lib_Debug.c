/*
 * Noc_Lib_Debug.c
 *
 *  Created on: Nov 9, 2023
 *      Author: NUC
 */

#include "Noc_Lib_Debug.h"

#define MAGIC_KEY_LENGTH	8
#define DEBUG_BUFFER 		32

bool bUserInputFlag = false;

uint8_t dbgInBuf[DEBUG_BUFFER];
uint8_t dbgInIdx = 0;
NocDbgState debugState = NOC_DEBUG_STATE_NONE;

const unsigned char magicKey[MAGIC_KEY_LENGTH] = "20102011";
uint8_t magicKeyIdx = 0;
bool isMagicKeyEnable = false;

static void _showDebugMenuRoot(void)
{
    printf("\r\n");
    printf("===========================\r\n");
    printf("[Welcome to NOC Debug Menu]\r\n");
    printf("===========================\r\n");
    printf("1. Blink LED\r\n");
    printf("2. Enter Sleep Mode\r\n");
    printf("\r\n---------------------------\r\n");
    printf("99. Exit Menu\r\n");
    printf(">> \r\n");
}

static void _handleDebugMenuRoot(uint8_t input)
{
    switch (input)
    {
      case 1:
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	break;

      case 2:
	printf("Case 2: \r\n");
	break;

      case 99:
	printf("Good bye~. Closing Debug Menu\r\n");
	isMagicKeyEnable = false;
	magicKeyIdx = 0;
	dbgInIdx = 0;

	debugState = NOC_DEBUG_STATE_NONE;
	break;

      default:
	/* Do nothing */
	break;
    }
}

static void _parseMagicKey(void)
{
    if (isMagicKeyEnable == true)
      return;

    if (magicKeyIdx >= MAGIC_KEY_LENGTH)
    {
	magicKeyIdx = 0;
        return;
    }

    if (dbgInIdx >= DEBUG_BUFFER)
    {
	dbgInIdx = 0;
        return;
    }

    if (bUserInputFlag == true)
    {
	bUserInputFlag = false;
	dbgInBuf[dbgInIdx] = readBuf;

	if (dbgInBuf[dbgInIdx] == magicKey[magicKeyIdx])
	{
	    printf("%d th Key is %c\r\n", dbgInIdx + 1, magicKey[magicKeyIdx]);
	    dbgInIdx++;
	    magicKeyIdx++;

	    if (magicKeyIdx == MAGIC_KEY_LENGTH)
	    {
		printf("BINGO!!! You have unlocked the Debug menu\r\n");
		isMagicKeyEnable = true;
		dbgInIdx = 0;
		magicKeyIdx = 0;
	    }
	}
	else
	{
	    dbgInIdx = 0;
	    magicKeyIdx = 0;
	}
    }
}

static void _processUserInput(void)
{
    if ((!isMagicKeyEnable) || (debugState != NOC_DEBUG_STATE_READY))
    {
	return;
    }

    if (dbgInIdx >= DEBUG_BUFFER)
    {
	printf("Buffer Overflow!!! Please input less than 32 chars\r\n");
	dbgInIdx = 0;
        return;
    }

    if (bUserInputFlag == true)
    {
	bUserInputFlag = false;
	dbgInBuf[dbgInIdx++] = readBuf;

	if ((readBuf != DEBUG_ASCII_CR) && (readBuf != DEBUG_ASCII_NL))
	{
	    printf("%c\r\n", readBuf);	// echo user input
	}
	else
	{
	    _handleDebugMenuRoot(atoi(dbgInBuf));
	    dbgInIdx = 0;
	    return;
	}
    }
}

void NocLib_DebugMenuHandler(void)
{
    _parseMagicKey();

    if (isMagicKeyEnable && (debugState == NOC_DEBUG_STATE_NONE))
    {
	_showDebugMenuRoot();
	debugState = NOC_DEBUG_STATE_READY;
    }

    _processUserInput();
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  /* Do something */
//  if(_isValidInput(readBuf))
//    bUserInputFlag = true;
//  else
//    bUserInputFlag = false;
//
//  /* Because HAL_UART_IRQHandler set huart->RxISR to NULL after every receive completed */
//  /* So we need to init it again */
//  HAL_UART_Receive_IT(huart, &readBuf, 1);
//}
//
//static bool _isValidInput(uint8_t input)
//{
//  if ((input >= DEBUG_ASCII_0) || (input == DEBUG_ASCII_NL) || (input == DEBUG_ASCII_CR))
//  {
//    return true;
//  }
//  return false;
//}
