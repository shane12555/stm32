#include <stdio.h>
#include "rcc.h"
#include "uart.h"

int main(void) {
    /* 1. 啟動原子鐘 (100MHz, HSE) */
    rcc_setup();
    
    /* 2. 初始化 UART (115200) */
    uart_init();

    /* 3. [關鍵] 暖機延遲 (防止開機第一句被吃掉) */
    // 讓 USB 轉接晶片有一點時間準備
    for(int i = 0; i < 2000000; i++) {
        __asm("nop");
    }

    /* 4. 開機畫面 */
    printf("\r\n\r\n"); // 清空
    printf("=============================================\r\n");
    printf("   Chatbot System Online (HSE 100MHz)        \r\n");
    printf("   Connection: Stable                        \r\n");
    printf("   Waiting for input...                      \r\n");
    printf("=============================================\r\n");

    char c;
    while(1) {
        // 1. 等待輸入
        c = uart_read(); 

        // 2. 機器人回覆 (這裡證明是雙向溝通)
        printf("[STM32] I received: '%c'\r\n", c);
    }
}