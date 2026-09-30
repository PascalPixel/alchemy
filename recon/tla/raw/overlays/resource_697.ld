/* resource_697: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_697/ENTRY.o"(.text)
        "*/resource_697_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_697/BALANCE.o"(.text)
        "*/resource_697_overlay.o"(.text.x0200cddc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_697/IMPORT.o"(.text)
        "*/resource_697_overlay.o"(.rodata.x0200da70)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
