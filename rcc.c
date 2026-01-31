#include "rcc.h"

void rcc_setup(void) {
    /* 1. 開啟 PWR 並設定電壓 (Scale 1) */
    RCC_APB1ENR |= (1 << 28);
    PWR_CR |= (3 << 14);

    /* 2. 設定 Flash Latency = 3 Wait States */
    FLASH_ACR = (1 << 8) | (1 << 9) | (1 << 10) | (3 << 0);

    /* 3. 設定匯流排除頻 (APB1 = Div 2) */
    RCC_CFGR |= (4 << 10); 

    /* 4. 設定 PLL (16MHz -> 100MHz) */
    RCC_PLLCFGR = 0;             // 清空
    RCC_PLLCFGR |= (16 << 0);    // M=16
    RCC_PLLCFGR |= (200 << 6);   // N=200
    RCC_PLLCFGR |= (0 << 16);    // P=2
    
    /* [修正重點] Bit 22 必須是 0 (HSI)，絕對不能是 1 */
    RCC_PLLCFGR &= ~(1 << 22);   // Select HSI as PLL source

    RCC_PLLCFGR |= (7 << 24);    // Q=7

    /* 5. 啟動 PLL */
    RCC_CR |= (1 << 24);

    /* 6. 等待 PLL 鎖定 (原本卡死在這裡) */
    while (!(RCC_CR & (1 << 25)));

    /* 7. 切換系統時脈 */
    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |= (2 << 0);

    /* 8. 等待切換完成 */
    while ((RCC_CFGR & (3 << 2)) != (2 << 2));
}