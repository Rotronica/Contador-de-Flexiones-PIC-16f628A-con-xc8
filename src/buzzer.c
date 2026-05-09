#include "../include/buzzer.h"
#include "../include/timer0.h"
#include <stdbool.h>
#include <xc.h>

#define BUZZER_PIN 3 // Pin RB7
#define TIEMPO_MS 500

static bool buzzer_activo = false;
static uint16_t tiempo_buzzer = 0;

void buzzer_init(void) {
  TRISA &= ~(1 << BUZZER_PIN);
  PORTA &= ~(1 << BUZZER_PIN);
}

// Cuando quieras activar el buzzer (ej: al completar una flexión)
void buzzer_start(void) {
  buzzer_activo = true;
  tiempo_buzzer = timer0_millis(); // Guardar el momento de inicio
  PORTA |= 1 << BUZZER_PIN;        // Encender inmediatamente
}

// En el main loop (llamar MUY frecuente)
void buzzer_update(void) {
  if (buzzer_activo) {
    // Verificar si ya pasaron 2 segundos
    if (timer0_espera(&tiempo_buzzer, TIEMPO_MS)) {
      // ¡Se cumplió el tiempo!
      PORTA &= ~(1 << BUZZER_PIN);
      buzzer_activo = false; // Desactivar estado
    }
    // Si no se cumplió el tiempo, NO hacer nada (el buzzer sigue sonando)
  }
}