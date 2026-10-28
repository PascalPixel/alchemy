SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/daila/entry.o"(.text)
        "*/games/tla/src/scene/daila/scene_tables.o"(.text)
        "*/games/tla/src/scene/daila/placements.o"(.text)
        "*/games/tla/src/scene/daila/talk.o"(.text)
        "*/games/tla/src/scene/daila/hip.o"(.text)
        "*/games/tla/src/scene/daila/s03a0.o"(.text)
        "*/games/tla/src/scene/daila/events.o"(.text)
        "*/games/tla/src/scene/daila/scene.o"(.text)
        "*/games/tla/src/scene/daila/import.o"(.text)
        "*/games/tla/src/scene/daila/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
