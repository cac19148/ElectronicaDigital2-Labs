/*
 * Archivo:     Lab0.c
 * Dispositivo: PIC16F887
 * Autor:       Fernando Jose Caceros Morales
 * Compilador:  XC8 MPLABX V5.40
 * Programa:    Laboratorio1
 * Hardware:    
 * Creado:      21 de julio de 2021
 * Ultima modificacion: 
 */

#include <xc.h>
#include "ADConverter.h"
#include "7Seg.h"
#include "Oscilador_Config.h"


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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


#define _XTAL_FREQ 1000000     //frecuencia de 1 MHz
//******************************************************************************
//                              VARIABLES
//******************************************************************************

unsigned char   Display_No = 0;
unsigned char   Valor_ADC = 0;
unsigned char   Valor_ADC_n1 = 0;
unsigned char   Valor_ADCn0 = 0;
unsigned char   Valor_ADCn1 = 0;
unsigned char   RB0_old = 0;
unsigned char   RB1_old = 0;

//******************************************************************************
//                              INTERRUPCIÓN
//******************************************************************************
void __interrupt() isr(void)
{    
   
  //interrupcion del tmr0
    if (T0IF == 1) 
    {
        TMR0 = 100;
        INTCONbits.T0IF = 0; //se baja la bandera de interrupcion del tmr0
        PORTAbits.RA1 = 0;
        PORTAbits.RA2 = 0;
        //multiplexacion de los displays
        switch(Display_No) 
        {
            case 0: 
                PORTAbits.RA1 = 1; //display 0 encendido
                PORTAbits.RA2 = 0;
                PORTC = Tabla7Seg(Valor_ADCn1);//se traduce las centenas al display 2
                Display_No = 1; 
                break;
            case 1:
                PORTAbits.RA1 = 0;
                PORTAbits.RA2 = 1; //display 1 encendido
                PORTC = Tabla7Seg(Valor_ADCn0);//se traduce las decenas al display 1
                Display_No = 0;      
                break;
        }
    }
    //interrupcion del adc
    if (ADIF == 1) 
    {
        __delay_us(50);   //delay de 50 ms
        Valor_ADC = ADCVal (8);
        __delay_us(50);   //delay de 50 ms
        PIR1bits.ADIF = 0;//interrupcion de adc
        ADCON0bits.GO = 1;//inicio de la siguiente conversión
    }
    //interrupcion on-change
    if(RBIF == 1)  
    {
        if (PORTBbits.RB0 == 0) 
        {
            PORTEbits.RE0=1;
            PORTD = PORTD+1; 
        }
        else if (PORTBbits.RB1 == 0) 
        {
            PORTD = PORTD-1; 
        } 
        INTCONbits.RBIF = 0; //clear this interrupt condition 
    }
}

void main(void) {
    //configuraciones
    //configuracion reloj
    Osc_Config (1);
    //configuracion in out
    ANSEL = 1; //Pines digitales
    ANSELH = 0; //Pines digitales
    ANSELHbits.ANS8  = 1;//RA0 y RA1 como pines analogicos
    TRISA  = 0; 
    TRISB  = 7;
    TRISC  = 0;
    TRISD  = 0;
    TRISE  = 0;
    PORTA  = 0;//se limpian los puertos
    PORTB  = 0;
    PORTC  = 0;
    PORTD  = 0;
    PORTE  = 0;
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    //configuracion adc
    ADCON0bits.ADCS = 0;//00 se selecciona Fosc/2 para conversion (2us full TAD)
    ADCON0bits.CHS0 = 0;//se selecciona el canal AN0
    ADCON0bits.ADON = 1;//se enciende el adc
    __delay_us(50);   //delay de 50 ms
    ADCON0bits.GO = 1;  //se comienza la conversion adc
    ADCON1bits.VCFG1 = 0;//se ponen los voltajes de referencia internos del PIC
    ADCON1bits.VCFG0 = 0;//0V a 5V
    ADCON1bits.ADFM = 0; //se justifica a la izquierda, vals más significativos
    //configuracion tmr0
    OPTION_REGbits.T0CS = 0; //reloj interno (low to high)
    OPTION_REGbits.PSA  = 0; //prescaler 
    OPTION_REGbits.PS2  = 0; //011, 1:8
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 1;
    //reset tmr0
    TMR0 = 100; //para que el tmr0 se reinicie cada 5 ms
    INTCONbits.T0IF = 0; //baja la bandera de interrupcion del tmr0
    //configuracion interrupciones
    INTCONbits.GIE  = 1; //se habilitan las interrupciones globales
    INTCONbits.T0IE = 1; //interrupcion overflow tmr0 habilitada
    INTCONbits.PEIE = 1; //se habilitan las interrupciones de los perifericos
    //PIE1bits.ADIE = 1;   //se habilitan las interrupciones por adc
    INTCONbits.RBIE = 1;
    //configuracion interrupt on change b
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    Display_No = 0;
    while (1)
    {
        Valor_ADC_n1 = Valor_ADC & 0xf0;
        Valor_ADCn0 = Valor_ADC & 0x0f;
        Valor_ADCn1 = Valor_ADC_n1 >> 4;
        
        if (Valor_ADC > PORTD)
        {
            PORTAbits.RA3 = 1;
        }
        else
        {
            PORTAbits.RA3 = 0;
        }
    }
          
}