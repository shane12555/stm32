#include <stdio.h>
#include <stdint.h>
#include "rcc.h"
#include "uart.h"
#include "nvic.h" // 必須要有這個

// 補上暫存器定義，避免編譯錯誤
#define USART2_BASE       0x40004400
#define USART2_SR         (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR         (*(volatile uint32_t *)(USART2_BASE + 0x04))
// 為了讀取 CR1 檢查
#define USART2_CR1        (*(volatile uint32_t *)(USART2_BASE + 0x0C))

void delay_dummy(int count) {
    for(int i = 0; i < count; i++) { __asm("nop"); }
}

// ISR
void USART2_IRQHandler(void) {
    if (USART2_SR & (1 << 5)) {
        char c = USART2_DR;
        printf("\r\n[ISR] CAUGHT: '%c'\r\n", c);
    }
}

int main(void) {
    rcc_setup();
    uart_init();
    
    // --- [照妖鏡] 強制檢查開關狀態 ---
    uint32_t cr1 = USART2_CR1;
    uint32_t iser = NVIC_ISER1;

    printf("\r\n=== DIAGNOSTIC START ===\r\n");
    
    // 檢查 1: UART 硬體開關 (Bit 5)
    if (cr1 & (1 << 5)) {
        printf("UART RXNEIE: [OK] (ON)\r\n");
    } else {
        printf("UART RXNEIE: [FAIL] (OFF) -> Check uart.c!\r\n");
    }

    // 檢查 2: CPU 總機開關 (Bit 6)
    if (iser & (1 << 6)) {
        printf("NVIC ISER1 : [OK] (ON)\r\n");
    } else {
        printf("NVIC ISER1 : [FAIL] (OFF) -> Check uart.c/nvic.h!\r\n");
    }
    printf("========================\r\n");

    int counter = 0;
    while(1) {
        printf("Main: %d ", counter++);
        fflush(stdout);
        delay_dummy(5000000);
    }
}