// src/config_pic.c
#include <xc.h>

// CONFIGURACIÓN DE FUSIBLES PARA PIC16F628A
#pragma config FOSC = INTOSCIO
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config CP = OFF