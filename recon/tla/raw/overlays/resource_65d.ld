SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mikasalla/entry.o"(.text)
        "*/games/tla/src/scene/mikasalla/scene_tables.o"(.text)
        "*/games/tla/src/scene/mikasalla/exits.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_a.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_b.o"(.text)
        "*/games/tla/src/scene/mikasalla/s0438.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_c.o"(.text)
        "*/games/tla/src/scene/mikasalla/import.o"(.text)
        "*/games/tla/src/scene/mikasalla/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
