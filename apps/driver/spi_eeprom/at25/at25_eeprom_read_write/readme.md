[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SPI EEPROM AT25 driver - EEPROM Read Write

This example application shows how to use the AT25 driver to perform read and write operations on AT25 series of EEPROM.

## Description

This example uses the AT25 driver to communicate with the SPI based AT25 series of external EEPROMs to perform write and read operations in Bare-Metal environment.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_e70_s70_v70_v71) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/driver/spi_eeprom/at25/at25_eeprom_read_write/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X | MPLABX project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
| sam_e70_xult_freertos.X | MPLABX project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
| sam_v71_xult.X | MPLABX project for [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
| sam_v71_xult_freertos.X | MPLABX project for [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| sam_v71_xult.X <br> sam_v71_xult_freertos.X | [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
| sam_e70_xult.X <br> sam_e70_xult_freertos.X | [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
|||

### Setting up [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- To run the demo, the following additional hardware are required:
  - [EEPROM 4 click board](https://www.mikroe.com/eeprom-4-click)

- Install [EEPROM 4 click board](https://www.mikroe.com/eeprom-4-click) to the click slot provided on board
- Connect a jumper between LED2 and PB08 on J203 header
- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- To run the demo, the following additional hardware are required:
  - [EEPROM 4 click board](https://www.mikroe.com/eeprom-4-click)
  - [mikroBUS Xplained Pro board](https://www.microchip.com/developmenttools/ProductDetails/ATMBUSADAPTER-XPRO)

- Install an [EEPROM 4 click board](https://www.mikroe.com/eeprom-4-click) on to the [mikroBUS Xplained Pro board](https://www.microchip.com/developmenttools/ProductDetails/ATMBUSADAPTER-XPRO)
- Connect [mikroBUS Xplained Pro board](https://www.microchip.com/developmenttools/ProductDetails/ATMBUSADAPTER-XPRO) to the EXT1 header
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. The LED is turned ON when the value read from EEPROM matches with the written data

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) | LED1 |
|  [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) | LED0 |
|||
