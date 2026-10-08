SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/IMPORT.o"(.text)
        "*/resource_3a3_overlay.o"(.rodata.x02008fc8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
