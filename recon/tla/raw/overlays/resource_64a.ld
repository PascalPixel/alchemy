SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/clear/entry.o"(.text)
        "*/games/tla/src/scene/clear/scene_tables.o"(.text)
        "*/games/tla/src/scene/clear/s005c.o"(.text)
        "*/games/tla/src/scene/clear/s01f4.o"(.text)
        "*/games/tla/src/scene/clear/s02e0.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008328)
        "*/games/tla/src/scene/clear/prepare.o"(.text)
        "*/games/tla/src/scene/clear/s0a70.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008b24)
        "*/resource_64a_overlay.o"(.text.x02008db4)
        "*/resource_64a_overlay.o"(.text.x02008e44)
        "*/games/tla/src/scene/clear/s1804.o"(.text)
        "*/games/tla/src/scene/clear/s187c.o"(.text)
        "*/resource_64a_overlay.o"(.text.x020098e0)
        "*/resource_64a_overlay.o"(.text.x02009c7c)
        "*/resource_64a_overlay.o"(.text.x0200a068)
        "*/games/tla/src/scene/clear/s217c.o"(.text)
        "*/games/tla/src/scene/clear/import.o"(.text)
        "*/games/tla/src/scene/clear/data.o"(.data)
        "*/games/tla/src/scene/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/clear/data2.o"(.data)
    }
    .bss : { "*/resource_64a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
