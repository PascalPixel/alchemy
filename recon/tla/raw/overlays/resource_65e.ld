SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mikasalla_interiors/entry.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events3.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/s0470.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events2.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/import.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
