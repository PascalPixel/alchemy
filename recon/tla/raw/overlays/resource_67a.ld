/* resource_67a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/ENTRY.o"(.text)
        "*/resource_67a_overlay.o"(.text.x02008038)
        "*/resource_67a_overlay.o"(.text.x02008040)
        "*/resource_67a_overlay.o"(.text.x02008044)
        "*/resource_67a_overlay.o"(.text.x0200804c)
        "*/resource_67a_overlay.o"(.text.x02008054)
        "*/resource_67a_overlay.o"(.text.x02008068)
        "*/resource_67a_overlay.o"(.text.x02008070)
        "*/resource_67a_overlay.o"(.text.x02008084)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/IMPORT.o"(.text)
        "*/resource_67a_overlay.o"(.rodata.x02008090)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
