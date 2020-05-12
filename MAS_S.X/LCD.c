/*
 * File:   LCD_Lib.c
 * Author: Andres Sierra
 *
 * Created on 11 de marzo de 2020, 01:08 PM
 */


#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "header_mas.h"


void setCursorLCD(uint8_t y, uint8_t x){        //Sets cursor on x, y as needed
    if(y == 1){
        cursor = 0x80 + x - 1;
    }else{
        cursor = 0xC0 + x - 1;
    }
    cmdLCD(cursor);
}

void clcLCD(void){          //Clears screen
    cmdLCD(0x01);
}

void writeStrLCD(uint8_t *string){      //Writes a string to LCD
	int cont;
	for(cont=0;string[cont]!='\0';cont++){
        writeCharLCD(string[cont]);
    }
}

void writeCharLCD(uint8_t character){   //Writes a single character to LCD
   RS = 1;
   dataPort = character;
   E = 1;
   __delay_us(40);
   E = 0;
}

void initLCD(void){                     //Initializes LCD per the datasheet's instructions
     __delay_ms(15);
    cmdLCD(0x030);
    cmdLCD(0x030);
    cmdLCD(0x030);
  /////////////////////////////////////////////////////
    cmdLCD(0x38);
    cmdLCD(0x10);
    cmdLCD(0x01);
    cmdLCD(0x06);
    cmdLCD(0x0E);
}

void cmdLCD(uint8_t cmd){               //Writes as a command
    RS = 0;
	dataPort = cmd;
	E  = 1;
    __delay_ms(4);
    E  = 0;
}

void writeIntLCD(uint8_t numero){       //Converts integer to string and writes to LCD
    char buffer [4];
    sprintf (buffer, "%d", numero);
    writeStrLCD(buffer);
}

void writeFloat(uint8_t integer, uint8_t decimal, uint8_t initPos){ //Takes integer and decimals as parameters and writes them as a float to the LCD starting from position initPos
    //3.52V
    //123456789
    setCursorLCD(2, initPos);
    writeIntLCD(integer);// lcd_write_string
    setCursorLCD(2, initPos + 1);
    writeCharLCD('.');
    setCursorLCD(2, initPos + 2);
    
    if(decimal < 10){
        writeCharLCD('0');
    }
    writeIntLCD(decimal);
    
    setCursorLCD(2, initPos + 4);
    writeCharLCD('V');
}
