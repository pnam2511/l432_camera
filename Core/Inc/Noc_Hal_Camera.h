#ifndef _NOC_HAL_CAMERA_H_
#define _NOC_HAL_CAMERA_H_

#include "stm32l432xx.h"

/* REGISTER DEFINE */
#define CAMERA_ADDR   0x42

#define VREF_REG      0x03
#define CLKRC_REG     0x11
#define COM7_REG      0x12
#define PID_REG       0x0A
#define VER_REG       0x0B
#define COM3_REG      0x0C
#define COM8_REG      0x13
#define COM10_REG     0x15
#define HSTART_REG    0x17
#define HSTOP_REG     0x18
#define VSTART_REG    0x19
#define VSTOP_REG     0x1A
#define HREF_REG      0x32
#define COM11_REG     0x3B
#define COM14_REG     0x3E
#define COM15_REG     0x40
#define DBLV_REG	  0x6B
#define AWBCTR0_REG   0x6F
#define SCALING_XSC   0x70
#define SCALING_YSC   0x71
#define SCALING_DCWCTR       0x72
#define SCALING_PCLK_DIV     0x73
#define SCALING_PCLK_DELAY   0xA2

void Camera_Config(void);

#endif /* _NOC_HAL_CAMERA_H_ */
