/* resource_662: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_662/ENTRY.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008038)
        "*/resource_662_overlay.o"(.text.x0200818c)
        "*/resource_662_overlay.o"(.text.x020081d4)
        "*/resource_662_overlay.o"(.text.x0200833c)
        "*/resource_662_overlay.o"(.text.x02008458)
        "*/resource_662_overlay.o"(.text.x02008754)
        "*/resource_662_overlay.o"(.text.x020087dc)
        "*/resource_662_overlay.o"(.text.x02008898)
        "*/resource_662_overlay.o"(.text.x020088fc)
        "*/resource_662_overlay.o"(.text.x0200891c)
        "*/resource_662_overlay.o"(.text.x02008978)
        "*/resource_662_overlay.o"(.text.x0200899c)
        "*/resource_662_overlay.o"(.text.x02008acc)
        "*/resource_662_overlay.o"(.text.x02008b54)
        "*/resource_662_overlay.o"(.text.x02008bc0)
        "*/resource_662_overlay.o"(.text.x02008c44)
        "*/resource_662_overlay.o"(.text.x02008cec)
        "*/resource_662_overlay.o"(.text.x02008d04)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_662/SCENE_TABLES.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008d44)
        "*/resource_662_overlay.o"(.text.x02008d84)
        "*/resource_662_overlay.o"(.text.x02008dac)
        "*/resource_662_overlay.o"(.text.x02008de0)
        "*/resource_662_overlay.o"(.text.x02008e14)
        "*/resource_662_overlay.o"(.text.x02008e3c)
        "*/resource_662_overlay.o"(.text.x02008e68)
        "*/resource_662_overlay.o"(.text.x02008f10)
        "*/resource_662_overlay.o"(.text.x02009040)
        "*/resource_662_overlay.o"(.text.x02009120)
        "*/resource_662_overlay.o"(.text.x020091b4)
        "*/resource_662_overlay.o"(.text.x020091e0)
        "*/resource_662_overlay.o"(.text.x02009238)
        "*/resource_662_overlay.o"(.text.x0200930c)
        "*/resource_662_overlay.o"(.text.x020093c8)
        "*/resource_662_overlay.o"(.text.x020093d8)
        "*/resource_662_overlay.o"(.text.x02009418)
        "*/resource_662_overlay.o"(.text.x020097dc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_662/IMPORT.o"(.text)
        "*/resource_662_overlay.o"(.rodata.x02009968)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
