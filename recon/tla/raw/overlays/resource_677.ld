/* resource_677: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_677/ENTRY.o"(.text)
        "*/resource_677_overlay.o"(.text.x02008038)
        "*/resource_677_overlay.o"(.text.x02008040)
        "*/resource_677_overlay.o"(.text.x02008044)
        "*/resource_677_overlay.o"(.text.x0200804c)
        "*/resource_677_overlay.o"(.text.x0200808c)
        "*/resource_677_overlay.o"(.text.x020080cc)
        "*/resource_677_overlay.o"(.text.x0200810c)
        "*/resource_677_overlay.o"(.text.x0200814c)
        "*/resource_677_overlay.o"(.text.x0200818c)
        "*/resource_677_overlay.o"(.text.x020081b4)
        "*/resource_677_overlay.o"(.text.x020081dc)
        "*/resource_677_overlay.o"(.text.x02008204)
        "*/resource_677_overlay.o"(.text.x02008244)
        "*/resource_677_overlay.o"(.text.x0200829c)
        "*/resource_677_overlay.o"(.text.x020082fc)
        "*/resource_677_overlay.o"(.text.x0200835c)
        "*/resource_677_overlay.o"(.text.x020083bc)
        "*/resource_677_overlay.o"(.text.x02008424)
        "*/resource_677_overlay.o"(.text.x02008480)
        "*/resource_677_overlay.o"(.text.x020084e0)
        "*/resource_677_overlay.o"(.text.x02008524)
        "*/resource_677_overlay.o"(.text.x0200877c)
        "*/resource_677_overlay.o"(.text.x0200886c)
        "*/resource_677_overlay.o"(.text.x0200895c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_677/IMPORT.o"(.text)
        "*/resource_677_overlay.o"(.rodata.x02008a98)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
