SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_662/entry.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008038)
        "*/resource_662_overlay.o"(.text.x0200818c)
        "*/resource_662_overlay.o"(.text.x020081d4)
        "*/resource_662_overlay.o"(.text.x0200833c)
        "*/resource_662_overlay.o"(.text.x02008458)
        "*/resource_662_overlay.o"(.text.x02008754)
        "*/resource_662_overlay.o"(.text.x020087dc)
        "*/games/tla/src/field/overlay_662/stop.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008978)
        "*/resource_662_overlay.o"(.text.x0200899c)
        "*/resource_662_overlay.o"(.text.x02008acc)
        "*/resource_662_overlay.o"(.text.x02008b54)
        "*/resource_662_overlay.o"(.text.x02008bc0)
        "*/resource_662_overlay.o"(.text.x02008c44)
        "*/games/tla/src/field/overlay_662/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_662/events.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008e68)
        "*/resource_662_overlay.o"(.text.x02008f10)
        "*/resource_662_overlay.o"(.text.x02009040)
        "*/resource_662_overlay.o"(.text.x02009120)
        "*/games/tla/src/field/overlay_662/s11b4.o"(.text)
        "*/resource_662_overlay.o"(.text.x020091e0)
        "*/resource_662_overlay.o"(.text.x02009238)
        "*/resource_662_overlay.o"(.text.x0200930c)
        "*/games/tla/src/field/overlay_662/s13c8.o"(.text)
        "*/resource_662_overlay.o"(.text.x02009418)
        "*/games/tla/src/field/overlay_662/prepare.o"(.text)
        "*/games/tla/src/field/overlay_662/import.o"(.text)
        "*/games/tla/src/field/overlay_662/data.o"(.data)
    }
    .bss : { "*/resource_662_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
