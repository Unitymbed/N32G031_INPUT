
#include "n32g031.h"

extern uint32_t SystemCoreClock;


void delay_ms(uint32_t ms)
{
    SysTick->LOAD  = (SystemCoreClock / 1000) - 1;
    SysTick->VAL   = 0;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while (ms--)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }

    SysTick->CTRL = 0;
}


void gpio_init(void)
{
    // Enable clocks for GPIOA and GPIOB
    RCC->APB2PCLKEN |= RCC_APB2PCLKEN_IOPAEN | RCC_APB2PCLKEN_IOPBEN;

    // Set PB7, PB1 as push-pull output
    GPIOB->PMODE &= ~((3 << (7 * 2)) | (3 << (1 * 2)));   // Clear mode
    GPIOB->PMODE |=  ((1 << (7 * 2)) | (1 << (1 * 2)));   // Output mode

    GPIOB->POTYPE &= ~((1 << 7) | (1 << 1)); // Push-pull type

    // Set PA4 as input
    GPIOA->PMODE &= ~(3 << (4 * 2));         // Input mode
}

int main(void)
{
    SystemInit();
    gpio_init();

    while (1)
    {
        
        GPIOB->POD ^= (1 << 7);   // Toggle PB7
        delay_ms(500);

        
        if ((GPIOA->PID & (1 << 4)) == 0)   // Active LOW button press
        {
            GPIOB->POD |= (1 << 1);         // Turn ON PB1
        }
        else
        {
            GPIOB->POD &= ~(1 << 1);        // Turn OFF PB1
        }
    }
}

