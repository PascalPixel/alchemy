SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_palace/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s00c8.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/scene_events.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020080f4)
        "*/games/tla/src/scene/lemuria_palace/prepare.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020081d8)
        "*/resource_68d_overlay.o"(.text.x020082b4)
        "*/resource_68d_overlay.o"(.text.x020083a0)
        "*/games/tla/src/scene/lemuria_palace/s04e8.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/blend_fade.o"(.text)
        "*/resource_68d_overlay.o"(.text.x0200858c)
        "*/games/tla/src/scene/lemuria_palace/import.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/data.o"(.data)
        "*/games/tla/src/scene/lemuria_palace/tiles.o"(.rodata)
        "*/games/tla/src/scene/lemuria_palace/data2.o"(.data)
    }
    .bss : { "*/resource_68d_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
