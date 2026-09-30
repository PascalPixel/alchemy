/* resource_66d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/ENTRY.o"(.text)
        "*/resource_66d_overlay.o"(.text.x02008038)
        "*/resource_66d_overlay.o"(.text.x02008040)
        "*/resource_66d_overlay.o"(.text.x02008044)
        "*/resource_66d_overlay.o"(.text.x0200804c)
        "*/resource_66d_overlay.o"(.text.x02008088)
        "*/resource_66d_overlay.o"(.text.x020080b8)
        "*/resource_66d_overlay.o"(.text.x020080d8)
        "*/resource_66d_overlay.o"(.text.x02008140)
        "*/resource_66d_overlay.o"(.text.x020081a8)
        "*/resource_66d_overlay.o"(.text.x020081f0)
        "*/resource_66d_overlay.o"(.text.x02008250)
        "*/resource_66d_overlay.o"(.text.x020082b0)
        "*/resource_66d_overlay.o"(.text.x0200830c)
        "*/resource_66d_overlay.o"(.text.x02008368)
        "*/resource_66d_overlay.o"(.text.x02008400)
        "*/resource_66d_overlay.o"(.text.x0200841c)
        "*/resource_66d_overlay.o"(.text.x02008448)
        "*/resource_66d_overlay.o"(.text.x02008464)
        "*/resource_66d_overlay.o"(.text.x02008480)
        "*/resource_66d_overlay.o"(.text.x02008520)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/IMPORT.o"(.text)
        "*/resource_66d_overlay.o"(.rodata.x020085fc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
