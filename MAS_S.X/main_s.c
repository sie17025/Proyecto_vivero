/*
 * File:   Main_L4.c
 * Author: Andres Sierra
 * 
 * Codigo extraido de:
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 *
 * Created on 12 de febrero de 2020, 11:19 AM
 */

#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000

#include "header_mas.h"



//*****************************************************************************
// Código Principal
//*****************************************************************************
int prox, temp, luz, hum, caudal;

void setup(void);

void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       
       spiWrite(0);
       prox = spiRead();
       __delay_ms(5);
       spiWrite(1);
       temp = spiRead();
       __delay_ms(5);
       spiWrite(2);
       caudal = spiRead();
       __delay_ms(5);       
       spiWrite(3);
       luz = spiRead();
       __delay_ms(5);      
       spiWrite(4);
       hum = spiRead();
       __delay_ms(5);
      
       
//        writeFloat(int_pot, dec1, 1);       //escribe el entero y los decimales 
        setCursorLCD(2, 1);
        writeStrLCD("   ");
        setCursorLCD(2, 1);
        writeIntLCD(prox);
        setCursorLCD(2, 9);
        writeStrLCD("   ");
        setCursorLCD(2, 9);                 // selecciona la posicion para escribir el sigiente valor
        writeIntLCD(temp);                  // escribe el valor del contador
////        writeCharLCD(' ');                  
        setCursorLCD(2, 14);
        writeStrLCD("   ");
        setCursorLCD(2, 14);             // Selecciona la posicion final para escribir 
        writeIntLCD(luz);                   // escribe el valor de la fotoresistencia
        setCursorLCD(2, 4);  
        writeStrLCD("   ");
        setCursorLCD(2, 4);              // Selecciona la posicion final para escribir 
        writeIntLCD(hum);
        //setCursorLCD(1, 4);
        //writeStrLCD("   ");
        //setCursorLCD(1, 4);              // Selecciona la posicion final para escribir 
        //writeIntLCD(caudal);
//        writeStrLCD("  ");
       
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************

void setup(void){
    ANSEL = 0;
    ANSELH = 0;

    TRISB = 0;
    TRISC = 0b00010000;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    //PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
  
    initLCD();
    clcLCD();
    
    //Write first row that won't be modified
    setCursorLCD(1, 1);
    writeStrLCD("P");
    setCursorLCD(1, 3);
    writeStrLCD("TEM");
    setCursorLCD(1, 7);
    writeStrLCD("HU");
    setCursorLCD(1, 10);
    writeStrLCD("SOL");
    setCursorLCD(1, 14);
    writeStrLCD("CA");

}