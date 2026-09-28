/* resource_3c1: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3c1_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/TABLES.o"(.text)
        "*/resource_3c1_overlay.o"(.text.x02008068)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/ENTRY_CUE.o"(.text)
        "*/resource_3c1_overlay.o"(.text.x02008288)
        "*/resource_3c1_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
