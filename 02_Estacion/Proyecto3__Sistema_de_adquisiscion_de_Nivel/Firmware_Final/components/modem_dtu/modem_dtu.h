#ifndef DTU_H
#define DTU_H

#include <stdbool.h>
#include <stdint.h>

#define DTU_RESPONSE_TIMEOUT_MS 12000

void dtu_init(void);

bool dtu_send_level(
    uint32_t timestamp,
    float level_m
);

#endif