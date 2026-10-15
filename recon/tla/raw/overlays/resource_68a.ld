SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/ancient_lemuria/entry.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/start.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/scene_tables.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/map.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008110)
        "*/games/tla/src/scene/ancient_lemuria/copies.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008468)
        "*/games/tla/src/scene/ancient_lemuria/s0534.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/scene_events.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/s0630.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/s0830.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/s0838.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/import.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
