#ifndef DISTANCIA_H
#define DISTANCIA_H

#include <stdint.h>

// Configuración de pines del sensor
#define TRIG_PIN PORTAbits.RA6
#define TRIG_TRIS TRISAbits.TRISA6
#define ECHO_PIN PORTAbits.RA7
#define ECHO_TRIS TRISAbits.TRISA7

// Límites del sensor
#define MAX_DISTANCIA_CM 50 // Máximo 50cm
#define TIMEOUT_TICKS 2900  // 50cm * 58us/cm = 1740us

// Parámetros del filtro exponencial
// alpha = 192 significa: 75% nueva lectura, 25% valor anterior
#define FILTER_ALPHA 192 // Rango: 128(0.5) a 224(0.875)
#define FILTER_ONE_MINUS_ALPHA (256 - FILTER_ALPHA) // 64 para alpha=192

// Inicializar el sensor
void sensor_init(void);

// Obtener distancia filtrada (0-30cm, 0 = sin objeto)
uint16_t sensor_leer_distancia(void);

// Obtener distancia sin filtro (para depuración)
uint16_t sensor_leer_distancia_raw(void);

// Resetear el filtro (útil después de timeout)
void sensor_reset_filtro(void);

#endif