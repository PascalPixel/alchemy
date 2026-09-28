/* resource_65b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/ENTRY.o"(.text)
        "*/resource_65b_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/IMPORT.o"(.text)
        "*/resource_65b_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
