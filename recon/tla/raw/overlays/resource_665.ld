/* resource_665: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_665/ENTRY.o"(.text)
        "*/resource_665_overlay.o"(.text.x02008038)
        "*/resource_665_overlay.o"(.text.x02008040)
        "*/resource_665_overlay.o"(.text.x02008044)
        "*/resource_665_overlay.o"(.text.x0200804c)
        "*/resource_665_overlay.o"(.text.x02008098)
        "*/resource_665_overlay.o"(.text.x0200810c)
        "*/resource_665_overlay.o"(.text.x02008124)
        "*/resource_665_overlay.o"(.text.x02008130)
        "*/resource_665_overlay.o"(.text.x0200813c)
        "*/resource_665_overlay.o"(.text.x020081a4)
        "*/resource_665_overlay.o"(.text.x020081f0)
        "*/resource_665_overlay.o"(.text.x0200823c)
        "*/resource_665_overlay.o"(.text.x02008248)
        "*/resource_665_overlay.o"(.text.x02008254)
        "*/resource_665_overlay.o"(.text.x02008260)
        "*/resource_665_overlay.o"(.text.x0200826c)
        "*/resource_665_overlay.o"(.text.x02008288)
        "*/resource_665_overlay.o"(.text.x020082f8)
        "*/resource_665_overlay.o"(.text.x02008398)
        "*/resource_665_overlay.o"(.text.x0200847c)
        "*/resource_665_overlay.o"(.text.x0200866c)
        "*/resource_665_overlay.o"(.text.x02008760)
        "*/resource_665_overlay.o"(.text.x02008774)
        "*/resource_665_overlay.o"(.text.x02008788)
        "*/resource_665_overlay.o"(.text.x020087d4)
        "*/resource_665_overlay.o"(.text.x02008a18)
        "*/resource_665_overlay.o"(.text.x02008a98)
        "*/resource_665_overlay.o"(.text.x02008aa0)
        "*/resource_665_overlay.o"(.text.x02009958)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_665/IMPORT.o"(.text)
        "*/resource_665_overlay.o"(.rodata.x02009d54)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
