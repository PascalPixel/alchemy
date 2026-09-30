/* resource_66c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/ENTRY.o"(.text)
        "*/resource_66c_overlay.o"(.text.x02008038)
        "*/resource_66c_overlay.o"(.text.x02008040)
        "*/resource_66c_overlay.o"(.text.x02008044)
        "*/resource_66c_overlay.o"(.text.x0200804c)
        "*/resource_66c_overlay.o"(.text.x0200806c)
        "*/resource_66c_overlay.o"(.text.x02008074)
        "*/resource_66c_overlay.o"(.text.x02008154)
        "*/resource_66c_overlay.o"(.text.x02008184)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/IMPORT.o"(.text)
        "*/resource_66c_overlay.o"(.rodata.x02008224)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
