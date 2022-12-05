#ifndef _NOC_HAL_LCD_H_
#define _NOC_HAL_LCD_H_

#include "stm32l432xx.h"
#include <string.h>

/*void load_jpg (
	FIL* fp,		 Open file object to load
	void *work,		 Pointer to the working buffer (must be 4-byte aligned)
	UINT sz_work	 Size of the working buffer (must be power of 2)
);*/

/* Dot screen size */
#define DISP_XS	128
#define DISP_YS	160

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

void NocHalLCD_Init(void /* struct */);
void NocHalLCD_SetWindowAddr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void NocHalLCD_ClrScreen(void);
void NocHalLCD_DisplayImage(const uint16_t *image);

/* Text functions */
void disp_font_face (const uint8_t *font);
void disp_font_color (uint32_t color);
void disp_locate (int col, int row);
void disp_putc (uint8_t chr);

#endif /* _NOC_HAL_LCD_H_ */
