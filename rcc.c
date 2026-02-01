#include <stdint.h>
#include "rcc.h"

void rcc_setup(void) {
    /* 1. 開啟 HSE (原子鐘/外部晶振) + BYPASS 模式 */
    // Bit 16: HSEON, Bit 18: HSEBYP (因為時脈來自 ST-Link)
    RCC_CR |= (1 << 16) | (1 << 18);
    while (!(RCC_CR & (1 << 17))); // 等待穩定

    /* 2. 電源設定 */
    RCC_APB1ENR |= (1 << 28);
    PWR_CR |= (3 << 14);
    FLASH_ACR = (1 << 8) | (1 << 9) | (1 << 10) | (3 << 0);

    /* 3. 設定 PLL (來源改用 HSE) */
    RCC_PLLCFGR = 0;
    RCC_PLLCFGR |= (1 << 22);   // [重點] 來源選擇 HSE
    RCC_PLLCFGR |= (8 << 0);    // M=8 (8MHz / 8 = 1MHz)
    RCC_PLLCFGR |= (200 << 6);  // N=200 (1MHz * 200 = 200MHz)
    RCC_PLLCFGR |= (0 << 16);   // P=2 (200MHz / 2 = 100MHz)
    RCC_PLLCFGR |= (7 << 24);   // Q=7

    /* 4. APB1 除以 4 (25MHz) */
    RCC_CFGR |= (5 << 10);

    /* 5. 啟動 PLL */
    RCC_CR |= (1 << 24);
    while (!(RCC_CR & (1 << 25)));

    /* 6. 切換系統時脈 */
    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |= (2 << 0);
    while ((RCC_CFGR & (3 << 2)) != (2 << 2));
}