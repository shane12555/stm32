#include <stdint.h>
#include "rcc.h"
#include "uart.h" 

/* 輔助函式：發送整串字串 */
void uart_print(char *str) {
    while (*str) {             // 當指標指到的字不是 '\0' (字串結尾)
        uart_write(*str);      // 傳送當前字元
        str++;                 // 指標往後移一格
    }
}

int main(void) {
    /* 1. 系統加速 (100MHz) */
    rcc_setup();

    /* 2. 初始化 UART (開啟電源、設定腳位、Baudrate) */
    uart_init();

    /* 3. 無窮迴圈：每隔一秒打一次招呼 */
    while(1) {
        uart_print("Hello World from STM32!\r\n"); // \r\n 是換行
        
        // 簡單延遲 (讓訊息不要刷太快)
        for(volatile int i=0; i<10000000; i++); 
    }
}