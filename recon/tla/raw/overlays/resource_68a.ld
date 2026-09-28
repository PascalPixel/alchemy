/* resource_68a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/ENTRY.o"(.text)
        "*/resource_68a_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68A/IMPORT.o"(.text)
        "*/resource_68a_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
