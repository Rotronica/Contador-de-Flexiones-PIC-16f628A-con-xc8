#pragma once
#include <stdint.h>
void timer0_init(void);
uint16_t timer0_millis(void);
uint8_t timer0_espera(uint16_t *referencia, uint16_t intervalo_ms);