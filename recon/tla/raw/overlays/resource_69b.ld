/* resource_69b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69B/ENTRY.o"(.text)
        "*/resource_69b_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_69B/IMPORT.o"(.text)
        "*/resource_69b_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
