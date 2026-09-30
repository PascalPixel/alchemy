/* resource_65b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/ENTRY.o"(.text)
        "*/resource_65b_overlay.o"(.text.x02008038)
        "*/resource_65b_overlay.o"(.text.x02008040)
        "*/resource_65b_overlay.o"(.text.x02008044)
        "*/resource_65b_overlay.o"(.text.x0200804c)
        "*/resource_65b_overlay.o"(.text.x02008054)
        "*/resource_65b_overlay.o"(.text.x020080c0)
        "*/resource_65b_overlay.o"(.text.x020080c8)
        "*/resource_65b_overlay.o"(.text.x02008224)
        "*/resource_65b_overlay.o"(.text.x02008280)
        "*/resource_65b_overlay.o"(.text.x02008294)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/IMPORT.o"(.text)
        "*/resource_65b_overlay.o"(.rodata.x02008348)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
