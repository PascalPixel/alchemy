/* resource_3a4: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.o"(.text)
        "*/resource_3a4_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.o"(.text)
        "*/resource_3a4_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
