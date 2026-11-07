SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/clear/entry.o"(.text)
        "*/games/tla/src/scene/clear/scene_tables.o"(.text)
        "*/games/tla/src/scene/clear/s005c.o"(.text)
        "*/games/tla/src/scene/clear/s01f4.o"(.text)
        "*/games/tla/src/scene/clear/s02e0.o"(.text)
        "*/games/tla/src/scene/clear/s0328.o"(.text)
        "*/games/tla/src/scene/clear/prepare.o"(.text)
        "*/games/tla/src/scene/clear/s0a70.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008b24)
        "*/games/tla/src/scene/clear/s0db4.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008e44)
        "*/games/tla/src/scene/clear/s0e44.o"(.text)
        "*/games/tla/src/scene/clear/s1804.o"(.text)
        "*/games/tla/src/scene/clear/s187c.o"(.text)
        "*/games/tla/src/scene/clear/s18e0.o"(.text)
        "*/games/tla/src/scene/clear/s1c7c.o"(.text)
        "*/games/tla/src/scene/clear/s2068.o"(.text)
        "*/games/tla/src/scene/clear/s217c.o"(.text)
        "*/games/tla/src/scene/clear/import.o"(.text)
        "*/games/tla/src/scene/clear/data.o"(.data)
        "*/games/tla/src/scene/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/clear/data2.o"(.data)
    }
    .bss : { "*/resource_64a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
