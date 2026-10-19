SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/menu/clear/entry.o"(.text)
        "*/games/tla/src/menu/clear/scene_tables.o"(.text)
        "*/games/tla/src/menu/clear/s005c.o"(.text)
        "*/games/tla/src/menu/clear/s01f4.o"(.text)
        "*/games/tla/src/menu/clear/s02e0.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008328)
        "*/games/tla/src/menu/clear/prepare.o"(.text)
        "*/games/tla/src/menu/clear/s0a70.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008b24)
        "*/resource_64a_overlay.o"(.text.x02008db4)
        "*/resource_64a_overlay.o"(.text.x02008e44)
        "*/resource_64a_overlay.o"(.text.x02009804)
        "*/resource_64a_overlay.o"(.text.x0200987c)
        "*/resource_64a_overlay.o"(.text.x020098e0)
        "*/resource_64a_overlay.o"(.text.x02009c7c)
        "*/resource_64a_overlay.o"(.text.x0200a068)
        "*/games/tla/src/menu/clear/s217c.o"(.text)
        "*/games/tla/src/menu/clear/import.o"(.text)
        "*/games/tla/src/menu/clear/data.o"(.data)
        "*/games/tla/src/menu/clear/tilemap.o"(.rodata)
        "*/games/tla/src/menu/clear/data2.o"(.data)
    }
    .bss : { "*/resource_64a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
