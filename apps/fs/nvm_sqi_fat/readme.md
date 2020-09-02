---
parent: Harmony 3 driver and system service application examples for SAM E70/S70/V70/V71 family
title: FAT filesystem using NVM and SST26 Media 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# FAT filesystem using NVM and SST26 Media

This application shows an example of using the FAT File System to access multiple files across multiple media (NVM, SQI FLASH).

## Description

### File System Operations on NVM:

- Application places a FAT disk image consisting of a Master Boot Record (MBR) sector, Logical Boot Sector, File allocation Table, and Root Directory Area in the internal Flash memory (NVM)

- During execution, the application first opens an existing file named **FILE.TXT** and performs following file system related operations:
  - **SYS_FS_FileStat**
  - **SYS_FS_FileSize**
  - **SYS_FS_FileSeek**
  - **SYS_FS_FileEOF**

- Finally, the string **"Hello World"** is written to this file. The string is then read and compared with the string that was written to the file

### File system layer uses:

- One instance of the Memory driver is used to communicate with underlying NVM media
- One instance of the Memory driver is used to communicate with the On-Board SQI Flash memory

### The Application Consists of three tasks which are called in RTOS Thread Context:

1. **APP_SST26_Tasks():**
    - Performs File Operations on SQI Flash memory
2. **APP_NVM_Tasks():**
    - Performs File Operations on NVM
3. **APP_MONITOR_Tasks():**
    - Monitors the state of above two Tasks

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_e70_s70_v70_v71) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/fs/nvm_sqi_fat/firmware** .

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
2. The LED toggles when the following criteria is satisfied.
    - **For NVM media:**
        - File **"FILE.TXT"** has the app data **"Hello World"** written in it
    - **For SST26 media:**
        - 4KB of data has successfully been written and read back from file **"newfile.txt"**

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) | LED1 |
|||
