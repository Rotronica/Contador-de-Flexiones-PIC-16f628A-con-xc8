#include "../include/flexiones.h"
#include "../include/buzzer.h"
#include <stdint.h>
#include <xc.h>

// Estados posibles del sistema
typedef enum {
  ESTADO_ARRIBA,  // Posición inicial (pecho arriba)
  ESTADO_BAJANDO, // Está bajando hacia el suelo
  ESTADO_ABJO,    // Flexión completada (pecho abajo)
  ESTADO_SUBIENDO // Está subiendo de vuelta
} estado_flexion_t;

//== == == == == == == == == == == == == == == == == == == == ==
// VARIABLES PRIVADAS
// ==========================================
static uint16_t contador = 0;
static estado_flexion_t estado = ESTADO_ARRIBA;
// Filtro anti-rebote
static uint8_t lecturas_consecutivas = 0;
#define LECTURAS_PARA_CAMBIO 2 // Necesita 2 lecturas iguales para cambiar

void flexiones_init(void) {
  contador = 0;
  estado = ESTADO_ARRIBA;
  lecturas_consecutivas = 0;
}

uint8_t flexiones_actualizar(uint16_t distancia, uint8_t umbral_flexion,
                             uint8_t umbral_arriba) {
  static uint16_t distancia_anterior = 0;
  static uint8_t contador_estable = 0;
  static uint16_t guardar_tiempo = 0;

  // Ignorar lecturas inválidas (0 = sin objeto)
  if (distancia == 0 || distancia >= 30) {
    // Si no hay objeto y estábamos en flexión, forzar vuelta a arriba
    // if (estado == ESTADO_ABJO || estado == ESTADO_SUBIENDO) {
    estado = ESTADO_ARRIBA;
    lecturas_consecutivas = 0;
    //}
  }

  // ==========================================
  // MÁQUINA DE ESTADOS
  // ==========================================
  switch (estado) {
  case ESTADO_ARRIBA:
    // Esperando que el usuario baje
    if (distancia <= umbral_flexion) {
      // Detectó bajada, cambiar estado
      estado = ESTADO_BAJANDO;
    }
    break;

  case ESTADO_BAJANDO:
    // Está bajando, esperar que llegue abajo
    if ((distancia <= umbral_flexion) && distancia > 0) {
      // Confirmar que sigue bajando
      if (distancia <= 5) {
        // Llegó muy abajo, considerar flexión completada
        estado = ESTADO_ABJO;
      }
    } else if (distancia > umbral_arriba) {
      // Volvió a subir sin completar (falso positivo)
      estado = ESTADO_ARRIBA;
    }
    break;

  case ESTADO_ABJO:
    // Flexión completada, esperando que suba
    if (distancia >= umbral_arriba) {
      // Empezó a subir
      estado = ESTADO_SUBIENDO;
    }
    break;

  case ESTADO_SUBIENDO:
    // Está subiendo, esperar que llegue arriba
    if (distancia >= umbral_arriba) {
      // Confirmar múltiples lecturas para evitar rebotes
      if (distancia == distancia_anterior) {
        contador_estable++;
        if (contador_estable >= LECTURAS_PARA_CAMBIO) {
          // ¡FLEXIÓN COMPLETA!
          contador++;
          buzzer_start();
          estado = ESTADO_ARRIBA;
          contador_estable = 0;
        }
      } else {
        contador_estable = 0;
      }
    } else if (distancia <= umbral_flexion) {
      // Volvió a bajar sin completar (rebote)
      estado = ESTADO_ABJO;
      contador_estable = 0;
    }
    break;
  }
  distancia_anterior = distancia;
  return (uint8_t)contador;
}