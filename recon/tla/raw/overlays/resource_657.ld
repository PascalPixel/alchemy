/* resource_657: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_657/ENTRY.o"(.text)
        "*/resource_657_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_657/IMPORT.o"(.text)
        "*/resource_657_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
