# Contador de Flexiones para Gimnasio

**Versión:** 0.1.0  
**Autor:** Rodrigo C.C.  
**Estado:** Prototipo funcional para pruebas

---

## 📋 Descripción del Proyecto

Sistema electrónico diseñado para contar flexiones de pecho en entornos de gimnasio utilizando un sensor ultrasónico HC-SR04. El dispositivo detecta el movimiento completo de bajada y subida, requiriendo que el usuario mantenga la posición baja durante 2 segundos para validar cada repetición, promoviendo una técnica correcta.

Este proyecto está basado en un **PIC16F628A** y ha sido desarrollado pensando en un producto comercial: código optimizado, multiplexación de displays y sistema no-bloqueante para una operación fluida.

---

## 🎯 Características Principales

- **Detección automática** de flexiones mediante sensor ultrasónico
- **Retención de 2 segundos** en posición baja para mayor exigencia
- **Display de 3 dígitos** con multiplexación para mostrar conteo y retroalimentación
- **Alimentación:** 5V DC
- **Configuración no-bloqueante** (no interrumpe el flujo del display)

---

## 📦 Componentes Utilizados

| Componente | Referencia | Cantidad |
|------------|------------|----------|
| Microcontrolador | PIC16F628A | 1 |
| Sensor ultrasónico | HC-SR04 | 1 |
| Display 7 segmentos | Cátodo común, 3 dígitos | 1 |
| Buzzer | Activo (5V) | 1 |
| Pulsadores | NA (Normalmente Abierto) | 2 |
| Cristal | No aplica (usa oscilador interno) | - |
| Resistencias | 220Ω (segmentos), 4.7kΩ (pull-up RA4) | Varias |

---

## 🔌 Diagrama de Pines (Conexiones)

### Microcontrolador PIC16F628A

| Componente | Pin PIC | Puerto | Observación |
|------------|---------|--------|-------------|
| **Sensor TRIG** | 15 | RA6 | Salida |
| **Sensor ECHO** | 16 | RA7 | Entrada |
| **Display Dígito 1** | 17 | RA0 | Multiplexación |
| **Display Dígito 2** | 18 | RA1 | Multiplexación |
| **Display Dígito 3** | 1 | RA2 | Multiplexación |
| **Display Segmentos** | 6-13 | RB0-RB7 | 7 segmentos |
| **Buzzer** | 13 | RB7 | Salida (con resistencia limitadora) |
| **Pulsador +** | 2 | RA3 | Pull-up externo |
| **Pulsador -** | 3 | RA4 | Pull-up externo |

> ⚠️ **Importante:** RA4 requiere una resistencia pull-up externa de 4.7kΩ a Vcc por ser open-drain.

---

## 📁 Estructura del Proyecto (MPLAB X)
---
contador_flexiones/
├── include/
│ ├── timer0.h # Base de tiempo (1ms)
│ ├── display.h # Multiplexación y buffer
│ ├── sensor.h # Sensor ultrasónico
│ ├── flexiones.h # Máquina de estados y conteo
│ └── buzzer.h # Control de pitidos
├── src/
│ ├── fusibles # Configuracion de fusibles
│ ├── timer0.c # ISR y funciones de tiempo
│ ├── display.c # Lógica del display
│ ├── sensor.c # Lectura del sensor
│ ├── flexiones.c # Detección de flexiones
│ └── buzzer.c # Gestión de pitidos
├── main.c # Bucle principal
├── README.md

---

## ⚙️ Configuración del Compilador

**IDE:** MPLAB X IDE v6.xx  
**Compilador:** XC8 v2.xx (modo gratuito)  
**Optimización:** `-Os` (optimizar para tamaño)  

**Configuración FUSES:**

```c
#pragma config FOSC = INTOSCIO   // Oscilador interno, RA6/RA7 I/O
#pragma config WDTE = OFF        // Watchdog deshabilitado
#pragma config PWRTE = ON        // Power-up timer
#pragma config MCLRE = ONN       // MCLR activado 
#pragma config BOREN = OFF       // Brown-out reset
#pragma config LVP = OFF         // Low Voltage Programming
#pragma config CPD = OFF         // Protección EEPROM
#pragma config CP = OFF          // Protección código
```

# 🧠 Lógica de Funcionamiento
---
    ARRIBA (reposo)
       │
       ▼ (distancia < umbral_bajo)
    BAJANDO
       │
       ▼ (distancia < 5cm)
     ABJO (flexión alcanzada)
       │
       ▼ (iniciar temporizador 2s)
  MANTENIENDO
       │
       ├── si sale antes → cancelar
       │
       ▼ (2 segundos cumplidos)
    SUBIENDO
       │
       ▼ (distancia > umbral_alto)
    ARRIBA → CONTADOR++
    
---

# 📧 Contacto
Autor: Rodrigo C.C.
Proyecto: Contador de Flexiones para Gimnasio
Estado: En desarrollo activo (prototipo funcional)
# ⚖️ Licencia
Este proyecto es de uso interno. Para consultas comerciales o de distribución, contactar al autor.
© 2026 Rodrigo C.C. - Todos los derechos reservados


