/* resource_6ad: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6AD/ENTRY.o"(.text)
        "*/resource_6ad_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6AD/IMPORT.o"(.text)
        "*/resource_6ad_overlay.o"(.rodata.x0200d6c8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
