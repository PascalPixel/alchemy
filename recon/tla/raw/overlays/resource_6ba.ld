/* resource_6ba: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/ENTRY.o"(.text)
        "*/resource_6ba_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/IMPORT.o"(.text)
        "*/resource_6ba_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
