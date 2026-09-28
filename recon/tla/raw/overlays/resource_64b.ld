/* resource_64b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/resource_64b_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/resource_64b_overlay.o"(.rodata.x0200ce38)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
