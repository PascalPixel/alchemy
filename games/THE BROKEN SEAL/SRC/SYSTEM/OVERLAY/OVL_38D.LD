SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/KYUDEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/IMPORT.o"(.text)
        "*/resource_38d_overlay.o"(.rodata.x0200a5c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
