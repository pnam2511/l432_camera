/*
 * Noc_Lib_Debug.c
 *
 *  Created on: Nov 9, 2023
 *      Author: NUC
 */

#include "Noc_Lib_Debug.h"

#define MAGIC_KEY_LENGTH	8

bool bUserInputFlag = false;

uint8_t dbgInBuf[32];
uint8_t dbgInIdx = 0;

const unsigned char magicKey[MAGIC_KEY_LENGTH] = "aabbCC12";
uint8_t magicKeyIdx = 0;
bool isMagicKeyEnable = false;

void _showDebugMenuRoot(void)
{
	printf("\r\n");
	printf("===========================\r\n");
	printf("[Welcome to NOC Debug Menu]\r\n");
	printf("===========================\r\n");
	printf("1. Blink LED\r\n");
	printf("2. Enter Sleep Mode\r\n");
	printf("\r\n---------------------------\r\n");
	printf("99. Exit Menu\r\n");
	printf("\r\n>> ");
}

static void _parseMagicKey(void)
{
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
			}
		}
		else
		{
			dbgInIdx = 0;
			magicKeyIdx = 0;
		}
	}
}


void NocLib_DebugMenuHandler(void)
{
	_parseMagicKey();
	if (isMagicKeyEnable == true)
	{
		_showDebugMenuRoot();
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	/* Do something */
//	if (isalnum(readBuf))
//	{
//		bUserInputFlag = true;
//	}
//	else
//	{
//		bUserInputFlag = false;
//	}
//
//	/* Because HAL_UART_IRQHandler set huart->RxISR to NULL after every receive completed */
//	/* So we need to init it again */
//	HAL_UART_Receive_IT(huart, &readBuf, 1);
//
//}
