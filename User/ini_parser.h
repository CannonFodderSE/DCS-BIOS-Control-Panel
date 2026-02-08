#pragma once
#include <stdint.h>

/*
 * Simple INI parser
 * - Supports [section]
 * - key = value
 * - Comments starting with ';'
 */

typedef void (*ini_callback_t)(const char *section,
                               const char *key,
                               const char *value);

void INI_Parse(const char *text, ini_callback_t cb);
