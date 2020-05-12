/*
 * File:   USART_Lib.c
 * Author: Andres Sierra
 *
 * Created on 12 de marzo de 2020, 12:27 AM
 */


#include <xc.h>

#include <stdint.h>

#include <pic16f887.h>

#include "header_pro.h"
//******************************************************************************

//Inicialización de UART

//******************************************************************************

char UART_Init(const long int baudrate)
{
  unsigned int x;
  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64); //SPBRG for Low Baud Rate
  if(x>255) //If High Baud Rate required
  {
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
    BRGH = 1; //Setting High Baud Rate
  }
  if(x<256)
  {
    SPBRG = x; //Writing SPBRG register
    SYNC = 0; //Selecting Asynchronous Mode
    SPEN = 1; //Enables Serial Port
    TRISC7 = 1;  
    TRISC6 = 1; 
    CREN = 1; //Enables Continuous Reception
    TXEN = 1; //Enables Transmission
    return 1;
  }
  return 0;
}

//******************************************************************************

//Función para leer un caracter por UART

//******************************************************************************

char UART_Read()
{
  while(!RCIF); //Waits for Reception to complete
  return RCREG; //Returns the 8 bit data
}

//******************************************************************************

//Función para leer un string por UART

//******************************************************************************

void UART_Read_Text(char *Output, unsigned int length)
{
  int i;
  for(int i=0;i<length;i++)
    Output[i] = UART_Read();
}

//******************************************************************************

//Función para mandar un caracter por UART

//******************************************************************************

void UART_Write(char data)
{
  while(!TRMT); //Waiting for Previous Data to Transmit completly
  TXREG = data; //Writing data to Transmit Register, Starts transmission
}

//******************************************************************************

//Función para mandar un string por UART

//******************************************************************************

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\\0';i++)
    UART_Write(text[i]);
}



char UART_TX_Empty()
{
  return TRMT; //Returns Transmit Shift Status bit
}


char UART_Data_Ready()
{
  return RCIF;
}