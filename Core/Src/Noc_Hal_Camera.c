#include "Noc_Hal_Camera.h"
#include "main.h"
#include <stdio.h>

#define CameraI2C_Config hi2c1
#define _NOC_HAL_CAMERA_

extern TIM_HandleTypeDef htim1;

extern int frameCount;

extern uint8_t g_KeyPressed;
uint8_t volatile g_PreviousVsync = LOW;
uint8_t g_StartCapture = FALSE;

uint8_t dataBuf[2] = {0, 0};

static uint8_t Camera_I2C_Read(uint8_t regAddr, uint8_t *data)
{
  HAL_StatusTypeDef ret;

  do {
    // HAL_I2C_Mem_Read doesn't work (because of SCCB protocol(doesn't have ack))? */
    // ret = HAL_I2C_Mem_Read(sp_hi2c, SLAVE_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, 1, 1000);
    ret = HAL_I2C_Master_Transmit(&CameraI2C_Config, CAMERA_ADDR, &regAddr, 1, 100);
    ret |= HAL_I2C_Master_Receive(&CameraI2C_Config, CAMERA_ADDR, data, 1, 100);
  } while (ret != HAL_OK && 0);

  return ret;
}

static uint8_t Camera_I2C_Write(uint8_t regAddr, uint8_t data)
{
  HAL_StatusTypeDef ret;

  do {
    ret = HAL_I2C_Mem_Write(&CameraI2C_Config, CAMERA_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
  } while (ret != HAL_OK && 0);

  return ret;
}

void NocHalCamera_frameControl(int hstart, int hstop, int vstart, int vstop)
{
  Camera_I2C_Write(HSTART_REG, hstart >> 3);
  Camera_I2C_Write(HSTOP_REG, hstop >> 3);
  Camera_I2C_Write(HREF_REG, ((hstop & 0b111) << 3) | (hstart & 0b111));

  Camera_I2C_Write(VSTART_REG, vstart >> 2);
  Camera_I2C_Write(VSTOP_REG, vstop >> 2);
  Camera_I2C_Write(VREF_REG, ((vstop & 0b11) << 2) | (vstart & 0b11));
}

nocCAMRESULT NochalCamera_Config(void)
{
  // Register Reset
  Camera_I2C_Write(COM7_REG, 0x80);
  HAL_Delay(100);

  Camera_I2C_Read(PID_REG, dataBuf);
  Camera_I2C_Read(VER_REG, dataBuf);

  if(dataBuf[0] == 0x00)
    return CAM_ERR;
  if(dataBuf[0] != 0x73)
    return CAM_NOT_SUPPORT;

  Camera_I2C_Write(CLKRC_REG, 0x01); 			// Divided Pclock by 2
  Camera_I2C_Write(COM11_REG, 0b1000 | 0b10); 	// enable auto 50/60Hz detect + exposure timing can be less...
  Camera_I2C_Write(COM7_REG, 0b100); 			// rgb
  Camera_I2C_Write(COM15_REG, 0b11000000 | 0b010000); 	// rgb565

  // PCLK does not toggle during horizontal blank
  Camera_I2C_Write(COM10_REG, 0x20);

  // QQVGA Settings
  Camera_I2C_Write(COM3_REG,  0x04); 		// DCW/Downsampling is on
  Camera_I2C_Write(COM14_REG, 0x1A); 		// Scaling and dcw pclk-scale div by 4
  Camera_I2C_Write(SCALING_XSC, 0x3A); 		// hor. scale ratio[6-0]=58=0,5
  Camera_I2C_Write(SCALING_YSC, 0x35); 		// vertial scale ratio[6-0]=53=0,5
  Camera_I2C_Write(SCALING_DCWCTR, 0x22); 	// DCW/Downsampling mode: downsamplingHV 4x4 = reduce size by 16
  Camera_I2C_Write(SCALING_PCLK_DIV, 0xF2); 	// Clock divider on, divided by 4
  Camera_I2C_Write(SCALING_PCLK_DELAY, 0x02); 	// scaling pclk delay = 2clocks

  NocHalCamera_frameControl(196, 52, 8, 488);		// no clue why horizontal needs such strange values, vertical works ok

  Camera_I2C_Write(0xB0, 0x84); 			// no clue what this is but it's most important for colors
  Camera_I2C_Write(COM8_REG, 0xE7);			// AWB (Auto White Balance) on
  Camera_I2C_Write(AWBCTR0_REG, 0x9F);		// Simple AWB

  return CAM_OK;
}

void NocHalCamera_oneshotMode(void)
{
  if(g_KeyPressed == TRUE)
  {
    if(g_CurrentVsync != g_PreviousVsync)
    {
      if(g_PreviousVsync == HIGH)
      {
        g_StartCapture = TRUE;
        __HAL_TIM_ENABLE(&htim1);
      }
      g_PreviousVsync = g_CurrentVsync;
    }
  }
}