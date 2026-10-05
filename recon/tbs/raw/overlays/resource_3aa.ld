SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/KYUDEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/IMPORT.o"(.text)
        "*/resource_3aa_overlay.o"(.rodata.x02009b94)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
