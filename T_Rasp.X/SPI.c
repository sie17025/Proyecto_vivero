/*
 * File:   SPI_Lib.c
 * Author: Andres Sierra
 *
 * Created on 11 de marzo de 2020, 03:35 PM
 */


#include <stdint.h>
#include <xc.h>

#include "header_tr.h"


void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    //TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode
    {
        SSPSTAT = sTransmitEdge;
    //    TRISC3 = 1;
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;
    //    TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSPBUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
    spiReceiveWait();        // wait until the all bits receive
    return(SSPBUF); // read the received data from the buffer
}
