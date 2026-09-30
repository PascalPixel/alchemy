/* resource_65c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65C/ENTRY.o"(.text)
        "*/resource_65c_overlay.o"(.text.x02008038)
        "*/resource_65c_overlay.o"(.text.x02008040)
        "*/resource_65c_overlay.o"(.text.x02008044)
        "*/resource_65c_overlay.o"(.text.x0200804c)
        "*/resource_65c_overlay.o"(.text.x02008054)
        "*/resource_65c_overlay.o"(.text.x0200806c)
        "*/resource_65c_overlay.o"(.text.x02008074)
        "*/resource_65c_overlay.o"(.text.x020080fc)
        "*/resource_65c_overlay.o"(.text.x0200816c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65C/IMPORT.o"(.text)
        "*/resource_65c_overlay.o"(.rodata.x020081b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
