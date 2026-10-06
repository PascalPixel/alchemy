SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/IMPORT.o"(.text)
        "*/resource_3c7_overlay.o"(.rodata.x02009314)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
