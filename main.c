#include <stdio.h>
#include "rcc.h"
#include "uart.h"

int main(void) {
    // 1. 初始化
    rcc_setup();
    uart_init();

    // 2. 開機嗆聲
    printf("\r\n\r\n"); // 先換幾行，清空版面
    printf("==================================\r\n");
    printf("   Chatbot Mode Activated!        \r\n");
    printf("   Type any key to start...       \r\n");
    printf("==================================\r\n");

    char c;
    while(1) {
        // 1. 等待你按鍵盤
        c = uart_read(); 

        // 2. 這是最關鍵的一步！
        // STM32 會自己加料，證明它有在思考
        printf("STM32 says: I received '%c'\r\n", c);
    }
}