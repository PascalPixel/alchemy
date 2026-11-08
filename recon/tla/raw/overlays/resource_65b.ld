SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_drawbridge/entry.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0054.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/scene_events.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s00c8.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0224.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0280.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/prepare.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/import.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
