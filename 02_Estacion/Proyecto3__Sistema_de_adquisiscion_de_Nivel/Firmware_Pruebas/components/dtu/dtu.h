#ifndef DTU_H
#define DTU_H

#include <stdint.h>
#include "driver/uart.h"

// ======================================================
// CONFIGURACION UART RS485
// ======================================================

#define DTU_UART_PORT     UART_NUM_2

#define DTU_TX_PIN        5
#define DTU_RX_PIN        6
#define DTU_RTS_PIN       7

#define DTU_BAUD_RATE     115200
#define DTU_BUF_SIZE      1024

// ======================================================
// FUNCIONES
// ======================================================

void dtu_init(void);

void dtu_enviar_dato(const char *dato);

void dtu_leer_respuesta(uint32_t timeout_ms);

#endif