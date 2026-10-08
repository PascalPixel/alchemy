SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/DATA.o"(.data)
    }
    .bss : { "*/resource_3a4_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
