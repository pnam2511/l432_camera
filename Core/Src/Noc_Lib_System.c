#include "Noc_Lib_System.h"
#include "main.h"

#define NOC_LIB_SYSTEM

nocSYSSTATUS NocLibSys_Init(void)
{
  FATFS fs;

  // unselect all SPI devices first
  SDCARD_Unselect();
  LCD_UnSelect();

  // initialize SD-card as fast as possible, it glitches otherwise
  // (this is important only if SPI bus is shared by multiple devices)
  int status = SDCARD_Init();
  if(status < 0) {
      printf("SDCARD_Init() failed, status = %d\r\n", status);
      return SDCARD_INIT_ERR;
  }
  printf("SDCard_Init() done!\r\n");

  NocHalLCD_Init();
  NocHalLCD_ClrScreen();
  NocHalLCD_WriteString(0, 0, "LCD_Init() done!", C_WHITE);

  // mount the default drive
  FRESULT res = f_mount(&fs, "/", 0);
  if(res != FR_OK) {
      printf("f_mount() failed, res = %d\r\n", res);
      return F_MOUNT_ERR;
  }
  printf("f_mount() done!\r\n");

  return SYS_OK;
}

/* Modified version of scan_files in fafts/ff.c */
void NocLibSys_ShowDirectory(char *path)
{
  FRESULT res;
  DIR dir;
  static FILINFO finfo;
  UINT i = 1;
  char str[128];

  NocHalLCD_WriteString(0, 0, "Opening directory", C_WHITE);

  res = f_opendir(&dir, path);                      /* Open directory */
  if(res == FR_OK)
  {
    while(1)
    {
      res = f_readdir(&dir, &finfo);                 /* Read a directory item */
      if((res != FR_OK) || (finfo.fname[0] == 0))   /* Break on error or end of directory */
        break;
      
      sprintf(str, "%c%c%c%c %s/%s",
        ((finfo.fattrib & AM_DIR) ? 'D' : '-'),
        ((finfo.fattrib & AM_RDO) ? 'R' : '-'),
        ((finfo.fattrib & AM_SYS) ? 'S' : '-'),
        ((finfo.fattrib & AM_HID) ? 'H' : '-'),
        path, finfo.fname);

      NocHalLCD_WriteString(0, i++, str, C_WHITE);
    }
  }
}
