/* resource_676: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_676/ENTRY.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008038)
        "*/resource_676_overlay.o"(.text.x02008044)
        "*/resource_676_overlay.o"(.text.x02008050)
        "*/resource_676_overlay.o"(.text.x02008088)
        "*/resource_676_overlay.o"(.text.x0200814c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_676/SCENE_TABLES.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008218)
        "*/resource_676_overlay.o"(.text.x02008270)
        "*/resource_676_overlay.o"(.text.x02008328)
        "*/resource_676_overlay.o"(.text.x0200836c)
        "*/resource_676_overlay.o"(.text.x0200838c)
        "*/resource_676_overlay.o"(.text.x0200841c)
        "*/resource_676_overlay.o"(.text.x0200843c)
        "*/resource_676_overlay.o"(.text.x020084cc)
        "*/resource_676_overlay.o"(.text.x020084ec)
        "*/resource_676_overlay.o"(.text.x02008544)
        "*/resource_676_overlay.o"(.text.x02008584)
        "*/resource_676_overlay.o"(.text.x02008614)
        "*/resource_676_overlay.o"(.text.x020086a4)
        "*/resource_676_overlay.o"(.text.x020086bc)
        "*/resource_676_overlay.o"(.text.x02009688)
        "*/resource_676_overlay.o"(.text.x02009730)
        "*/resource_676_overlay.o"(.text.x0200982c)
        "*/resource_676_overlay.o"(.text.x02009904)
        "*/resource_676_overlay.o"(.text.x02009944)
        "*/resource_676_overlay.o"(.text.x02009d14)
        "*/resource_676_overlay.o"(.text.x02009ec0)
        "*/resource_676_overlay.o"(.text.x0200a244)
        "*/resource_676_overlay.o"(.text.x0200a278)
        "*/resource_676_overlay.o"(.text.x0200a2b0)
        "*/resource_676_overlay.o"(.text.x0200a2f8)
        "*/resource_676_overlay.o"(.text.x0200a3c8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_676/IMPORT.o"(.text)
        "*/resource_676_overlay.o"(.rodata.x0200a5a8)
    }
    .bss : { "*/resource_676_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
