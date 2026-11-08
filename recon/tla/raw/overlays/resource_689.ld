SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_dock/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/s0044.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/placements.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/s006c.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/scene_events.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/s02f8.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/import.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
