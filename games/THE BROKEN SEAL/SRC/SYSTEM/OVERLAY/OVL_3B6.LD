SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/IMPORT.o"(.text)
        "*/resource_3b6_overlay.o"(.rodata.x02008a90)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
