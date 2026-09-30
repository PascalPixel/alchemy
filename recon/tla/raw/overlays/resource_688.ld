/* resource_688: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_688/ENTRY.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008038)
        "*/resource_688_overlay.o"(.text.x02008044)
        "*/resource_688_overlay.o"(.text.x0200804c)
        "*/resource_688_overlay.o"(.text.x02008050)
        "*/resource_688_overlay.o"(.text.x02008058)
        "*/resource_688_overlay.o"(.text.x02008060)
        "*/resource_688_overlay.o"(.text.x020080b0)
        "*/resource_688_overlay.o"(.text.x02008100)
        "*/resource_688_overlay.o"(.text.x02008118)
        "*/resource_688_overlay.o"(.text.x02008120)
        "*/resource_688_overlay.o"(.text.x02008190)
        "*/resource_688_overlay.o"(.text.x0200830c)
        "*/resource_688_overlay.o"(.text.x02008310)
        "*/resource_688_overlay.o"(.text.x020083cc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_688/IMPORT.o"(.text)
        "*/resource_688_overlay.o"(.rodata.x020085e4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
