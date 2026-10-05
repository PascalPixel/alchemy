SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/IMPORT.o"(.text)
        "*/resource_3c1_overlay.o"(.rodata.x020082f0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
