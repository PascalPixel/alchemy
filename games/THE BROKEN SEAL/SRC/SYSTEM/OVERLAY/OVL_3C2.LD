SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_HEYA/IMPORT.o"(.text)
        "*/resource_3c2_overlay.o"(.rodata.x02008c3c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
