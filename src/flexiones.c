#include "../include/flexiones.h"
#include "../include/buzzer.h"
#include <stdint.h>
#include <xc.h>

// ==========================================
// ESTADOS POSIBLES DEL SISTEMA
// ==========================================
typedef enum {
  ESTADO_ARRIBA,  // Posición inicial (pecho arriba)
  ESTADO_BAJANDO, // Está bajando hacia el suelo
  ESTADO_ABJO,    // Flexión completada (pecho abajo)
  ESTADO_SUBIENDO // Está subiendo de vuelta
} estado_flexion_t;

// ==========================================
// VARIABLES PRIVADAS
// ==========================================
static uint16_t contador = 0;
static estado_flexion_t estado = ESTADO_ARRIBA;

// Filtro anti-rebote
static uint8_t lecturas_consecutivas = 0;
#define LECTURAS_PARA_CAMBIO 2 // 2 lecturas iguales para confirmar cambio

// ==========================================
// FILTRO PARA ROPA (cambios bruscos)
// ==========================================
static uint8_t contador_inestable = 0;
#define LECTURAS_INESTABLES 2 // 2 lecturas anómalas para ignorar

// ==========================================
// FILTRO PARA "SIN OBJETO" TEMPORAL
// ==========================================
static uint8_t contador_sin_objeto = 0;
#define TIEMPO_SIN_OBJETO 20 // 20 lecturas ≈ 1 segundo

// ==========================================
// FUNCIONES PÚBLICAS
// ==========================================

void flexiones_init(void) {
  contador = 0;
  estado = ESTADO_ARRIBA;
  lecturas_consecutivas = 0;
  contador_inestable = 0;
  contador_sin_objeto = 0;
}

/*void flexiones_reset(void) {
  contador = 0;
  estado = ESTADO_ARRIBA;
  lecturas_consecutivas = 0;
  contador_inestable = 0;
  contador_sin_objeto = 0;
}*/

/*uint16_t flexiones_get_conteo(void) { return contador; }
 */
// ==========================================
// FUNCIÓN PRINCIPAL DE ACTUALIZACIÓN
// ==========================================
uint8_t flexiones_actualizar(uint16_t distancia, uint8_t umbral_flexion,
                             uint8_t umbral_arriba) {
  static uint16_t distancia_anterior = 0;
  static uint8_t contador_estable = 0;
  static uint16_t distancia_estable = 0;

  // ==========================================
  // PASO 1: VALIDAR LECTURA VÁLIDA
  // ==========================================
  if (distancia == 0) {
    // Sin objeto detectado - contador de ausencia
    contador_sin_objeto++;
    if (contador_sin_objeto >= TIEMPO_SIN_OBJETO) {
      // Más de 1 segundo sin objeto: reiniciar estado
      if (estado != ESTADO_ARRIBA) {
        estado = ESTADO_ARRIBA;
        lecturas_consecutivas = 0;
        contador_estable = 0;
      }
    }
    return (uint8_t)contador;
  } else {
    contador_sin_objeto = 0;
  }

  // Limitar distancia máxima (más allá de esto no es relevante)
  if (distancia > 50) {
    distancia = 50;
  }

  // ==========================================
  // PASO 2: FILTRO PARA CAMBIOS BRUSCOS (ROPA)
  // ==========================================
  if (distancia_anterior != 0) {
    int16_t cambio = (int16_t)distancia - (int16_t)distancia_anterior;

    // Si el cambio es muy brusco (>10cm), puede ser interferencia de ropa
    if (cambio > 10 || cambio < -10) {
      contador_inestable++;
      if (contador_inestable < LECTURAS_INESTABLES) {
        // Ignorar esta lectura inestable
        return (uint8_t)contador;
      }
    } else {
      contador_inestable = 0;
    }

    // Si hay demasiados cambios bruscos, aumentar estabilidad
    if (contador_inestable >= LECTURAS_INESTABLES) {
      // Usar el promedio de las últimas dos lecturas estables
      distancia = (distancia + distancia_estable) / 2;
      contador_inestable = 0;
    }
  }

  // Guardar distancia estable para futuras comparaciones
  distancia_estable = distancia;

  // ==========================================
  // PASO 3: MÁQUINA DE ESTADOS
  // ==========================================
  switch (estado) {

  case ESTADO_ARRIBA:
    // Esperando que el usuario baje
    if (distancia <= umbral_flexion) {
      estado = ESTADO_BAJANDO;
      lecturas_consecutivas = 0;
    }
    break;

  case ESTADO_BAJANDO:
    // Está bajando, esperar que llegue abajo
    if (distancia <= umbral_flexion && distancia > 0) {
      // Confirmar que está en posición de flexión
      if (distancia <= 5) {
        estado = ESTADO_ABJO;
        lecturas_consecutivas = 0;
      }
    } else if (distancia > umbral_arriba) {
      // Volvió a subir sin completar la flexión
      estado = ESTADO_ARRIBA;
    }
    break;

  case ESTADO_ABJO:
    // Flexión completada, esperando que suba
    if (distancia >= umbral_arriba) {
      estado = ESTADO_SUBIENDO;
      lecturas_consecutivas = 0;
      contador_estable = 0;
    }
    break;

  case ESTADO_SUBIENDO:
    // Está subiendo, esperar que llegue arriba
    if (distancia >= umbral_arriba) {
      // Confirmar múltiples lecturas para evitar rebotes
      if (distancia == distancia_anterior) {
        contador_estable++;
        if (contador_estable >= LECTURAS_PARA_CAMBIO) {
          // ⭐ FLEXIÓN COMPLETA Y VÁLIDA
          contador++;
          buzzer_start(); // Activar buzzer
          estado = ESTADO_ARRIBA;
          contador_estable = 0;
          lecturas_consecutivas = 0;
        }
      } else {
        contador_estable = 0;
      }
    } else if (distancia <= umbral_flexion) {
      // Volvió a bajar sin completar (rebote o duda)
      estado = ESTADO_ABJO;
      contador_estable = 0;
    }
    break;
  }

  // Guardar distancia para la próxima iteración
  distancia_anterior = distancia;

  return (uint8_t)contador;
}