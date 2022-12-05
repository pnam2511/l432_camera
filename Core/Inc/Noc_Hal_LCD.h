#ifndef _NOC_HAL_LCD_H_
#define _NOC_HAL_LCD_H_

#include "stm32l432xx.h"

/* Define SPI pin mode */
#define SPI_Cmd_Mode()  HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET);
#define SPI_Data_Mode() HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET);

#define CS_LOW()        HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET);
#define CS_HIGH()       HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET);

/* LCD Parameters define */
#define BYTES_PER_PIXEL   2  //16bit depth 5-6-5 => 2 bytes = 1 pixel
#define MAX_IMAGE_SIZE    160*128*BYTES_PER_PIXEL

/* REGISTER DEFINE */
#define ST7735_SWRESET   0x01
#define ST7735_SLPOUT    0x11
#define ST7735_CASET     0x2A
#define ST7735_RASET     0x2B
#define ST7735_RAMWR     0x2C
#define ST7735_MADCTL    0x36
#define ST7735_CASET     0x2A
#define ST7735_COLMOD    0x3A
#define ST7735_FRMCTR1   0xB1
#define ST7735_FRMCTR2   0xB2
#define ST7735_FRMCTR3   0xB3
#define ST7735_NORON     0x13
#define ST7735_DISPON    0x29

void NocHalLCD_Init(void /* struct */);
void NocHalLCD_SetWindowAddr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void NocHalLCD_ClrScreen(void);
void NocHalLCD_DisplayImage(const uint16_t *image);

#endif /* _NOC_HAL_LCD_H_ */
