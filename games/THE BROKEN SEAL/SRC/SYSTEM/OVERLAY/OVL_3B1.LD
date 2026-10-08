SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STAGED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ROWING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/DATA.o"(.data)
    }
    .bss : { "*/resource_3b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
