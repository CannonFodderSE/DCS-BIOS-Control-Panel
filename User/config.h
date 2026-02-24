#pragma once
#include <stdint.h>
#include "gpio_mgr.h"

uint32_t millis(void);

#define MAX_ENCODERS   4
#define MAX_BUTTONS    32
#define MAX_TEXT_LEN   45

typedef struct {
    uint8_t dhcp;
    uint8_t join_multicast;
    uint8_t useDCSIP;
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t mask[4];
    uint8_t gw[4];
    uint8_t dcsip[4];
    uint16_t dcs_port;
} net_config_t;

typedef struct {
    gpio_pin_t a_pin;
    gpio_pin_t b_pin;
    char cw_text[MAX_TEXT_LEN];
    char ccw_text[MAX_TEXT_LEN];
} encoder_cfg_t;

typedef struct {
    gpio_pin_t pin;
    char press_text[MAX_TEXT_LEN];
    char release_text[MAX_TEXT_LEN];
} button_cfg_t;

typedef struct {
    uint16_t debounce_ms;
    uint16_t max_text_length;
    uint8_t encoder_count;
    uint8_t button_count;
    encoder_cfg_t encoders[MAX_ENCODERS];
    button_cfg_t buttons[MAX_BUTTONS];
    net_config_t net;
} system_config_t;

extern system_config_t g_cfg;

void Config_Load(void);
