#pragma once

#include "ch32v30x.h"     // <-- ADD THIS LINE
#include <stdint.h>

/*
 * GPIO Manager
 * - Parses pins in "PA0", "PB12" format
 * - Enforces reserved pin blacklist
 * - Enables internal pull-ups
 */

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_pin_t;

int GPIO_ParsePin(const char *name, gpio_pin_t *out);
void GPIO_ConfigInputPullup(gpio_pin_t pin);
uint8_t GPIO_ReadPin(gpio_pin_t pin);
