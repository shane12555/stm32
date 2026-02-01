#include "uart.h"
#include <stdint.h> // 必須加入這個，因為我們用了 uint32_t

/* --- 1. 補齊暫存器地址定義 (這是編譯通過的關鍵) --- */
#define RCC_BASE          0x40023800
#define RCC_AHB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_BASE        0x40020000
#define GPIOA_MODER       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL        (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define USART2_BASE       0x40004400
#define USART2_SR         (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR         (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR        (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1        (*(volatile uint32_t *)(USART2_BASE + 0x0C))

void uart_init(void) {
    /* 1. 開啟時脈 */
    RCC_AHB1ENR |= (1 << 0);  // GPIOA EN
    RCC_APB1ENR |= (1 << 17); // USART2 EN

    /* 2. 配置 GPIO (PA2, PA3) 為 AF 模式 */
    GPIOA_MODER &= ~((3 << 4) | (3 << 6)); // 清除
    GPIOA_MODER |=  ((2 << 4) | (2 << 6)); // 設定 AF

    /* 3. 設定 AF7 (USART2) */
    GPIOA_AFRL &= ~((0xF << 8) | (0xF << 12)); // 清除
    GPIOA_AFRL |=  ((7   << 8) | (7   << 12)); // 設定 7

    /* 4. 設定波特率 (關鍵修改！) */
    // 我們要配合 rcc.c 的降頻設定 (APB1 = 25MHz)
    // 25,000,000 / 115200 = 217
    USART2_BRR = 217; 

    /* 5. 啟用 UART (發送 + 接收) */
    USART2_CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void uart_write(int ch) {
    while (!(USART2_SR & (1 << 7))); // 等待 TXE
    USART2_DR = (ch & 0xFF);
}

int uart_read(void) {
    while (!(USART2_SR & (1 << 5))); // 等待 RXNE (接收緩衝區有資料)
    return USART2_DR;
}