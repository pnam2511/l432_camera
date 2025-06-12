Build: make -j8 all

Pin configuration:
ST7735:
CLK <--> PB3 (SPI3_SCK)
SDA <--> PB5 (SPI3_MOSI)
RS  <--> PB1 (gpio) (data/command selection)
RST <--> NRST
CS  <--> PB6 (gpio) (select ST7735 or SDCard)

SDCard:
SD_CS   <--> PA0 (gpio)
SD_MOSI <--> PB5 (SPI3_MOSI)
SD_SCK  <--> PB3 (SPI3_SCK)
SD_MISO <--> PB4 (SPI3_MISO)
