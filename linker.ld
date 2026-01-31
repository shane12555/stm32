MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* 已初始化的變數 (要從 Flash 搬過來) */
    .data :
    {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH

    _sidata = LOADADDR(.data);

    /* --- 新增：未初始化的變數 (要清零) --- */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* [清潔起點] Start of BSS */
        *(.bss)            /* 標準未初始化變數 */
        *(COMMON)          /* 這是 C 語言的一種特殊全域變數 */
        . = ALIGN(4);
        _ebss = .;         /* [清潔終點] End of BSS */
    } > SRAM

    _estack = ORIGIN(SRAM) + LENGTH(SRAM);
}