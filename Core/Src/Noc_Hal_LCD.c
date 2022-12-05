#include "Noc_Hal_LCD.h"
#include "main.h"

#define NOC_HAL_LCD

#define ST7735_SPI_PORT hspi3

static int LocX, LocY;
static uint32_t ChrColor;		/* Current character color ((bg << 16) + fg) */
static const uint8_t *FontS;	/* Current font (ANK) */

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
  NocHalLCD_WriteByte(0x00);

  NocHalLCD_SendCommand(ST7735_COLMOD); //16-bit color
  NocHalLCD_WriteByte(0x05);

  NocHalLCD_SendCommand(ST7735_CASET); //Column address set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x7F);

  NocHalLCD_SendCommand(ST7735_RASET); //Row address set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(0x9F);

  NocHalLCD_SendCommand(ST7735_NORON); //Normal display on
  HAL_Delay(100);

  NocHalLCD_SendCommand(ST7735_DISPON); //Display on
  HAL_Delay(150);

  CS_HIGH();

  disp_font_face(Font5x7);	/* Select font */
  disp_font_color(C_WHITE);
}

void NocHalLCD_SetWindowAddr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  NocHalLCD_SendCommand(ST7735_CASET); // Column addr set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(x0);     // XSTART
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(x1);     // XEND

  NocHalLCD_SendCommand(ST7735_RASET); // Row addr set
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(y0);     // YSTART
  NocHalLCD_WriteByte(0x00);
  NocHalLCD_WriteByte(y1);     // YEND

  NocHalLCD_SendCommand(ST7735_RAMWR); // write to RAM
}

void NocHalLCD_ClrScreen(void)
{
  NocHalLCD_SetWindowAddr(0, 0, 128, 160);
  for(int i = 0; i < (160*128*BYTES_PER_PIXEL); i++)
  {
	NocHalLCD_WriteByte(0x00);
  }
  CS_HIGH();
}

void NocHalLCD_DisplayImage(const uint16_t *image)
{
  CS_LOW();
  NocHalLCD_SetWindowAddr(0, 0, 128, 160);
  for(int i = 0; i < (160*128*BYTES_PER_PIXEL); i++)
  {
	NocHalLCD_WriteByte(image[i]);
  }
  CS_HIGH();
}

void disp_locate (
	int col,	/* Column position */
	int row		/* Row position */
)
{
	if (FontS) {	/* Pixel position is calcurated with size of single byte font */
		LocX = col * FontS[14];
		LocY = row * FontS[15];
	}
}

void disp_font_face (
	const uint8_t *font	/* Pointer to the font structure in FONTX2 format */
)
{
	if (!memcmp(font, "FONTX2", 5)) {
		FontS = font;
	}
}

void disp_font_color (
	uint32_t color	/* (bg << 16) + fg */
)
{
	ChrColor = color;
}

void disp_putc (
	uint8_t chr		/* Character to be output (kanji chars are given in two disp_putc sequence) */
)
{
	const uint8_t *fnt;
	uint8_t b, d;
	uint16_t dchr;
	uint32_t col;
	int h, wc, w, wb, i, fofs;


	if ((fnt = FontS) == 0) return;	/* Exit if no font registerd */

	if (chr < 0x20) {	/* Processes the control character */
		switch (chr) {
		case '\n':	/* LF */
			LocY += fnt[15];
			/* follow next case */
		case '\r':	/* CR */
			LocX = 0;
			return;
		case '\b':	/* BS */
			LocX -= fnt[14];
			if (LocX < 0) LocX = 0;
			return;
		case '\f':	/* FF */
			NocHalLCD_ClrScreen();
			LocX = LocY = 0;
			return;
		}
	}

	/* Exit if current position is out of screen */
	if ((unsigned int)LocX >= DISP_XS || (unsigned int)LocY >= DISP_YS) return;

		dchr = chr;
		fofs = 17;		/* Font area start address */

	h = fnt[15]; w = fnt[14]; wb = (w + 7) / 8;	/* Font size: height, dot width and byte width */
	fnt += fofs + dchr * wb * h;				/* Goto start of the bitmap */

	if (LocX + w > DISP_XS) w = DISP_XS - LocX;	/* Clip right of font face at right edge */
	if (LocY + h > DISP_YS) h = DISP_YS - LocY;	/* Clip bottom of font face at bottom edge */

	CS_LOW();	/* Select display module */
	NocHalLCD_SetWindowAddr(LocX, LocY, LocX + w - 1, LocY + h - 1);
	d = 0;
	do {
		wc = w; b = i = 0;
		do {
			if (!b) {		/* Get next 8 dots */
				b = 0x80;
				d = fnt[i++];
			}
			col = ChrColor;
			if (!(b & d)) col >>= 16;	/* Select color, BG or FG */
			b >>= 1;		/* Next bit */
			NocHalLCD_WriteByte(col);	/* Put the color */
		} while (--wc);
		fnt += wb;		/* Next raster */
	} while (--h);
	CS_HIGH();	/* Deselect display module */

	LocX += w;	/* Update current position */
}

/*static unsigned int tjd_input (	 Returns number of bytes read (zero on error)
	JDEC* jd,			 Decompressor object
	uint8_t* buff,		 Pointer to the read buffer (null to remove data)
	unsigned int nd		 Number of bytes to read/skip from input stream
)
{
	UINT rb;
	FIL *fp = (FIL*)jd->device;

	if (buff)
	{	 Read nd bytes from the input strem
		f_read(fp, buff, nd, &rb);
		return rb;	 Returns number of bytes could be read
	}
	else
	{	 Skip nd bytes on the input stream
		return (f_lseek(fp, f_tell(fp) + nd) == FR_OK) ? nd : 0;
	}
}

static int tjd_output (
	JDEC* jd,		 Decompressor object of current session
	void* bitmap,	 Bitmap data to be output
	JRECT* rect		 Rectangular region to output
)
{
	jd = jd;	 Suppress warning (device identifier is not needed in this appication)

	 Check user interrupt at left end
	if (!rect->left) return 0;	 Abort decompression

	 Put the rectangular into the display device
	NocHalLCD_DisplayImage((uint16_t*)bitmap);

	return 1;	 Continue decompression
}

void load_jpg (
	FIL* fp,		 Open file object to load
	void *work,		 Pointer to the working buffer (must be 4-byte aligned)
	UINT sz_work	 Size of the working buffer (must be power of 2)
)
{
	JDEC jd;		 Decompression object (70 bytes)
	JRESULT rc;
	BYTE scale;

	 Prepare to decompress the file
	rc = jd_prepare(&jd, tjd_input, work, sz_work, fp);
	if (rc == JDR_OK)
	{
		printf("jd_prepare -> Done!\r\n");
		 Determine scale factor
		for (scale = 0; scale < 3; scale++)
		{
			if ((jd.width >> scale) <= DISP_XS && (jd.height >> scale) <= DISP_YS)
				break;
		}

		 Start to decompress the JPEG file
		rc = jd_decomp(&jd, tjd_output, scale);	 Start decompression
		printf("Decompression result: %d\r\n", rc);
	}
	else
	{
		printf("Error: %d", rc);
	}
}*/
