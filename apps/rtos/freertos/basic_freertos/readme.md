[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# FreeRTOS basic

This example application blinks an LED to show the FreeRTOS threads that are running and to indicate status.

## Description

This demonstration creates three tasks and a queue. Task1 sends message to Task2 and Task3 to unblock and toggle an LED. Task1 priority is low compared to Task2 and Task3 which shares same priority.

- **Task1**: This task sends the data (i.e. delay, the amount of time for which the task need blocked) to Task2 and Task3 using a queue, then Task1 blocks itself for 200ms to allow other tasks to schedule and run
- **Task2**: This task blocks until it receives the data from queue, if the Task2 receives the expected delay (i.e. 1000ms) from Task1 then toggles the LED and blocks itself for the amount of delay received
- **Task3**: This task blocks until it receives the data from queue, if the Task3 receives the expected delay (i.e. 100ms) from Task1 then toggles the LED and blocks itself for the amount of delay received

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/core_apps_sam_e70_s70_v70_v71) and then click Clone button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/rtos/freertos/basic_freertos/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X | MPLABX project for [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
| sam_v71_xult.X | MPLABX project for [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| sam_v71_xult.X | [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
| sam_e70_xult.X | [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) |
|||

### Setting up [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

2. The LED indicates the success or failure.
    - The LED toggles on success i.e. each time when the Task2 or Task3 receives a valid message

Refer to the following table for LED name:

| Board | LED Name |
| ----- | -------- |
|  [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) | LED1 |
|  [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) | LED0 |
|||