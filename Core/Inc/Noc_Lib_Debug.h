/*
 * Noc_Lib_Debug.h
 *
 *  Created on: Nov 9, 2023
 *      Author: NUC
 */

#ifndef INC_NOC_LIB_DEBUG_H_
#define INC_NOC_LIB_DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define DEBUG_ASCII_0		0x30
#define DEBUG_ASCII_NL		0x0A
#define DEBUG_ASCII_CR		0x0D

/* enum */
typedef enum
{
  NOC_DEBUG_STATE_READY,
  NOC_DEBUG_STATE_PROCESSING,
  NOC_DEBUG_STATE_NONE,
} NocDbgState;

/* Export functions prototype */
void NocLib_DebugMenuHandler(void);

#endif /* INC_NOC_LIB_DEBUG_H_ */
