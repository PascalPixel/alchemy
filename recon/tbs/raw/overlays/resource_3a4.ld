SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SWING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ROCKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/RESTORE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.o"(.text)
        "*/resource_3a4_overlay.o"(.rodata.x0200bd28)
    }
    .bss : { "*/resource_3a4_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
