/*
 * File:   Main_Master.c
 * Author: Andres Sierra
 *
 * Codigo extraido de:
 * Author: Pablo
 * Ejemplo de uso de I2C Master
 * Created on 17 de febrero de 2020, 10:32 AM
 * Created on 12 de febrero de 2020, 11:19 AM
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
#include "header_pro.h"
#include <xc.h>
#include <math.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

int caudal, temp, prox, luz, hum, var, luz1 , temp1;
float pot_LCD, pot_dec, sensor, sen_dec;

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    temp=2;
    caudal=3;
    luz = 4;
    hum=5;
    //UART_Init(9600);
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        prox = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0x71);
        temp = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        caudal = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10); 
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        luz = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0x41);
        hum = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        temp1 = temp*35/200;
        luz1 = luz*100/255;
        
        //writeFloat(int_pot, dec1, 1);       //escribe el entero y los decimales 
        setCursorLCD(2, 1);
        writeIntLCD(prox);
        setCursorLCD(2, 3);                 // selecciona la posicion para escribir el sigiente valor
        writeIntLCD(temp1);                         
        setCursorLCD(2, 7);                // Selecciona la posicion final para escribir 
        writeIntLCD(hum);                   // escribe el valor de la fotoresistencia
        setCursorLCD(2, 10);                // Selecciona la posicion final para escribir 
        writeIntLCD(luz1);
        setCursorLCD(2, 14);                 // selecciona la posicion para escribir el sigiente valor
        writeIntLCD(caudal);                // 
        
                // Enviar valores USART
        if (RCIF) {
            var = RCREG;
        }
        switch (var) {
            case 1: // 1
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = prox;
                __delay_ms(10);
                break;
            case 2: // 2
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = temp1;
                __delay_ms(10);
                break;
            case 3: // 3
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = caudal;
                __delay_ms(10);
                break;
            case 4: // 4
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = luz1;
                __delay_ms(10);
                break;
            case 5: // 5
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = hum;
                __delay_ms(10);
                break;
            default: // 0 
                while (TXSTAbits.TRMT == 0) {
                }
                TXREG = 5;
                __delay_ms(10);
                break;
        }  
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //Configs
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    

    ////////////////////// serial  
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
    
    ///////////////////
    
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