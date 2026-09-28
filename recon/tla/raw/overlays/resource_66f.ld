/* resource_66f: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/ENTRY.o"(.text)
        "*/resource_66f_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/IMPORT.o"(.text)
        "*/resource_66f_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
