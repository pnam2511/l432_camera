#ifndef _NOC_LIB_SYSTEM_H_
#define _NOC_LIB_SYSTEM_H_

#include "stm32l432xx.h"
#include "../../sdcard/sdcard.h"
#include "Noc_Hal_LCD.h"
#include "../../fatfs/ff.h"

typedef enum {
	SYS_OK = 0,				    /* (0) Succeeded */
	SDCARD_INIT_ERR,			/* (1) An error occured when initializing SD card */
	F_MOUNT_ERR,				/* (2) Mounting SD card error */
} nocSYSSTATUS;

nocSYSSTATUS NocLibSys_Init(void);
// void NocLibSys_ShowDirectory(char *path);

#endif /* _NOC_LIB_SYSTEM_H_ */
