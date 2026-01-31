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

    /* 關鍵：VMA 在 SRAM，LMA 在 FLASH */
    .data :
    {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH

    /* 記住搬運的來源地址 */
    _sidata = LOADADDR(.data);

    _estack = ORIGIN(SRAM) + LENGTH(SRAM);
}