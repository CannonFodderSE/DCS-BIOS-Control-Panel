#include "timebase.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_rcc.h"

static volatile uint32_t g_msTicks = 0;

/*********************************************************************
 * @fn      Timebase_Init
 *
 * @brief   Initializes TIM3 for use with button and encoder debounce functions.
 *
 * @return  none
 */
void Timebase_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef tim;
    TIM_TimeBaseStructInit(&tim);

    uint32_t timer_clk = SystemCoreClock / 2;

    tim.TIM_Prescaler     = (timer_clk / 10000) - 1;   // 10kHz
    tim.TIM_Period        = 10 - 1;                     // 1ms overflow
    tim.TIM_CounterMode   = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &tim);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************************
 * @fn      millis
 *
 * @brief   Creates a millisecond timer.
 *
 * @return  Count in milliseconds
 */
uint32_t millis(void)
{
    if (TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET)
    {
        TIM_ClearFlag(TIM3, TIM_FLAG_Update);
        g_msTicks++;
    }

    return g_msTicks;
}
