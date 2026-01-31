#include "uart.h"
#include "rcc.h" // 我們需要 RCC_APB1ENR 和 RCC_AHB1ENR

void uart_init(void) {
    /* --- 1. 開啟時脈 (Power On) --- */
    /* 就像家裡的電器，沒插電是動不了的 */
    
    // 1.1 開啟 GPIOA 時脈 (因為 PA2, PA3 在 GPIOA)
    // 參考 rcc.h: #define RCC_AHB1ENR ...
    RCC_AHB1ENR |= (1 << 0); // Bit 0 = GPIOA EN

    // 1.2 開啟 USART2 時脈 (這是 UART 硬體模組)
    // 查 Datasheet: USART2 掛在 APB1 匯流排的 Bit 17
    RCC_APB1ENR |= (1 << 17); // Bit 17 = USART2 EN


    /* --- 2. 配置 GPIO (把 PA2, PA3 變成 UART 腳) --- */
    
    // 2.1 設定模式為 "Alternate Function" (替代功能模式)
    // PA2 (TX): Pin 2 (Bits 4,5) -> 設為 10 (AF)
    // PA3 (RX): Pin 3 (Bits 6,7) -> 設為 10 (AF)
    GPIOA_MODER &= ~((3 << 4) | (3 << 6)); // 先清除舊設定
    GPIOA_MODER |=  ((2 << 4) | (2 << 6)); // 設定為 10 (Alternate Function)

    // 2.2 設定替代功能為 AF07 (查 Datasheet 知道 USART2 是 AF7)
    // AFRL 暫存器控制 Pin 0~7。
    // Pin 2 在 AFRL 的 Bits 8-11
    // Pin 3 在 AFRL 的 Bits 12-15
    // 我們要填入 7 (二進位 0111)
    GPIOA_AFRL &= ~((0xF << 8) | (0xF << 12)); // 先清除
    GPIOA_AFRL |=  ((7   << 8) | (7   << 12)); // 填入 7


    /* --- 3. 設定波特率 (Baud Rate) --- */
    // 目標: 115200 bps
    // 來源時脈: APB1 = 50 MHz (我們在 rcc.c 設了除以2)
    // 公式: BRR = APB1_Clock / BaudRate
    // 計算: 50,000,000 / 115200 = 434.027...
    // 結果: 填入 434 即可
    USART2->BRR = 434; 


    /* --- 4. 啟用 UART 模組 --- */
    // CR1 (Control Register 1)
    // Bit 3 (TE): Transmitter Enable (開啟傳送)
    // Bit 2 (RE): Receiver Enable (開啟接收)
    // Bit 13 (UE): USART Enable (開啟總開關)
    USART2->CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void uart_write(int ch) {
    /* 等待傳送緩衝區變空 (Transmit Data Register Empty - TXE) */
    // SR (Status Register) 的 Bit 7 是 TXE
    // 如果是 0，代表上一筆資料還沒傳完，要等它。
    while (!(USART2->SR & (1 << 7))); 

    /* 把資料寫入 Data Register (DR) */
    USART2->DR = (ch & 0xFF);
}

int uart_read(void) {
    /* 等待收到資料 (Read Data Register Not Empty - RXNE) */
    // SR 的 Bit 5 是 RXNE
    // 如果是 0，代表還沒收到資料，就一直等。
    while (!(USART2->SR & (1 << 5)));

    /* 讀取 Data Register (DR) */
    return USART2->DR;
}