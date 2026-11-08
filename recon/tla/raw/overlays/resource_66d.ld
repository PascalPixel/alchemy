SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/naribwe/entry.o"(.text)
        "*/games/tla/src/scene/naribwe/scene_tables.o"(.text)
        "*/games/tla/src/scene/naribwe/events_a.o"(.text)
        "*/games/tla/src/scene/naribwe/prepare.o"(.text)
        "*/games/tla/src/scene/naribwe/import.o"(.text)
        "*/games/tla/src/scene/naribwe/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
