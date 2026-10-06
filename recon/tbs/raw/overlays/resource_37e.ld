SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/SCENE_37E.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/IMPORT.o"(.text)
        "*/resource_37e_overlay.o"(.rodata.x020080c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
