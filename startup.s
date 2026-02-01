.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler
.global Default_Handler

/* 引入 C 語言寫的 Main */
.global main

/* 引入 Linker 定義的地址 */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss
.word _estack

/* --- 1. 定義預設中斷處理函式 (Default Handler) --- */
/* 如果中斷發生但沒人處理，就會掉進這裡 (死迴圈) */
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop

/* 定義 Weak Alias：如果在 C 裡面沒寫這些函式，就自動用 Default_Handler 代替 */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak USART2_IRQHandler
.thumb_set USART2_IRQHandler, Default_Handler


/* --- 2. 完整的中斷向量表 --- */
.section .isr_vector, "a"
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack              /* 0: Stack Pointer */
    .word Reset_Handler        /* 1: Reset */
    .word NMI_Handler          /* 2: NMI */
    .word HardFault_Handler    /* 3: Hard Fault */
    .word Default_Handler      /* 4: MemManage */
    .word Default_Handler      /* 5: BusFault */
    .word Default_Handler      /* 6: UsageFault */
    .word 0                    /* 7: Reserved */
    .word 0                    /* 8: Reserved */
    .word 0                    /* 9: Reserved */
    .word 0                    /* 10: Reserved */
    .word Default_Handler      /* 11: SVCall */
    .word Default_Handler      /* 12: Debug Monitor */
    .word 0                    /* 13: Reserved */
    .word Default_Handler      /* 14: PendSV */
    .word Default_Handler      /* 15: SysTick */

    /* External Interrupts (IRQ 0 ~ ...) */
    
    /* 我們需要跳過 IRQ 0 到 IRQ 37 (WWDG ... SPI3) */
    /* 使用 .rept 重複 38 次 Default_Handler */
    .rept 38
        .word Default_Handler
    .endr

    /* IRQ 38: USART2 全局中斷 (這就是我們要的！) */
    .word USART2_IRQHandler

    /* 如果後面還有別的中斷要用，要繼續往下加... */


/* --- 3. 啟動程式碼 (跟你原本的一樣) --- */
.section .text
.type Reset_Handler, %function
Reset_Handler:
    /* 初始化 Stack Pointer */
    ldr r0, =_estack
    mov sp, r0

    /* Copy .data from Flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata

Copy_Loop:
    cmp r0, r1
    beq Zero_BSS_Init
    ldr r3, [r2], #4
    str r3, [r0], #4
    b Copy_Loop

Zero_BSS_Init:
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0

Zero_Loop:
    cmp r0, r1
    beq Jump_To_Main
    str r2, [r0], #4
    b Zero_Loop

Jump_To_Main:
    bl main

Loop_Forever:
    b Loop_Forever