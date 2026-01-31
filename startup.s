.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler

/* 引入 Linker 定義的地址 */
.word _sidata
.word _sdata
.word _edata
.word _sbss    /* 新增：BSS 起點 */
.word _ebss    /* 新增：BSS 終點 */
.word _estack

/* 中斷向量表 */
.section .isr_vector, "a"
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack
    .word Reset_Handler

/* 啟動程式碼 */
.section .text
.type Reset_Handler, %function
Reset_Handler:
    /* 1. 初始化 Stack Pointer */
    ldr r0, =_estack
    mov sp, r0

    /* 2. 搬運工：Copy .data from Flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata

Copy_Loop:
    cmp r0, r1
    beq Zero_BSS_Init     /* 搬完了？去做清潔工作 */

    ldr r3, [r2], #4
    str r3, [r0], #4
    b Copy_Loop

    /* 3. 清潔工：Zero .bss section (新增部分) */
Zero_BSS_Init:
    ldr r0, =_sbss        /* R0 = 目標地起點 */
    ldr r1, =_ebss        /* R1 = 目標地終點 */
    mov r2, #0            /* R2 = 準備掃把 (數值 0) */

Zero_Loop:
    cmp r0, r1            /* 到終點了嗎？ */
    beq Jump_To_Main      /* 掃完了，跳去執行 main */

    str r2, [r0], #4      /* 把 0 (R2) 寫進 RAM (R0)，R0 自動往後移 */
    b Zero_Loop           /* 繼續掃下一個 */

Jump_To_Main:
    bl main

Loop_Forever:
    b Loop_Forever