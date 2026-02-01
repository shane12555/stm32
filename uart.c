#include "uart.h"
#include <stdint.h>
#include "nvic.h"

/* --- 1. 補齊暫存器地址 (保證編譯不報錯) --- */
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
    GPIOA_MODER &= ~((3 << 4) | (3 << 6)); 
    GPIOA_MODER |=  ((2 << 4) | (2 << 6)); 

    /* 3. 設定 AF7 (USART2) */
    GPIOA_AFRL &= ~((0xF << 8) | (0xF << 12)); 
    GPIOA_AFRL |=  ((7   << 8) | (7   << 12)); 

    /* 4. 設定波特率 (配合 rcc.c 的原子鐘設定) */
    // 原子鐘輸入 = 8MHz -> PLL = 100MHz -> APB1 (/4) = 25MHz
    // 25,000,000 / 115200 = 217.01
    USART2_BRR = 217; 

    /*5. 設定 NVIC (CPU 總機) --- */
    // 我們剛剛算出來的：ISER1 的第 6 bit
    // 這樣 CPU 才會受理 UART 的請求
    NVIC_ISER1 |= (1 << 6);

    /* 通知硬體(Bit 5) + 總電源(Bit 13) + 發送(Bit 3) + 接收(Bit 2) */
    USART2_CR1 = (1 << 13) | (1 << 5) | (1 << 3) | (1 << 2);
}

void uart_write(int ch) {
    while (!(USART2_SR & (1 << 7))); // 等待 TXE
    USART2_DR = (ch & 0xFF);
}

int uart_read(void) {
    while (!(USART2_SR & (1 << 5))); // 等待 RXNE
    return USART2_DR;
}