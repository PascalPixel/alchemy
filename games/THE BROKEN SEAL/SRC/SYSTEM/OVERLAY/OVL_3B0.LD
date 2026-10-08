SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/HOBASHIRA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/IMPORT.o"(.text)
        "*/resource_3b0_overlay.o"(.rodata.x02009314)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/HOBASHIRA.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
