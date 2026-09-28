/* resource_65e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/ENTRY.o"(.text)
        "*/resource_65e_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/IMPORT.o"(.text)
        "*/resource_65e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
