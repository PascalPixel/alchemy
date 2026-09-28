/* resource_69a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69A/ENTRY.o"(.text)
        "*/resource_69a_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69A/IMPORT.o"(.text)
        "*/resource_69a_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
