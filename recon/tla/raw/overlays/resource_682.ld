/* resource_682: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_682/ENTRY.o"(.text)
        "*/resource_682_overlay.o"(.text.x02008038)
        "*/resource_682_overlay.o"(.text.x02008040)
        "*/resource_682_overlay.o"(.text.x02008044)
        "*/resource_682_overlay.o"(.text.x0200804c)
        "*/resource_682_overlay.o"(.text.x0200806c)
        "*/resource_682_overlay.o"(.text.x02008080)
        "*/resource_682_overlay.o"(.text.x020080fc)
        "*/resource_682_overlay.o"(.text.x0200814c)
        "*/resource_682_overlay.o"(.text.x02008178)
        "*/resource_682_overlay.o"(.text.x02008188)
        "*/resource_682_overlay.o"(.text.x02008430)
        "*/resource_682_overlay.o"(.text.x02008468)
        "*/resource_682_overlay.o"(.text.x020084b4)
        "*/resource_682_overlay.o"(.text.x02008554)
        "*/resource_682_overlay.o"(.text.x02008590)
        "*/resource_682_overlay.o"(.text.x020085dc)
        "*/resource_682_overlay.o"(.text.x0200860c)
        "*/resource_682_overlay.o"(.text.x02008638)
        "*/resource_682_overlay.o"(.text.x02008a60)
        "*/resource_682_overlay.o"(.text.x02008a80)
        "*/resource_682_overlay.o"(.text.x02008b18)
        "*/resource_682_overlay.o"(.text.x02008c04)
        "*/resource_682_overlay.o"(.text.x02008c40)
        "*/resource_682_overlay.o"(.text.x02008c90)
        "*/resource_682_overlay.o"(.text.x02008d4c)
        "*/resource_682_overlay.o"(.text.x02008e04)
        "*/resource_682_overlay.o"(.text.x02008e24)
        "*/resource_682_overlay.o"(.text.x02008e30)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_682/IMPORT.o"(.text)
        "*/resource_682_overlay.o"(.rodata.x0200abcc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
