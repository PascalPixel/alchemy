/* resource_69c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69C/ENTRY.o"(.text)
        "*/resource_69c_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69C/IMPORT.o"(.text)
        "*/resource_69c_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
