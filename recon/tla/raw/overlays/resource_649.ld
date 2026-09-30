/* resource_649: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/ENTRY.o"(.text)
        "*/resource_649_overlay.o"(.text.x02008038)
        "*/resource_649_overlay.o"(.text.x02008040)
        "*/resource_649_overlay.o"(.text.x02008044)
        "*/resource_649_overlay.o"(.text.x0200804c)
        "*/resource_649_overlay.o"(.text.x02008054)
        "*/resource_649_overlay.o"(.text.x0200805c)
        "*/resource_649_overlay.o"(.text.x02008204)
        "*/resource_649_overlay.o"(.text.x02008208)
        "*/resource_649_overlay.o"(.text.x02008278)
        "*/resource_649_overlay.o"(.text.x0200831c)
        "*/resource_649_overlay.o"(.text.x02008490)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/IMPORT.o"(.text)
        "*/resource_649_overlay.o"(.rodata.x0200864c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
