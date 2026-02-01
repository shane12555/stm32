#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

// NVIC (巢狀向量中斷控制器) 基底地址
#define NVIC_BASE      0xE000E100

// ISER (Interrupt Set-Enable Registers) - 用來開啟中斷
// 偏移量 0x00
#define NVIC_ISER0     (*(volatile uint32_t *)(NVIC_BASE + 0x00))
#define NVIC_ISER1     (*(volatile uint32_t *)(NVIC_BASE + 0x04))

// 查表：STM32F411 的 USART2 中斷編號 (IRQ Number) 是 38
// 38 超過了 32，所以它在 ISER1 (32~63) 裡面
// 38 - 32 = 6。所以我們要設定 ISER1 的第 6 bit。

#endif