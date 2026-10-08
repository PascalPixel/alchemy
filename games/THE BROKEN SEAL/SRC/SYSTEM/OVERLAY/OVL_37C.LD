SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/TABLE_UPDATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/IMPORT.o"(.text)
        "*/resource_37c_overlay.o"(.rodata.x020080c8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
