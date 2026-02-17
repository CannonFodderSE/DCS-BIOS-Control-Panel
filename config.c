#include "config.h"
#include "ini_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char newline_str[2] = "\n";
size_t current_len = 0;

system_config_t g_cfg;

/* ---------------- DEFAULT CONFIG ---------------- */

static const char g_ini_text[] =
#include "dcs_config.ini.inc"
;

/*
 * dcs_config.ini.inc will contain the raw INI text as a C string.
 * See instructions below.
 */

/* ---------------- PARSER CALLBACK ---------------- */

static void cfg_handler(const char *section,
                        const char *key,
                        const char *value)
{
    char *octet;
    int place = 1;
    if (strcmp(section, "general") == 0)
    {
        if (strcmp(key, "debounce_ms") == 0)
            g_cfg.debounce_ms = atoi(value);
        else if (strcmp(key, "max_text_length") == 0)
            g_cfg.max_text_length = atoi(value);
    }
    else if (strcmp(section, "network") == 0)
    {
        if (strcmp(key, "dhcp") == 0)
            g_cfg.net.dhcp = atoi(value);
        else if (strcmp(key, "join_multicast") == 0)
            g_cfg.net.join_multicast = atoi(value);
        else if (strcmp(key, "use_dcs_ip") == 0)
            g_cfg.net.useDCSIP = atoi(value);
        else if (strcmp(key, "dcs_port") == 0)
            g_cfg.net.dcs_port = atoi(value);
        else if (strcmp(key, "module_ip") == 0){
            // Use strtok to get the first octet
            octet = strtok(value, ".");
            g_cfg.net.ip[0] = atoi(octet);
            // Loop through the rest of the string to get the remaining octets
            while (octet != NULL) {
                // Call strtok again with NULL as the first argument to continue tokenizing the same string
                octet = strtok(NULL, ".");
                g_cfg.net.ip[place] = atoi(octet);
                place++;
            }
            octet = 0;
            place = 1;
        }
        else if (strcmp(key, "subnet_mask") == 0){
            // Use strtok to get the first octet
            octet = strtok(value, ".");
            g_cfg.net.mask[0] = atoi(octet);
            // Loop through the rest of the string to get the remaining octets
            while (octet != NULL) {
                // Call strtok again with NULL as the first argument to continue tokenizing the same string
                octet = strtok(NULL, ".");
                g_cfg.net.mask[place] = atoi(octet);
                place++;
            }
            octet = 0;
            place = 1;
        }
        else if (strcmp(key, "dcs_ip") == 0){
            // Use strtok to get the first octet
            octet = strtok(value, ".");
            g_cfg.net.dcsip[0] = atoi(octet);
            // Loop through the rest of the string to get the remaining octets
            while (octet != NULL) {
                // Call strtok again with NULL as the first argument to continue tokenizing the same string
                octet = strtok(NULL, ".");
                g_cfg.net.dcsip[place] = atoi(octet);
                place++;
            }
            octet = 0;
            place = 1;
            
        }
        else if (strcmp(key, "gateway") == 0){
            // Use strtok to get the first octet
            octet = strtok(value, ".");
            g_cfg.net.gw[0] = atoi(octet);
            // Loop through the rest of the string to get the remaining octets
            while (octet != NULL) {
                // Call strtok again with NULL as the first argument to continue tokenizing the same string
                octet = strtok(NULL, ".");
                g_cfg.net.gw[place] = atoi(octet);
                place++;
            }
            octet = 0;
            place = 1;
        }
        else if (strcmp(key, "mac") == 0)
            {
                unsigned int m[6];
                if (sscanf(value, "%02x:%02x:%02x:%02x:%02x:%02x",
                        &m[0], &m[1], &m[2],
                        &m[3], &m[4], &m[5]) == 6)
                {
                    for (int i = 0; i < 6; i++)
                        g_cfg.net.mac[i] = (uint8_t)m[i];
                }
                else
                {
                    printf("[CFG] Invalid MAC format\r\n");
                }
            }

    }
    else if (strcmp(section, "encoders") == 0)
    {
        if (strcmp(key, "count") == 0)
            g_cfg.encoder_count = atoi(value);

        for (int i = 0; i < MAX_ENCODERS; i++)
        {
            char tmp[32];
            sprintf(tmp, "enc%d_a_pin", i);
            if (strcmp(key, tmp) == 0)
                GPIO_ParsePin(value, &g_cfg.encoders[i].a_pin);

            sprintf(tmp, "enc%d_b_pin", i);
            if (strcmp(key, tmp) == 0)
                GPIO_ParsePin(value, &g_cfg.encoders[i].b_pin);

            sprintf(tmp, "enc%d_cw_text", i);
            if (strcmp(key, tmp) == 0){
                strncpy(g_cfg.encoders[i].cw_text, value, MAX_TEXT_LEN);

                // Append "\n" to end of string
                current_len = strlen(g_cfg.encoders[i].cw_text);
                strncat(g_cfg.encoders[i].cw_text,
                    newline_str,
                    sizeof(g_cfg.encoders[i].cw_text) - current_len - 1);
            }

            sprintf(tmp, "enc%d_ccw_text", i);
            if (strcmp(key, tmp) == 0){
                strncpy(g_cfg.encoders[i].ccw_text, value, MAX_TEXT_LEN);
            
                // Append "\n" to end of string
                current_len = strlen(g_cfg.encoders[i].ccw_text);
                strncat(g_cfg.encoders[i].ccw_text,
                    newline_str,
                    sizeof(g_cfg.encoders[i].ccw_text) - current_len - 1);
            }
        }
    }
    else if (strcmp(section, "buttons") == 0)
    {
        if (strcmp(key, "count") == 0)
            g_cfg.button_count = atoi(value);

        for (int i = 0; i < MAX_BUTTONS; i++)
        {
            char tmp[32];
            sprintf(tmp, "btn%d_pin", i);
            if (strcmp(key, tmp) == 0)
                GPIO_ParsePin(value, &g_cfg.buttons[i].pin);

            sprintf(tmp, "btn%d_press_text", i);
            if (strcmp(key, tmp) == 0){
                strncpy(g_cfg.buttons[i].press_text, value, MAX_TEXT_LEN);
                
                // Append "\n" to end of string
                current_len = strlen(g_cfg.buttons[i].press_text);
                strncat(g_cfg.buttons[i].press_text,
                    newline_str,
                    sizeof(g_cfg.buttons[i].press_text) - current_len - 1);
            }

            sprintf(tmp, "btn%d_release_text", i);
            if (strcmp(key, tmp) == 0){
                strncpy(g_cfg.buttons[i].release_text, value, MAX_TEXT_LEN);
                
                // Append "\n" to end of string
                current_len = strlen(g_cfg.buttons[i].release_text);
                strncat(g_cfg.buttons[i].release_text,
                    newline_str,
                    sizeof(g_cfg.buttons[i].release_text) - current_len - 1);
            }
        }
    }
}

/* ---------------- LOAD FUNCTION ---------------- */

void Config_Load(void)
{
    memset(&g_cfg, 0, sizeof(g_cfg));
    // Default locally-administered MAC if not overridden
    uint8_t default_mac[6] = { 0x02, 0x32, 0x30, 0x70, 0x00, 0x01 };
    memcpy(g_cfg.net.mac, default_mac, 6);

    printf("[CFG] Parsing configuration\r\n");
    INI_Parse(g_ini_text, cfg_handler);

    printf("[CFG] Encoders: %d\r\n", g_cfg.encoder_count);
    printf("[CFG] Buttons : %d\r\n", g_cfg.button_count);
    printf("[CFG] Debounce: %d ms\r\n", g_cfg.debounce_ms);
}
