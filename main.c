#include <stdint.h>
#include "rcc.h"

#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define RCC_AHB1ENR_MAIN   (*(volatile uint32_t *)(0x40023800 + 0x30))

int main(void) {
    /* 1. 系統加速至 100MHz */
    rcc_setup();

    /* 2. 初始化 GPIO (PA5) */
    RCC_AHB1ENR_MAIN |= (1 << 0);
    GPIOA_MODER &= ~(3 << 10);
    GPIOA_MODER |= (1 << 10);

    while(1) {
        GPIOA_ODR ^= (1 << 5);
        /* 100MHz 下的延遲 */
        for(volatile int i=0; i<2000000; i++); // 我把它加倍到 200萬，讓閃爍稍微人眼可見一點
    }
}