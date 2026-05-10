#ifndef FLEXIONES_H
#define FLEXIONES_H

#include <stdbool.h>
#include <stdint.h>

// Inicializar el contador de flexiones
void flexiones_init(void);

// Actualizar estado con la distancia medida
// Retorna el contador actual
// Parámetros:
//   distancia: distancia medida por el sensor (cm)
//   umbral_flexion: distancia para considerar "flexión completada" (8-15 cm)
//   umbral_arriba: distancia para considerar "posición inicial" (20-35 cm)
uint8_t flexiones_actualizar(uint16_t distancia, uint8_t umbral_flexion,
                             uint8_t umbral_arriba);

// Obtener el número de flexiones contadas
uint16_t flexiones_get_conteo(void);

// Resetear el contador de flexiones
void flexiones_reset(void);

#endif