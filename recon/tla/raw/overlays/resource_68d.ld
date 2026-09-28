/* resource_68d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/ENTRY.o"(.text)
        "*/resource_68d_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/IMPORT.o"(.text)
        "*/resource_68d_overlay.o"(.text.x0200a12c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
