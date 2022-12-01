#include "Noc_Hal_LCD.h"
#include "main.h"

#define ST7735_SPI_PORT hspi3
#define NOC_HAL_LCD

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

void NocHalLCD_DisplayImage(const uint8_t *image)
{
  NocHalLCD_SetWindowAddr(0, 0, 128, 160);
  for(int i = 0; i < (160*128*BYTES_PER_PIXEL); i++)
  {
	NocHalLCD_WriteByte(image[i]);
  }
  CS_HIGH();
}
