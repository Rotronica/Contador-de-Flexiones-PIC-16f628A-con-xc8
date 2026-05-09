#define _XTAL_FREQ 4000000 // 4 MHz (frecuencia del oscilador interno)

#include "../include/sensor.h"
#include <stdint.h>
#include <xc.h>

// ==========================================
// VARIABLES ESTÁTICAS DEL FILTRO
// ==========================================
static uint16_t distancia_filtrada = 0;
static uint8_t filtro_inicializado = 0;

// ==========================================
// INICIALIZACIÓN DEL SENSOR
// ==========================================
void sensor_init(void) {
  // Configurar pines
  TRIG_TRIS = 0; // TRIG como salida
  ECHO_TRIS = 1; // ECHO como entrada
  TRIG_PIN = 0;  // Estado inicial bajo

  // Configurar Timer1 para medición de pulsos
  T1CON = 0x00; // Timer1 apagado, prescaler 1:1, reloj interno

  // Inicializar filtro
  distancia_filtrada = 0;
  filtro_inicializado = 0;
}

// ==========================================
// RESETEAR EL FILTRO
// ==========================================
/*void sensor_reset_filtro(void) {
  filtro_inicializado = 0;
  distancia_filtrada = 0;
}*/

// ==========================================
// LECTURA CRUDA DEL SENSOR (SIN FILTRO)
// ==========================================
uint16_t sensor_leer_distancia_raw(void) {
  uint16_t ticks = 0;
  uint16_t timeout = 0;
  uint16_t distancia;

  // 1. Enviar pulso TRIG de 10us
  TRIG_PIN = 1;
  __delay_us(10);
  TRIG_PIN = 0;

  // 2. Esperar que ECHO se ponga en alto
  timeout = 0;
  while (ECHO_PIN == 0) {
    timeout++;
    if (timeout > 5000) {
      return 0; // Timeout sin eco
    }
  }

  // 3. Resetear e iniciar Timer1 para medir el eco
  TMR1H = 0;
  TMR1L = 0;
  T1CONbits.TMR1ON = 1;

  // 4. Medir duración del pulso ECHO
  while (ECHO_PIN == 1) {
    ticks = ((uint16_t)TMR1H << 8) | TMR1L;
    if (ticks > TIMEOUT_TICKS) {
      T1CONbits.TMR1ON = 0;
      return 0; // Objeto fuera del rango (más de 50cm)
    }
  }

  // 5. Apagar Timer1 y leer valor final
  T1CONbits.TMR1ON = 0;
  ticks = ((uint16_t)TMR1H << 8) | TMR1L;

  // 6. Convertir a centímetros (1 tick = 1us, 1cm = 58us)
  distancia = ticks / 58;

  // 7. Limitar al rango máximo
  if (distancia > MAX_DISTANCIA_CM) {
    distancia = 0;
  }

  return distancia;
}

// ==========================================
// LECTURA CON FILTRO EXPONENCIAL
// ==========================================
uint16_t sensor_leer_distancia(void) {
  uint16_t distancia_nueva;

  // Obtener lectura cruda del sensor
  distancia_nueva = sensor_leer_distancia_raw();

  // ==========================================
  // FILTRO EXPONENCIAL
  // Fórmula: filtrada = α * nueva + (1-α) * filtrada_anterior
  // Con α = 192/256 = 0.75 (75% nueva, 25% anterior)
  // ==========================================

  if (!filtro_inicializado) {
    // Primera lectura: inicializar el filtro
    distancia_filtrada = distancia_nueva;
    filtro_inicializado = 1;
  } else {
    // Aplicar filtro exponencial usando enteros (evita float)
    // distancia_filtrada = (FILTER_ALPHA * nueva + FILTER_ONE_MINUS_ALPHA *
    // anterior) / 256
    distancia_filtrada = (FILTER_ALPHA * distancia_nueva +
                          FILTER_ONE_MINUS_ALPHA * distancia_filtrada) >>
                         8;
  }

  // Si no hay objeto (lectura 0), forzar respuesta inmediata
  // Esto evita que el filtro "retenga" el último valor cuando ya no hay objeto
  if (distancia_nueva == 0) {
    distancia_filtrada = 0;
    filtro_inicializado = 0; // Reiniciar para la próxima detección
  }

  return distancia_filtrada;
}