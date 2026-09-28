/* resource_653: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_653/ENTRY.o"(.text)
        "*/resource_653_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_653/IMPORT.o"(.text)
        "*/resource_653_overlay.o"(.rodata.x0200d3e0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
