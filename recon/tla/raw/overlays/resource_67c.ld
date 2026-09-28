/* resource_67c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67C/ENTRY.o"(.text)
        "*/resource_67c_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67C/IMPORT.o"(.text)
        "*/resource_67c_overlay.o"(.text.x0200bf04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
