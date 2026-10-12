SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_64a/entry.o"(.text)
        "*/games/tla/src/field/overlay_64a/scene_tables.o"(.text)
        "*/resource_64a_overlay.o"(.text.x0200805c)
        "*/resource_64a_overlay.o"(.text.x0200815c)
        "*/resource_64a_overlay.o"(.text.x020081f4)
        "*/games/tla/src/field/overlay_64a/s02e0.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008328)
        "*/games/tla/src/field/overlay_64a/prepare.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008a70)
        "*/resource_64a_overlay.o"(.text.x02008b24)
        "*/resource_64a_overlay.o"(.text.x02008db4)
        "*/resource_64a_overlay.o"(.text.x02008e44)
        "*/resource_64a_overlay.o"(.text.x02009804)
        "*/resource_64a_overlay.o"(.text.x0200987c)
        "*/resource_64a_overlay.o"(.text.x020098e0)
        "*/resource_64a_overlay.o"(.text.x02009c7c)
        "*/resource_64a_overlay.o"(.text.x0200a068)
        "*/resource_64a_overlay.o"(.text.x0200a17c)
        "*/games/tla/src/field/overlay_64a/import.o"(.text)
        "*/games/tla/src/field/overlay_64a/data.o"(.data)
        "*/games/tla/src/field/overlay_64a/tilemap.o"(.rodata)
        "*/games/tla/src/field/overlay_64a/data2.o"(.data)
    }
    .bss : { "*/resource_64a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
