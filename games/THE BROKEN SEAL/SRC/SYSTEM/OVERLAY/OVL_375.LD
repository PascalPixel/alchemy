SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/SUKURETA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/IMPORT.o"(.text)
        "*/resource_375_overlay.o"(.rodata.x02009bdc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
