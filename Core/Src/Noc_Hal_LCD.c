#include "Noc_Hal_LCD.h"
#include "main.h"

#define NOC_HAL_LCD

#define ST7735_SPI_PORT hspi3

static const uint8_t *FontS;	/* Current font (ANK) */

FontDef font;

static void NocHalLCD_WriteByte(uint8_t ucdata)
{
  SPI_Data_Mode();
  CS_LOW();

  HAL_SPI_Transmit(&ST7735_SPI_PORT, &ucdata, sizeof(ucdata), HAL_MAX_DELAY);

  CS_HIGH();
}

static void NocHalLCD_SendCommand(uint8_t cmd)
{
  SPI_Cmd_Mode();
  CS_LOW();

  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);

  CS_HIGH();
}

/*static uint8_t NocHalLCD_ReadByte(uint8_t addr)
{
  uint8_t ucdata[2];

  SPI_Cmd_Mode();
  CS_LOW();

  HAL_SPI_Transmit(&ST7735_SPI_PORT, &addr, sizeof(addr), HAL_MAX_DELAY);
  HAL_SPI_Receive(&ST7735_SPI_PORT, ucdata, sizeof(ucdata), HAL_MAX_DELAY);

  CS_HIGH();

  return ucdata[1];  // ucdata[0] is dummy read
}*/

void NocHalLCD_DrawPixel(uint16_t color)
{
  SPI_Data_Mode();
  CS_LOW();

  uint8_t ucdata[] = {color >> 8, color & 0xFF};
  HAL_SPI_Transmit(&ST7735_SPI_PORT, ucdata, sizeof(ucdata), HAL_MAX_DELAY);

  CS_HIGH();
}

void NocHalLCD_Init(void /* struct */)
{
  /* 0. SW Reset
	   1. Init t_Default struct
	   2. Pass struct to this function
	   3. Implement HAL_SPI_Transmit/Receive to transfer config to ST7735
	*/

  CS_LOW();

  NocHalLCD_SendCommand(ST7735_SWRESET); //Software Reset
  HAL_Delay(150);
  NocHalLCD_SendCommand(ST7735_SLPOUT); //exit Sleep
  HAL_Delay(200);

  NocHalLCD_SendCommand(ST7735_FRMCTR1);  //Frame rate ctrl in normal/full color mode
  NocHalLCD_WriteByte(13);   // RTNA = 13
  NocHalLCD_WriteByte(10);   // Front porch
  NocHalLCD_WriteByte(18);   // Back porch

  NocHalLCD_SendCommand(ST7735_FRMCTR2);  //Frame rate ctrl in idle/8-bit mode
  NocHalLCD_WriteByte(13);   // RTNA = 13
  NocHalLCD_WriteByte(10);   // Front porch
  NocHalLCD_WriteByte(18);   // Back porch

  NocHalLCD_SendCommand(ST7735_FRMCTR3);  //Frame rate ctrl in Partial/full color mode
  NocHalLCD_WriteByte(13);   // RTNA = 13     // 1 to 3 are used in line inversion mode
  NocHalLCD_WriteByte(10);   // Front porch
  NocHalLCD_WriteByte(18);   // Back porch
  NocHalLCD_WriteByte(13);   // RTNA = 13     // 4 to 6 are used in frame inversion mode
  NocHalLCD_WriteByte(10);   // Front porch
  NocHalLCD_WriteByte(18);   // Back porch

  NocHalLCD_SendCommand(ST7735_MADCTL); // default
  NocHalLCD_WriteByte(0x60);

  NocHalLCD_SendCommand(ST7735_COLMOD); //16-bit color
  NocHalLCD_WriteByte(0x05);

  NocHalLCD_SendCommand(ST7735_CASET); //Column address set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x9F);

  NocHalLCD_SendCommand(ST7735_RASET); //Row address set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x7F);

  NocHalLCD_SendCommand(ST7735_NORON); //Normal display on
  HAL_Delay(100);

  NocHalLCD_SendCommand(ST7735_DISPON); //Display on
  HAL_Delay(150);

  CS_HIGH();

  NocHalLCD_SetOSDFont(Font5x7);	/* Select font */
}

void NocHalLCD_SetWindowAddr(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd)
{
  NocHalLCD_SendCommand(ST7735_CASET); // Column addr set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(xStart);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(xEnd);

  NocHalLCD_SendCommand(ST7735_RASET); // Row addr set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(yStart);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(yEnd);

  NocHalLCD_SendCommand(ST7735_RAMWR); // write to RAM
}

void NocHalLCD_ClrScreen(void)
{
  NocHalLCD_SetWindowAddr(0, 160, 0, 128);

  for(int i = 0; i < (160*128); i++)
  {
	  NocHalLCD_DrawPixel(0x00);
  }

  CS_HIGH();
}

void NocHalLCD_DisplayImage(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom, const uint16_t *image)
{
  int MaskT, MaskL, MaskR, MaskB;	/* Active drawing area */

  int yc, xc, xl, xs;
  uint16_t pd;

  MaskL = 0;
  MaskR = DISP_XS - 1;
  MaskT = 0;
  MaskB = DISP_YS - 1;

  if (left > right || top > bottom) return; 	/* Check varidity */
  if (left > MaskR || right < MaskL  || top > MaskB || bottom < MaskT) return;	/* Check if in active area */

  yc = bottom - top + 1;			/* Vertical size */
  xc = right - left + 1; xs = 0;	/* Horizontal size and skip */

	if (top < MaskT) {		/* Clip top of source image if it is out of active area */
		image += xc * (MaskT - top);
		yc -= MaskT - top;
		top = MaskT;
	}
	if (bottom > MaskB) {	/* Clip bottom of source image if it is out of active area */
		yc -= bottom - MaskB;
		bottom = MaskB;
	}
	if (left < MaskL) {		/* Clip left of source image if it is out of active area */
		image += MaskL - left;
		xc -= MaskL - left;
		xs += MaskL - left;
		left = MaskL;
	}
	if (right > MaskR) {	/* Clip right of source image it is out of active area */
		xc -= right - MaskR;
		xs += right - MaskR;
		right = MaskR;
	}

  CS_LOW();

  NocHalLCD_SetWindowAddr(left, right, top, bottom);

	do {	/* Send image data */
		xl = xc;
		do {
			pd = *image++;
			NocHalLCD_DrawPixel(pd);
		} while (--xl);
		image += xs;
	} while (--yc);

  CS_HIGH();
}

void NocHalLCD_SetOSDFont(const uint8_t *ucfont)
{
	if (!memcmp(ucfont, "FONTX2", 5))
	{
		FontS = ucfont;
	}

	/* Font size: height, dot width and byte width */
	font.height = ucfont[15];
	font.width = ucfont[14];
	font.size = ((font.width + 7) / 8) * font.height;
}

void NocHalLCD_WriteChar(uint16_t x, uint16_t y, uint8_t chr, uint32_t color) /* (bg << 16) + fg */
{
	const uint8_t *fnt;

	uint32_t i, b, j;

	uint8_t fontStartAdrr = 17;

	if ((fnt = FontS) == 0)
		return;	/* Exit if no font registerd */

	/* Exit if current position is out of screen */
	if (x >= DISP_XS || y >= DISP_YS)
		return;

	CS_LOW();	/* Select display module */
	NocHalLCD_SetWindowAddr(x, x + font.width - 1, y, y + font.height - 1);

	for(i = 0; i < font.height; i++)
	{
		b = fnt[(fontStartAdrr + chr * font.size) + i];	/* Start of bitmap + offset font height */
		for(j = 0; j < font.width; j++)
		{
			if((b << j) & 0x80)
			{
				NocHalLCD_DrawPixel(color);   		// Color = blank
			}
			else
			{
				NocHalLCD_DrawPixel(color >> 16);   // Put color
			}
		}
	}

	CS_HIGH();
}

void NocHalLCD_WriteString(uint16_t x, uint16_t y, const char* str, uint32_t color)
{
	x = x * font.width;
	y = y * font.height;

	CS_LOW();

	while(*str)
	{
		if(x + font.width >= DISP_XS)
		{
			x = 0;
			y += font.height;
			if(y + font.height >= DISP_YS)
			{
				break;
			}

			if(*str == ' ')
			{
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}

		NocHalLCD_WriteChar(x, y, *str, color);
		x += font.width;
		str++;
	}

	CS_HIGH();
}

/* Returns number of bytes read (zero on error) */
static unsigned int tjd_input (
	JDEC* jd,			 /* Decompressor object */
	uint8_t* buff,		 /* Pointer to the read buffer (null to remove data) */
	unsigned int nd		 /* Number of bytes to read/skip from input stream */
)
{
	UINT rb;
	FIL *fp = (FIL*)jd->device;

	if (buff)
	{
		/* Read nd bytes from the input stream */
		f_read(fp, buff, nd, &rb);
		return rb;	 /* Returns number of bytes could be read */
	}
	else
	{
		/* Skip nd bytes on the input stream */
		return (f_lseek(fp, f_tell(fp) + nd) == FR_OK) ? nd : 0;
	}
}

static int tjd_output (
	JDEC* jd,		 /* Decompressor object of current session */
	void* bitmap,	 /* Bitmap data to be output */
	JRECT* rect		 /* Rectangular region to output */
)
{
	UNUSED(jd);

	/* Check user interrupt at left end */
	//if (!rect->left) return 0;	 /* Abort decompression */

	/* Put the rectangular into the display device */
	NocHalLCD_DisplayImage(rect->left, rect->right, rect->top, rect->bottom, (uint16_t*)bitmap);

	return 1;	 /* Continue decompression */
}

void load_jpg (
	FIL* fp,		 /* Open file object to load */
	void *work,		 /* Pointer to the working buffer (must be 4-byte aligned) */
	UINT sz_work	 /* Size of the working buffer (must be power of 2) */
)
{
	JDEC jd;		 /* Decompression object (70 bytes) */
	JRESULT rc;
	BYTE scale;

	/* Prepare to decompress the file */
	rc = jd_prepare(&jd, tjd_input, work, sz_work, fp);
	if (rc == JDR_OK)
	{
		printf("jd_prepare -> Done!\r\n");
		/* Determine scale factor */
		for (scale = 0; scale < 3; scale++)
		{
			if ((jd.width >> scale) <= DISP_XS && (jd.height >> scale) <= DISP_YS)
				break;
		}

		/* Start to decompress the JPEG file */
		rc = jd_decomp(&jd, tjd_output, scale);	 /* Start decompression */
		printf("Decompression result: %d\r\n", rc);
	}
	else
	{
		printf("load_jpg Error: %d\r\n", rc);
	}
}
