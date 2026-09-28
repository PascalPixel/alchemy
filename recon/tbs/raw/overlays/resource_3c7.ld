/* resource_3c7: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ENTRY.o"(.text)
        "*/resource_3c7_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/IMPORT.o"(.text)
        "*/resource_3c7_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
