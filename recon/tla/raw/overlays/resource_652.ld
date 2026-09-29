/* resource_652: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/ENTRY.o"(.text)
        "*/resource_652_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/LINES.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008218)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/IMPORT.o"(.text)
        "*/resource_652_overlay.o"(.rodata.x02008e04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
