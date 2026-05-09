/**
 * @file main.c
 * @author Rodrigo C.C.
 * @date 2026-05-07
 * @brief Main function
 */

#include <stdbool.h>
#define _XTAL_FREQ 4000000 // 4 MHz (frecuencia del oscilador interno)
#include "include/buzzer.h"
#include "include/display.h"
#include "include/flexiones.h"
#include "include/sensor.h"
#include "include/timer0.h"
#include <stdint.h>
#include <xc.h>
#define LIMITE_BAJO 12
#define LIMITE_ALTO 25

int main() {
  static uint8_t contador = 0;
  static uint16_t distancia = 0;

  // Configuracion de pines
  CMCON = 0x07;      // <--- ¡LÍNEA CRUCIAL! Apaga todos los comparadores.
  PCONbits.OSCF = 1; // 1 = Oscilador interno a 4 MHz, 0 = 48 kHz
  timer0_init();
  display_init();
  sensor_init();
  flexiones_init();
  buzzer_init();
  while (1) {
    buzzer_update();
    distancia = sensor_leer_distancia();
    contador = flexiones_actualizar(distancia, LIMITE_BAJO, LIMITE_ALTO);
    display_visual_contador(contador);
  }
}