.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler

/* 引入 Linker 定義的地址 */
.word _sidata
.word _sdata
.word _edata
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
    /* A. 初始化 Stack Pointer */
    ldr r0, =_estack
    mov sp, r0

    /* B. 執行 .data 搬運 (從 Flash 到 SRAM) */
    ldr r0, =_sdata       /* 目的地開始 */
    ldr r1, =_edata       /* 目的地結束 */
    ldr r2, =_sidata      /* 來源地 (Flash) */

Copy_Loop:
    cmp r0, r1
    beq Jump_To_Main

    ldr r3, [r2], #4      /* 讀 Flash */
    str r3, [r0], #4      /* 寫 SRAM */
    b Copy_Loop

Jump_To_Main:
    bl main

Loop_Forever:
    b Loop_Forever