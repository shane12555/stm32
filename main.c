#include <stdint.h>

/* --- 1. 硬體暫存器定義 (這些你已經很熟了) --- */
#define RCC_BASE      0x40023800
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

/* --- 2. 【實驗主角】未初始化的全域變數 --- */
/* 重點解析：
   這裡我們宣告了兩個變數，但沒有給它們「= 0」或「= 100」。
   在 C 語言規格書中，這類變數屬於 .bss (Block Started by Symbol) 區段。
   編譯器會預期：「在 main 開始執行前，這塊記憶體應該要被系統清零。」
*/
int g_dirty_variable_1;
int g_dirty_variable_2;

void main(void) {
    /* --- 3. 初始化硬體 --- */
    // 開啟 GPIOA 時鐘 (為了讓 LED 亮)
    RCC_AHB1ENR |= (1 << 0);
    
    // 設定 PA5 為 Output 模式
    GPIOA_MODER &= ~(3 << 10); // 清除第 10, 11 bit
    GPIOA_MODER |= (1 << 10);  // 設定第 10 bit 為 1 (Output)

    /* --- 4. 進行實驗驗證 --- */
    /* 邏輯：如果這兩個變數真的是 0，代表環境乾淨，亮燈慶祝。
            如果裡面有任何垃圾值 (Random Noise)，代表環境髒亂，閃爍報警。 */
    
    if (g_dirty_variable_1 == 0 && g_dirty_variable_2 == 0) {
        /* [Pass] 變數是 0 -> 綠燈恆亮 */
        GPIOA_ODR |= (1 << 5);
    } else {
        /* [Fail] 變數是髒的 -> 綠燈狂閃 */
        while(1) {
            GPIOA_ODR ^= (1 << 5); // 切換 LED 狀態 (亮<->滅)
            
            // 簡單的延遲迴圈，讓人眼看得到閃爍
            for(volatile int i=0; i<100000; i++); 
        }
    }

    /* 死迴圈，防止程式跑飛 */
    while(1);
}