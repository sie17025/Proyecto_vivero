/*
 * File:   Main_Tr.c
 * Author: Andres Sierra
 *
 * Created on 11 de marzo de 2020, 03:21 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "header_tr.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
uint8_t z;
int var1, Pot1, Pot2, cont, temp, caudal, prox, luz, hum, escoger;
//uint8_t dato;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main


//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){
        escoger = spiRead();
        if (escoger == 0 ){
            spiWrite(prox);
        }
        else if(escoger == 1 ){
            spiWrite(temp);
        }
        else if(escoger == 2 ){
            spiWrite(caudal);
        }
        else if(escoger == 3 ){
            spiWrite(luz);
        }
        else if(escoger == 4 ){
            spiWrite(hum);
        }
        SSPIF = 0;      
    }
}
//*****************************************************************************
void setup(void);
// ************************** INTERRUPCIONES ******************************

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    //UART_Init(9600);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){       
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
        
        while (TXSTAbits.TRMT == 0) {
        }
        TXREG = 1;
        while (!RCIF) {
        }
        prox = RCREG;
        __delay_ms(100);
        while (TXSTAbits.TRMT == 0) {
        }
        TXREG = 2;
        while (!RCIF) {
        }
        temp = RCREG;
        __delay_ms(100);
        while (TXSTAbits.TRMT == 0) {
        }
        TXREG = 3;
        while (!RCIF) {
        }
        caudal = RCREG;
        __delay_ms(100);
        while (TXSTAbits.TRMT == 0) {
        }
        TXREG = 4;
        while (!RCIF) {
        }
        luz = RCREG;
        __delay_ms(100);
        while (TXSTAbits.TRMT == 0) {
        }
        TXREG = 5;
        while (!RCIF) {
        }
        hum = RCREG;
        __delay_ms(100);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    PORTA = 0;          
    PORTB = 0; 
    PORTD = 0;
    TRISA = 0; 
    TRISB = 0; 
    TRISC = 0b10011000;
    TRISD = 0;
    ANSEL = 0;     
    ANSELH = 0;            // TODAS LAS ENTRADAS DIGITALES

  
    //ADCON1 = 0;
    //////////////////// serial  
    
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;

    //////////////////
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE   
    
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    TRISAbits.TRISA5 = 1;       // SS
       
    //////////////////
    
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
