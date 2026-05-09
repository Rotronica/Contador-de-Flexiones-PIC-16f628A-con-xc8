#include "../include/display.h"
#include "../include/timer0.h"
#include <stdint.h>
#include <xc.h>

#define TIEMPO 4 // Tiempo de multiplexado ms
static uint16_t time_1 = 0;
#define DIGITOS 3

#define DISPLAY_1 0 // Unidad
#define DISPLAY_2 1 // Decena
#define DISPLAY_3 2 // Centena
// Caracteres del 1-9 par la multiplexacion
static const uint8_t segmentos[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
                                    0x6D, 0x7D, 0x07, 0x7F, 0x6F};
static uint8_t buffer_datos[DIGITOS] = {0x3F, 0x3F, 0x3F}; // inicia en 000
static uint8_t nuevo_buffer[3];
static uint8_t actualizacion_pendiente = 0;

void display_init(void) {
  // Configuramaso RA0-RA2 como salidas
  TRISA &= ~(1 << DISPLAY_1 | 1 << DISPLAY_2 | 1 << DISPLAY_3);
  PORTA &= ~(1 << DISPLAY_1 | 1 << DISPLAY_2 | 1 << DISPLAY_3);
  // Puerto de salida para display del RB0-RB6
  TRISB &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6);
  PORTB &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6);
  // Pantalla display mostrando 000
  buffer_datos[0] = segmentos[0];
  buffer_datos[1] = segmentos[0];
  buffer_datos[2] = segmentos[0];
  display_multiplexar();
}
// ==========================================
// display.c - Versión SIN PARPADEO
// ==========================================

void display_visual_contador(uint16_t numero) {
  // Calcular en buffer temporal
  if (numero >= 100) {
    nuevo_buffer[0] = segmentos[numero % 10];
    nuevo_buffer[1] = segmentos[(numero / 10) % 10];
    nuevo_buffer[2] = segmentos[numero / 100];
  } else if (numero >= 10) {
    nuevo_buffer[0] = segmentos[numero % 10];
    nuevo_buffer[1] = segmentos[numero / 10];
    nuevo_buffer[2] = 0x00;
  } else {
    nuevo_buffer[0] = segmentos[numero];
    nuevo_buffer[1] = 0x00;
    nuevo_buffer[2] = 0x00;
  }
  actualizacion_pendiente = 1;
}
void display_multiplexar(void) {
  if (timer0_espera(&time_1, TIEMPO)) {
    static uint8_t digito_actual = 0;
    // Apagamos todo los digitos
    PORTA &= ~(1 << DISPLAY_1 | 1 << DISPLAY_2 | 1 << DISPLAY_3);
    // ⭐ Actualizar buffer en el momento seguro (todo apagado)
    if (actualizacion_pendiente) {
      buffer_datos[0] = nuevo_buffer[0];
      buffer_datos[1] = nuevo_buffer[1];
      buffer_datos[2] = nuevo_buffer[2];
      actualizacion_pendiente = 0;
    }
    // Mostrar por puerto hacia display
    PORTB = buffer_datos[digito_actual]; // Simplemente muestra el mensaje en el
                                         // buffer
    // Encender el digito correspondiente
    PORTA |= (1 << digito_actual);
    digito_actual++;
    if (digito_actual >= DIGITOS) {
      digito_actual = 0;
    }
  }
}
