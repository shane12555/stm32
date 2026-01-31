#ifndef UART_H
#define UART_H

#include <stdint.h>

/* --- 1. 定義 USART2 的基底地址 --- */
/* 查 Datasheet Memory Map 得到: USART2 在 APB1 匯流排，地址 0x4000 4400 */
#define USART2_BASE     0x40004400

/* --- 2. 定義暫存器結構 (這是最專業的寫法) --- */
/* 與其像 rcc.h 那樣一個一個 #define，不如用 struct 直接對應整個區塊 */
typedef struct {
    volatile uint32_t SR;    // Status Register (狀態)
    volatile uint32_t DR;    // Data Register (資料: 寫這裡=傳送, 讀這裡=接收)
    volatile uint32_t BRR;   // Baud Rate Register (波特率)
    volatile uint32_t CR1;   // Control Register 1 (控制1)
    volatile uint32_t CR2;   // Control Register 2 (控制2)
    volatile uint32_t CR3;   // Control Register 3 (控制3)
    volatile uint32_t GTPR;  // Guard Time (通常不用)
} USART_TypeDef;

/* 把地址轉型成結構指標，以後就可以用 USART2->DR 這樣操作 */
#define USART2 ((USART_TypeDef *)USART2_BASE)

/* --- 3. 定義 GPIO 相關 (PA2, PA3) --- */
#define GPIOA_BASE      0x40020000
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20)) // Alternate Function Low (Pin 0-7)

/* --- 4. 函式宣告 --- */
void uart_init(void);   // 初始化
void uart_write(int ch); // 傳送一個字
int uart_read(void);     // 接收一個字

#endif