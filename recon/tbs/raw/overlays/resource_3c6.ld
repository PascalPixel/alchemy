/* resource_3c6: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/MACHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/IMPORT.o"(.text)
        "*/resource_3c6_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
