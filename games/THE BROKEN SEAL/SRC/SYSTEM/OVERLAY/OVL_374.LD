SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IMPORT.o"(.text)
        "*/resource_374_overlay.o"(.rodata.x0200a854)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
