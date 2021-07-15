/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#if SYS_FS_AUTOMOUNT_ENABLE
    #define APP_MOUNT_NAME          SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
    #define APP_DEVICE_NAME         SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0
#else
    #define APP_MOUNT_NAME          "/mnt/myDrive1"
    #define APP_DEVICE_NAME         "/dev/rama1"
    #define APP_FS_TYPE             FAT
#endif

#define APP_FILE_NAME               "newfile.txt"

#define KEEP                        __attribute__((keep, address(DRV_MEMORY_DEVICE_START_ADDRESS)))

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA CACHE_ALIGN appData;

/* Work buffer used by FAT FS during Format */
uint8_t CACHE_ALIGN work[SYS_FS_FAT_MAX_SS];

SYS_FS_FORMAT_PARAM formatOpt;

/* Create an array for reserving region in RAM for Fat File system */
char CACHE_ALIGN RAM_FAT_MEDIA[DRV_MEMORY_DEVICE_MEDIA_SIZE_BYTES] KEEP;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
#if SYS_FS_AUTOMOUNT_ENABLE

void APP_SysFSEventHandler(SYS_FS_EVENT event,void* eventData,uintptr_t context)
{
    switch(event)
    {
        /* If the event is mount then check which media has been mounted */
        case SYS_FS_EVENT_MOUNT:
            if(strcmp((const char *)eventData, APP_MOUNT_NAME) == 0)
            {
                appData.diskMounted = true;
            }

            break;

        case SYS_FS_EVENT_MOUNT_WITH_NO_FILESYSTEM:
        {
            if(strcmp((const char *)eventData, APP_MOUNT_NAME) == 0)
            {
                appData.diskFormatRequired = true;
            }
            break;
        }

        /* If the event is unmount then check which media has been unmount */
        case SYS_FS_EVENT_UNMOUNT:
            if(strcmp((const char *)eventData, APP_MOUNT_NAME) == 0)
            {
                appData.diskMounted = false;
                appData.diskFormatRequired = false;

                if (appData.state == APP_IDLE)
                {
                    appData.state = APP_MOUNT_WAIT;
                }
                else
                {
                    appData.state = APP_ERROR;
                }
                LED_OFF();
            }

            break;

        case SYS_FS_EVENT_ERROR:
        default:
            break;
    }
}
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    uint32_t i;

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        appData.writeBuffer[i] = i;
    }

    /* Initialize the app state to wait for media attach. */
#if SYS_FS_AUTOMOUNT_ENABLE
    appData.state = APP_MOUNT_WAIT;

    SYS_FS_EventHandlerSet(APP_SysFSEventHandler,(uintptr_t)NULL);
#else
    appData.state = APP_MOUNT_DISK;
#endif
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
#if SYS_FS_AUTOMOUNT_ENABLE
        case APP_MOUNT_WAIT:
        {
            if (appData.diskFormatRequired == true)
            {
                /* Mount was successful. Format the disk. */
                appData.state = APP_FORMAT_DISK;
            }
            else if (appData.diskMounted == true)
            {
                /* Mount was successful. Open the file. */
                appData.state = APP_OPEN_FILE;
            }

            break;
        }
#else
        case APP_MOUNT_DISK:
        {
            /* Mount the disk */
            if(SYS_FS_Mount(APP_DEVICE_NAME, APP_MOUNT_NAME, APP_FS_TYPE, 0, NULL) != 0)
            {
                /* The disk could not be mounted. Try mounting again until
                 * the operation succeeds. */
                appData.state = APP_MOUNT_DISK;
            }
            else
            {
                /* Check If Mount was successful with no file system on media */
                if (SYS_FS_Error() == SYS_FS_ERROR_NO_FILESYSTEM)
                {
                    /* Format the disk. */
                    appData.state = APP_FORMAT_DISK;
                }
                else
                {
                    appData.state = APP_OPEN_FILE;
                }
            }

            break;
        }
#endif

        case APP_FORMAT_DISK:
        {
            formatOpt.fmt = SYS_FS_FORMAT_FAT;
            formatOpt.au_size = 0;

            if (SYS_FS_DriveFormat (APP_MOUNT_NAME, &formatOpt, (void *)work, SYS_FS_FAT_MAX_SS) != SYS_FS_RES_SUCCESS)
            {
                /* Format of the disk failed. */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Format succeeded. Open a file. */
                appData.state = APP_OPEN_FILE;
            }
            break;
        }

        case APP_OPEN_FILE:
        {
            appData.fileHandle = SYS_FS_FileOpen(APP_MOUNT_NAME"/"APP_FILE_NAME, (SYS_FS_FILE_OPEN_WRITE_PLUS));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* File open unsuccessful */
                appData.state = APP_ERROR;
            }
            else
            {
                /* File open was successful. Write to the file. */
                appData.state = APP_WRITE_TO_FILE;
            }
            break;
        }

        case APP_WRITE_TO_FILE:
        {
            if(SYS_FS_FileWrite (appData.fileHandle, (void *)appData.writeBuffer, BUFFER_SIZE) == -1)
            {
                /* Failed to write to the file. */
                appData.state = APP_ERROR;
            }
            else
            {
                /* File write was successful. */
                appData.state = APP_FLUSH_FILE;
            }
            break;
        }

        case APP_FLUSH_FILE:
        {
            if (SYS_FS_FileSync(appData.fileHandle) != 0)
            {
                /* Could not flush the contents of the file. Error out. */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Check the file status */
                appData.state = APP_READ_FILE_STAT;
            }
            break;
        }

        case APP_READ_FILE_STAT:
        {
            if(SYS_FS_FileStat(APP_MOUNT_NAME"/"APP_FILE_NAME, &appData.fileStatus) == SYS_FS_RES_FAILURE)
            {
                /* Reading file status was a failure */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Read file size */
                appData.state = APP_READ_FILE_SIZE;
            }
            break;
        }

        case APP_READ_FILE_SIZE:
        {
            appData.fileSize = SYS_FS_FileSize(appData.fileHandle);
            if(appData.fileSize == -1)
            {
                /* Reading file size was a failure */
                appData.state = APP_ERROR;
            }
            else
            {
                if(appData.fileSize == appData.fileStatus.fsize)
                {
                    appData.state = APP_DO_FILE_SEEK;
                }
                else
                {
                    appData.state = APP_ERROR;
                }
            }
            break;
        }

        case APP_DO_FILE_SEEK:
        {
            if(SYS_FS_FileSeek( appData.fileHandle, appData.fileSize, SYS_FS_SEEK_SET ) == -1)
            {
                /* File seek caused an error */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Check for End of file */
                appData.state = APP_CHECK_EOF;
            }
            break;
        }

        case APP_CHECK_EOF:
        {
            if(SYS_FS_FileEOF( appData.fileHandle ) == false )
            {
                /* Either, EOF is not reached or there was an error
                   In any case, for the application, its an error condition */
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_DO_ANOTHER_FILE_SEEK;
            }
            break;
        }

        case APP_DO_ANOTHER_FILE_SEEK:
        {
            /* Move file pointer to begining of file */
            if(SYS_FS_FileSeek( appData.fileHandle, 0, SYS_FS_SEEK_SET ) == -1)
            {
                /* File seek caused an error */
                appData.state = APP_ERROR;
            }
            else
            {
                /* Check for original file content */
                appData.state = APP_READ_FILE_CONTENT;
            }
            break;
        }

        case APP_READ_FILE_CONTENT:
        {
            if(SYS_FS_FileRead(appData.fileHandle, (void *)appData.readBuffer, appData.fileSize) == -1)
            {
                /* There was an error while reading the file. Close the file
                 * and error out. */
                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_ERROR;
            }
            else
            {
                if ((appData.fileSize != BUFFER_SIZE) || (memcmp(appData.readBuffer, appData.writeBuffer, BUFFER_SIZE) != 0))
                {
                    /* The written and the read data dont match. */
                    appData.state = APP_ERROR;
                }
                else
                {
                    /* The test was successful. */
                    appData.state = APP_CLOSE_FILE;
                }
            }
            break;
        }

        case APP_CLOSE_FILE:
        {
            /* Close the file */
            if (SYS_FS_FileClose(appData.fileHandle) != 0)
            {
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_UNMOUNT_DISK;
            }
            break;
        }

        case APP_UNMOUNT_DISK:
        {
            /* Unmount the disk */
            if (SYS_FS_Unmount(APP_MOUNT_NAME) != 0)
            {
                appData.state = APP_ERROR;
            }
            else
            {
                appData.state = APP_IDLE;
            }
            break;
        }

        case APP_IDLE:
        {
            /* The application comes here when the demo has completed
             * successfully. Glow LED. */
            LED_ON();

            break;
        }

        case APP_ERROR:
        {
            /* The application comes here when the demo has failed.*/
            break;
        }

        default:
        {
            break;
        }
    }

} //End of APP_Tasks
