/* resource_695: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/ENTRY.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008038)
        "*/resource_695_overlay.o"(.text.x02008040)
        "*/resource_695_overlay.o"(.text.x02008044)
        "*/resource_695_overlay.o"(.text.x0200804c)
        "*/resource_695_overlay.o"(.text.x02008098)
        "*/resource_695_overlay.o"(.text.x020080d4)
        "*/resource_695_overlay.o"(.text.x020080f0)
        "*/resource_695_overlay.o"(.text.x02008148)
        "*/resource_695_overlay.o"(.text.x02008184)
        "*/resource_695_overlay.o"(.text.x0200830c)
        "*/resource_695_overlay.o"(.text.x0200832c)
        "*/resource_695_overlay.o"(.text.x02008350)
        "*/resource_695_overlay.o"(.text.x02008358)
        "*/resource_695_overlay.o"(.text.x02008364)
        "*/resource_695_overlay.o"(.text.x02008374)
        "*/resource_695_overlay.o"(.text.x02008384)
        "*/resource_695_overlay.o"(.text.x02008394)
        "*/resource_695_overlay.o"(.text.x0200839c)
        "*/resource_695_overlay.o"(.text.x020083cc)
        "*/resource_695_overlay.o"(.text.x02008404)
        "*/resource_695_overlay.o"(.text.x02008430)
        "*/resource_695_overlay.o"(.text.x0200883c)
        "*/resource_695_overlay.o"(.text.x0200887c)
        "*/resource_695_overlay.o"(.text.x02008a24)
        "*/resource_695_overlay.o"(.text.x02008d84)
        "*/resource_695_overlay.o"(.text.x02008e04)
        "*/resource_695_overlay.o"(.text.x02008fe8)
        "*/resource_695_overlay.o"(.text.x02009038)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/IMPORT.o"(.text)
        "*/resource_695_overlay.o"(.rodata.x02009218)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
