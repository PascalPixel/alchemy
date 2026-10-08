SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_MEIRO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_MEIRO/SCENE_37D.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_MEIRO/IMPORT.o"(.text)
        "*/resource_37d_overlay.o"(.rodata.x020080c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
