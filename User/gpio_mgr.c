#include "gpio_mgr.h"
#include "ch32v30x.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* ---------------- RESERVED PIN LIST ---------------- */
static const char *reserved_pins[] =
{
    "PA2",  // FLASH-MEMORY CS
    "PA5",  // FLASH-MEMORY CLK
    "PA6",  // FLASH-MEMORY DO/IO1
    "PA7",  // FLASH-MEMORY DI/IO0
    "PA9",  // SUART1 TX
    "PA13",  // SWDIO
    "PA14",  // SWCLK
    "PA15",  // LED1 (RED LED)
    "PB2",  // BOOT BUTTON BOOT1
    "PB4",  // LED2 (GRN LED)
    "PB10",  // EEPROM CLK
    "PB11",  // EEPROM SDA
    "PC14",  // 32KHZ XTAL OSC32IN
    "PC15",  // 32KHZ XTAL OSC32OUT
};

#define RESERVED_COUNT (sizeof(reserved_pins)/sizeof(reserved_pins[0]))

static int IsReserved(const char *name)
{
    for (int i = 0; i < RESERVED_COUNT; i++)
    {
        if (strcmp(name, reserved_pins[i]) == 0)
            return 1;
    }
    return 0;
}

/* ---------------- PIN PARSER ---------------- */

int GPIO_ParsePin(const char *name, gpio_pin_t *out)
{
    if (!name || strlen(name) < 3)
        return -1;

    if (IsReserved(name))
    {
        printf("[GPIO] ERROR: Pin %s is reserved\r\n", name);
        return -2;
    }

    if (toupper(name[0]) != 'P')
        return -3;

    char portChar = toupper(name[1]);
    int pin = atoi(&name[2]);

    if (pin < 0 || pin > 15)
        return -4;

    switch (portChar)
{
    case 'A':
        out->port = GPIOA;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        break;

    case 'B':
        out->port = GPIOB;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        break;

    case 'C':
        out->port = GPIOC;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        break;

    case 'D':
        out->port = GPIOD;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        break;

    case 'E':
        out->port = GPIOE;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
        break;

    default:
        printf("[GPIO] ERROR: Invalid port %c\r\n", portChar);
        return -5;
}

    out->pin = (1 << pin);
    return 0;
}

/* ---------------- GPIO CONFIG ---------------- */

void GPIO_ConfigInputPullup(gpio_pin_t pin)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin   = pin.pin;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(pin.port, &gpio);
}

uint8_t GPIO_ReadPin(gpio_pin_t pin)
{
    return GPIO_ReadInputDataBit(pin.port, pin.pin);
}
