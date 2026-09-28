/* resource_675: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/ENTRY.o"(.text)
        "*/resource_675_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_675/IMPORT.o"(.text)
        "*/resource_675_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
