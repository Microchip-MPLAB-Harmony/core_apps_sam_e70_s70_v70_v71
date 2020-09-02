---
parent: Harmony 3 driver and system service application examples for SAM E70/S70/V70/V71 family
title: Memory driver synchronous - NVM SST26 Read Write 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Memory driver synchronous - NVM SST26 Read Write

This example application shows how to use the Memory driver in synchronous mode to perform block operations on the NVM and the SST26 media's.

## Description

### This application:

- Uses multi instances of the Memory driver to communicate with the NVM and the SST26 Flash memories in synchronous mode of operation in RTOS Environment.

- Performs block Erase/Write/Read operations on both the media's

- Consists of three tasks which are called in the RTOS thread context:
    1. **APP_SST26_Tasks():**
        - Performs operations on the SST26 QSPI/SQI Flash Memory
    2. **APP_NVM_Tasks():**
        - Performs operations on the NVM
    3. **APP_MONITOR_Tasks():**
        - Monitors the state of above two Tasks

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_e70_s70_v70_v71) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/driver/memory/sync/nvm_sst26_read_write/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult_freertos.X | MPLABX project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| sam_e70_xult_freertos.X | [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
|||

### Setting up [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using its IDE
2. The LED blinks when the data read from each media matches with the data written in them

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) | LED1 |
|||
