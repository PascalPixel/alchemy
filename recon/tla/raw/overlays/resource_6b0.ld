/* resource_6b0: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B0/ENTRY.o"(.text)
        "*/resource_6b0_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B0/IMPORT.o"(.text)
        "*/resource_6b0_overlay.o"(.text.x0200b4c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
