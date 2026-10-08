SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/TOREBI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/IMPORT.o"(.text)
        "*/resource_3ae_overlay.o"(.rodata.x020096b8)
    }
    .bss : { "*/resource_3ae_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
