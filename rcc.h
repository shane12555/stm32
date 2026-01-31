#ifndef RCC_H
#define RCC_H

#include <stdint.h>

/* --- 1. RCC (重置與時脈控制) 暫存器 --- */
#define RCC_BASE        0x40023800
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00)) // Clock Control Register
#define RCC_PLLCFGR     (*(volatile uint32_t *)(RCC_BASE + 0x04)) // PLL Config Register
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x08)) // Clock Configuration Register
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30)) // AHB1 Enable Register
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40)) // APB1 Enable Register

/* --- 2. Flash (快閃記憶體) 介面暫存器 --- */
#define FLASH_INTERFACE_BASE 0x40023C00
#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_INTERFACE_BASE + 0x00)) // Access Control Register

/* --- 3. PWR (電源控制) 暫存器 --- */
#define PWR_BASE        0x40007000
#define PWR_CR          (*(volatile uint32_t *)(PWR_BASE + 0x00)) // Power Control Register

/* 函式宣告 */
void rcc_setup(void);

#endif