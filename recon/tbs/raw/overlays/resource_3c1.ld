/* resource_3c1: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/IMPORT.o"(.text)
        "*/resource_3c1_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
