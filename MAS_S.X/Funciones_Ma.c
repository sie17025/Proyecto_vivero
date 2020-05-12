/*
 * File:   Funciones_Ma.c
 * Author: Andres Sierra
 *
 * Created on 15 de febrero de 2020, 05:54 PM
 */


#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000
#include "header_mas.h"



void conf(void){
    PORTA = 0;          
    PORTB = 0;
    PORTC = 0;  
    PORTD = 0;
    
    TRISA = 0; 
    TRISB = 0; 
    TRISC = 0b00010000;    //clk para el slave
    TRISD = 0;
    ANSEL = 0;     
    ANSELH = 0;            // TODAS LAS ENTRADAS DIGITALES
    
  
    ADCON1 = 0;
   
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    ////////////////////// serial  
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 103;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    ///////////////////
      
    
    //INTCONbits.PEIE =1;     // HABILITA INTERRUPCIONES 
    /*INTCONbits.T0IE =1;
    INTCONbits.T0IF =1;*/
    
    //PIE1bits.ADIE = 1;      // HABILITA INTERRUPCION DE ADC
    //PIR1bits.ADIF = 0;      // BANDERA DE A/D  
   // ADCON0bits.ADON = 1;      // HABILITA ADC
    //__delay_ms(20);
    //ADCON0bits.GO_DONE = 1;   // EMPEZAR LA CONVERCION A/D
   }

// ------------ Configuracion de la Frecuencia del Oscilador ------------------

void Freq_Osc(char option){
    switch(option){
        case 0:
            // Oscilador a 31 kHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
        case 1:
            // Oscilador a 125 kHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
        case 2:
            // Oscilador a 250 kHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;
            break;
        case 3:
            // Oscilador a 500 kHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;
            break;
        case 4:
            // Oscilador a 1 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 5:
            // Oscilador a 2 MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 6:
            // Oscilador a 4 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        case 7:
            // Oscilador a 8 MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        default:
            // Oscilador a 4 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
    }
    OSCCONbits.SCS = 1;         // iniciar con el oscilador interno
}

