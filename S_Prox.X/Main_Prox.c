/*
 * File:   Main_Prox.c
 * Author: Andres Sierra
 *
 * Created on 7 de marzo de 2020, 05:11 PM
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
#include "header_prox.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
uint8_t z;
uint8_t var1 = 0;
//uint8_t dato;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        /*if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORT = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
        */   
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTAbits.RA0;
            SSPCONbits.CKP = 1;  
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }

}

void servoRotate0() //0 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RB0 = 1;
    __delay_us(800);
    RB0 = 0;
    __delay_us(19200);
  }
}

void servoRotate90() //90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RB0 = 1;
    __delay_us(1700);
    RB0 = 0;
    __delay_us(18300);
  }
}

void servoRotate180() //180 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RB0 = 1;
    __delay_us(2600);
    RB0 = 0;
    __delay_us(17400);
  }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    servoRotate0();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
        if(PORTAbits.RA0 == 1){
            PORTBbits.RB7 = 1;
            servoRotate90();  //90 Degree
        }
        else if(PORTAbits.RA0 == 0){
            //__delay_ms(3000);
            PORTBbits.RB7 = 0;
            servoRotate0();   
            }
        
        
        /*
         * if(PORTAbits.RA0 == 0){
            var1 = 1;
        }
         * if (var1 == 1 && PORTAbits.RA0 == 0){
            PORTBbits.RB7 = 1;
            servoRotate90();  //90 Degree
            //__delay_ms(3000);
            //PORTBbits.RB7 = 0;
            //servoRotate0();
            //var1 = 0;
        }
        if (var1 == 1 && PORTAbits.RA0 == 1){
            PORTBbits.RB7 = 0;
            __delay_ms(3000);
            servoRotate0();  //90 Degree
            //PORTBbits.RB7 = 0;
            //servoRotate0();
            var1 = 0;
        }*/
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0xFF; 
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    I2C_Slave_Init(0x80);    
}
