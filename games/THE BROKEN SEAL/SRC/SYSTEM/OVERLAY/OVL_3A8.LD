SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/MACHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/IMPORT.o"(.text)
        "*/resource_3a8_overlay.o"(.rodata.x0200bd84)
    }
    .bss ALIGN(8) : { "*/resource_3a8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
