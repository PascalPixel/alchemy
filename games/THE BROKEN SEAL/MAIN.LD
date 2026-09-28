OUTPUT_ARCH(arm)
ENTRY(Runtime_ClearWordIfSet)
SECTIONS
{
    .text 0x080197b4 : { *(.text) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss (NOLOAD) : { *(.bss*) *(COMMON) }
    /DISCARD/ : { *(.comment) *(.note*) }
}
