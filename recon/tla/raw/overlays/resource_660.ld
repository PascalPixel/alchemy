/* resource_660: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/ENTRY.o"(.text)
        "*/resource_660_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/IMPORT.o"(.text)
        "*/resource_660_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
