OUTPUT_ARCH(arm)
ENTRY(Func_080132cc)
SECTIONS
{
    .text 0x080132cc : { *(.text) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss (NOLOAD) : { *(.bss*) *(COMMON) }
    /DISCARD/ : { *(.comment) *(.note*) }
}
