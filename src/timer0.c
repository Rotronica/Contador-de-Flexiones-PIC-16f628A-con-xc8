#include "../include/timer0.h"
#include <stdint.h>
#include <xc.h>

static uint16_t milisegundos = 0;
// Funcion de interrupcion para el tmr0
void __interrupt() isr() {
  if (INTCONbits.T0IF == 1) {
    INTCON &= ~(1 << 2); // Limpia la bandera del int. TMR0
    TMR0 = 131;
    milisegundos++;
  }
}
// Funcion de inicializacion del TMR0
void timer0_init(void) {
  // Configuracion par utilizar TMR0 modo Temporizador
  OPTION_REG = 0b10000010;
  // Activacion de la interrupcion TMR0
  INTCON |= (1 << 7 | 1 << 5);
  // Carga del registro TMR0 par una temporizacion de 1ms
  TMR0 = 131;
}
uint16_t timer0_millis(void) {
  uint16_t temp;
  INTCON &= ~(1 << 7); // Descativamos la interrupcion global
  temp = milisegundos;
  INTCON |= (1 << 7); // Se vuelve a activar la interrupcion global
  return temp;
}
uint8_t timer0_espera(uint16_t *referencia, uint16_t intervalo_ms) {
  uint16_t ahora = timer0_millis();
  if ((ahora - *referencia) >= intervalo_ms) {
    *referencia = ahora;
    return 1; // El tiempo paso
  }
  return 0; // El tiempo a un no paso
}