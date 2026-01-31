#include <stdint.h>

/* 硬體暫存器定義 */
#define RCC_BASE      0x40023800
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

/* 全域變數 (初始值在 Flash，實體在 RAM) */
int g_secret_code = 0xABCD; 

void main(void) {
    /* 1. 開啟 GPIOA 時鐘 */
    RCC_AHB1ENR |= (1 << 0);

    /* 2. 設定 PA5 為 Output 模式 */
    GPIOA_MODER &= ~(3 << 10);
    GPIOA_MODER |= (1 << 10);

    /* 3. 測試變數搬運與修改 */
    if (g_secret_code == 0xABCD) {
        /* 測試寫入 RAM */
        g_secret_code = 0x1234;

        if (g_secret_code == 0x1234) {
            /* [成功] 變數讀寫正常 -> 綠燈恆亮 */
            GPIOA_ODR |= (1 << 5);
        } else {
            /* [失敗] 寫入無效 -> 狂閃 */
            while(1) {
                GPIOA_ODR ^= (1 << 5);
                for(volatile int i=0; i<100000; i++);
            }
        }
    } else {
        /* [失敗] 初始值搬運失敗 -> 狂閃 */
        while(1) {
            GPIOA_ODR ^= (1 << 5);
            for(volatile int i=0; i<50000; i++);
        }
    }

    /* 死迴圈防止程式跑飛 */
    while(1);
}