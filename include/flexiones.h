#pragma once
#include <stdbool.h>
#include <stdint.h>

void flexiones_init(void);
uint8_t flexiones_actualizar(uint16_t distancia, uint8_t umbral_flexion,
                             uint8_t umbral_arriba);
uint16_t flexiones_get_conteo(void);
