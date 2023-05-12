/*******************************************************************************
  USART0 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_usart0.c

  Summary:
    USART0 PLIB Implementation File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#include "device.h"
#include "plib_usart0.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: USART0 Implementation
// *****************************************************************************
// *****************************************************************************

static void USART0_ErrorClear( void )
{
    uint32_t dummyData = 0U;

   if ((USART0_REGS->US_CSR & (US_CSR_USART_OVRE_Msk | US_CSR_USART_PARE_Msk | US_CSR_USART_FRAME_Msk)) != 0U)
   {
        /* Clear the error flags */
        USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;

        /* Flush existing error bytes from the RX FIFO */
        while ((USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) != 0U)
        {
            dummyData = USART0_REGS->US_RHR & US_RHR_RXCHR_Msk;
        }
   }

    /* Ignore the warning */
    (void)dummyData;
}


volatile static USART_OBJECT usart0Obj;

static void __attribute__((used)) USART0_ISR_RX_Handler( void )
{
    uint32_t rxData = 0U;
    uint8_t* pu8Data = (uint8_t *)usart0Obj.rxBuffer;
    uint16_t* pu16Data = (uint16_t*)usart0Obj.rxBuffer;

    if(usart0Obj.rxBusyStatus == true)
    {
        size_t rxSize = usart0Obj.rxSize;
        size_t rxProcessedSize = usart0Obj.rxProcessedSize;

        while(((USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) != 0U) && (rxSize > rxProcessedSize))
        {
            rxData = (USART0_REGS->US_RHR & US_RHR_RXCHR_Msk);
            if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
            {
                pu16Data[rxProcessedSize] = (uint16_t)rxData;
                rxProcessedSize++;
            }
            else
            {
                pu8Data[rxProcessedSize] = (uint8_t)rxData;
                rxProcessedSize++;
            }
        }

        usart0Obj.rxProcessedSize = rxProcessedSize;

        /* Check if the buffer is done */
        if(usart0Obj.rxProcessedSize >= rxSize)
        {
            usart0Obj.rxBusyStatus = false;

            /* Disable Read, Overrun, Parity and Framing error interrupts */
            USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);

            if(usart0Obj.rxCallback != NULL)
            {
                uintptr_t rxContext = usart0Obj.rxContext;

                usart0Obj.rxCallback(rxContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
    }
}

static void __attribute__((used)) USART0_ISR_TX_Handler( void )
{
    uint32_t txData = 0U;
    uint8_t* pu8Data = (uint8_t *)usart0Obj.txBuffer;
    uint16_t* pu16Data = (uint16_t*)usart0Obj.txBuffer;

    if(usart0Obj.txBusyStatus == true)
    {
        size_t txSize = usart0Obj.txSize;
        size_t txProcessedSize = usart0Obj.txProcessedSize;

        while(((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U) && (txSize > txProcessedSize))
        {
            if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
            {
                txData = pu16Data[txProcessedSize];
            }
            else
            {
                txData = pu8Data[txProcessedSize];
            }
            USART0_REGS->US_THR = txData & US_THR_TXCHR_Msk;
            txProcessedSize++;
        }

        usart0Obj.txProcessedSize = txProcessedSize;

        /* Check if the buffer is done */
        if(usart0Obj.txProcessedSize >= txSize)
        {
            usart0Obj.txBusyStatus = false;

            USART0_REGS->US_IDR = US_IDR_USART_TXRDY_Msk;

            if(usart0Obj.txCallback != NULL)
            {
                uintptr_t txContext = usart0Obj.txContext;

                usart0Obj.txCallback(txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
    }
}

void __attribute__((used)) USART0_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (USART0_REGS->US_CSR & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk | US_CSR_USART_PARE_Msk));

    if(errorStatus != 0U)
    {
        /* Save the error to be reported later */
        usart0Obj.errorStatus = (USART_ERROR)errorStatus;

        /* Clear error flags and flush the error data */
        USART0_ErrorClear();

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);

        usart0Obj.rxBusyStatus = false;

        /* USART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( usart0Obj.rxCallback != NULL )
        {
            uintptr_t rxContext = usart0Obj.rxContext;

            usart0Obj.rxCallback(rxContext);
        }
    }

    /* Receiver status */
    if ((USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) != 0U)
    {
        USART0_ISR_RX_Handler();
    }

    /* Transmitter status */
    if ( ((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U) && ((USART0_REGS->US_IMR & US_IMR_USART_TXRDY_Msk) != 0U) )
    {
        USART0_ISR_TX_Handler();
    }
}

void USART0_Initialize( void )
{
    /* Reset USART0 */
    USART0_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk);

    /* Enable USART0 */
    USART0_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);

    /* Configure USART0 mode */
    USART0_REGS->US_MR = (US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | US_MR_USART_OVER(0));

    /* Configure USART0 Baud Rate */
    USART0_REGS->US_BRGR = US_BRGR_CD(81U);

    /* Initialize instance object */
    usart0Obj.rxBuffer = NULL;
    usart0Obj.rxSize = 0;
    usart0Obj.rxProcessedSize = 0;
    usart0Obj.rxBusyStatus = false;
    usart0Obj.rxCallback = NULL;
    usart0Obj.txBuffer = NULL;
    usart0Obj.txSize = 0;
    usart0Obj.txProcessedSize = 0;
    usart0Obj.txBusyStatus = false;
    usart0Obj.txCallback = NULL;
    usart0Obj.errorStatus = USART_ERROR_NONE;
}

USART_ERROR USART0_ErrorGet( void )
{
    USART_ERROR errors = usart0Obj.errorStatus;

    usart0Obj.errorStatus = USART_ERROR_NONE;

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool USART0_SerialSetup( USART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    uint32_t baud;
    uint32_t brgVal = 0;
    uint32_t overSampVal = 0;
    uint32_t usartMode;
    bool status = false;

    if(usart0Obj.rxBusyStatus == true)
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }
    if(usart0Obj.txBusyStatus == true)
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }

    if (setup != NULL)
    {
        baud = setup->baudRate;

        if(srcClkFreq == 0U)
        {
            srcClkFreq = USART0_FrequencyGet();
        }

        /* Calculate BRG value */
        if (srcClkFreq >= (16U * baud))
        {
            brgVal = (srcClkFreq / (16U * baud));
        }
        else if (srcClkFreq >= (8U * baud))
        {
            brgVal = (srcClkFreq / (8U * baud));
            overSampVal = US_MR_USART_OVER(1U);
        }
        else
        {
            return false;
        }

        if (brgVal > 65535U)
        {
            /* The requested baud is so low that the ratio of srcClkFreq to baud exceeds the 16-bit register value of CD register */
            return false;
        }

        /* Configure USART0 mode */
        usartMode = USART0_REGS->US_MR;
        usartMode &= ~(US_MR_USART_CHRL_Msk | US_MR_USART_MODE9_Msk | US_MR_USART_PAR_Msk | US_MR_USART_NBSTOP_Msk | US_MR_USART_OVER_Msk);
        USART0_REGS->US_MR = usartMode | ((uint32_t)setup->dataWidth | (uint32_t)setup->parity | (uint32_t)setup->stopBits | (uint32_t)overSampVal);

        /* Configure USART0 Baud Rate */
        USART0_REGS->US_BRGR = US_BRGR_CD(brgVal);
        status = true;
    }

    return status;
}

bool USART0_Read( void *buffer, const size_t size )
{
    bool status = false;
    if(buffer != NULL)
    {
        /* Check if receive request is in progress */
        if(usart0Obj.rxBusyStatus == false)
        {
            /* Clear errors that may have got generated when there was no active read request pending */
            USART0_ErrorClear();

            /* Clear the errors related to previous read requests */
            usart0Obj.errorStatus = USART_ERROR_NONE;

            usart0Obj.rxBuffer = buffer;
            usart0Obj.rxSize = size;
            usart0Obj.rxProcessedSize = 0;
            usart0Obj.rxBusyStatus = true;

            status = true;

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            USART0_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk);
        }
    }

    return status;
}

bool USART0_Write( void *buffer, const size_t size )
{
    bool status = false;
    if(NULL != buffer)
    {
        uint8_t* pu8Data = (uint8_t *)buffer;
        uint16_t* pu16Data = (uint16_t*)buffer;
        uint32_t txData = 0U;
        /* Check if transmit request is in progress */
        if(usart0Obj.txBusyStatus == false)
        {
            usart0Obj.txBuffer = buffer;
            usart0Obj.txSize = size;
            usart0Obj.txProcessedSize = 0;
            usart0Obj.txBusyStatus = true;
            status = true;


            size_t txSize = usart0Obj.txSize;
            size_t txProcessedSize = usart0Obj.txProcessedSize;

            /* Initiate the transfer by writing as many bytes as possible */
            while (((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U) && (txProcessedSize < txSize))
            {
                if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
                {
                    txData = pu16Data[txProcessedSize];
                }
                else
                {
                    txData = pu8Data[txProcessedSize];
                }
                USART0_REGS->US_THR = (txData & US_THR_TXCHR_Msk);
                txProcessedSize++;
            }

            usart0Obj.txProcessedSize = txProcessedSize;

            USART0_REGS->US_IER = US_IER_USART_TXRDY_Msk;
        }
    }
    return status;
}


bool USART0_TransmitComplete( void )
{
    return((USART0_REGS->US_CSR & US_CSR_USART_TXEMPTY_Msk) != 0U);

}

void USART0_WriteCallbackRegister( USART_CALLBACK callback, uintptr_t context )
{
    usart0Obj.txCallback = callback;
    usart0Obj.txContext = context;
}

void USART0_ReadCallbackRegister( USART_CALLBACK callback, uintptr_t context )
{
    usart0Obj.rxCallback = callback;
    usart0Obj.rxContext = context;
}

bool USART0_WriteIsBusy( void )
{
    return usart0Obj.txBusyStatus;
}

bool USART0_ReadIsBusy( void )
{
    return usart0Obj.rxBusyStatus;
}

bool USART0_ReadAbort(void)
{
    if (usart0Obj.rxBusyStatus == true)
    {
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);

        usart0Obj.rxBusyStatus = false;

        /* If required, application should read the num bytes processed prior to calling the read abort API */
        usart0Obj.rxProcessedSize = 0U;
        usart0Obj.rxSize = 0U;
    }

    return true;
}

size_t USART0_WriteCountGet( void )
{
    return usart0Obj.txProcessedSize;
}

size_t USART0_ReadCountGet( void )
{
    return usart0Obj.rxProcessedSize;
}

