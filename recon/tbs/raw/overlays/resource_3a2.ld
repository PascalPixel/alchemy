SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/RAMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.o"(.text)
        "*/resource_3a2_overlay.o"(.rodata.x02009620)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
