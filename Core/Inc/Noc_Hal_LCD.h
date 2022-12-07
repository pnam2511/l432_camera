#ifndef _NOC_HAL_LCD_H_
#define _NOC_HAL_LCD_H_

#include "stm32l432xx.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "tjpgd.h"

typedef struct
{
	uint16_t width;
	uint16_t height;
	uint8_t  size;
} FontDef;

void load_jpg (
	FIL* fp,		 /* Open file object to load */
	void *work,		 /* Pointer to the working buffer (must be 4-byte aligned) */
	UINT sz_work	 /* Size of the working buffer (must be power of 2) */
);

/* Dot screen size */
#define DISP_XS	160
#define DISP_YS	128

/* Color values */
#define RGB16(r,g,b) (((r << 8) & 0xF800)|((g << 3) & 0x07E0)|(b >> 3))
#define	C_BLACK		RGB16(0,0,0)
#define	C_BLUE		RGB16(0,0,255)
#define	C_RED		RGB16(255,0,0)
#define	C_MAGENTA	RGB16(255,0,255)
#define	C_GREEN		RGB16(0,255,0)
#define	C_CYAN		RGB16(0,255,255)
#define	C_YELLOW	RGB16(255,255,0)
#define	C_WHITE		RGB16(255,255,255)
#define	C_LGRAY		RGB16(160,160,160)
#define	C_GRAY		RGB16(128,128,128)

extern const uint8_t Font5x7[];	/* Font image (FONTX2 format) */

/* Define SPI pin mode */
#define SPI_Cmd_Mode()  HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET);
#define SPI_Data_Mode() HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET);

#define CS_LOW()        HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET);
#define CS_HIGH()       HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET);

/* LCD Parameters define */
#define BYTES_PER_PIXEL   2  //16bit depth 5-6-5 => 2 bytes = 1 pixel
#define MAX_IMAGE_SIZE    (160*128*BYTES_PER_PIXEL)

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

void NocHalLCD_DrawPixel(uint16_t color);
void NocHalLCD_Init(void /* struct */);
void NocHalLCD_SetWindowAddr(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd);
void NocHalLCD_ClrScreen(void);
void NocHalLCD_DisplayImage(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom, const uint16_t *image);

/* Text functions */
void NocHalLCD_SetOSDFont(const uint8_t *font);
void NocHalLCD_WriteChar(uint16_t x, uint16_t y, uint8_t chr, uint32_t color);
void NocHalLCD_WriteString(uint16_t x, uint16_t y, const char* str, uint32_t color);

#endif /* _NOC_HAL_LCD_H_ */
