SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/MACHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/IMPORT.o"(.text)
        "*/resource_3c6_overlay.o"(.rodata.x02009638)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
