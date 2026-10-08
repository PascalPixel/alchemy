SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/IMPORT.o"(.text)
        "*/resource_376_overlay.o"(.rodata.x020092fc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
