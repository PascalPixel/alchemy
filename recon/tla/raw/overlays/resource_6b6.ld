SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/rooms/entry.o"(.text)
        "*/games/tla/src/debug/rooms/scene_tables.o"(.text)
        "*/games/tla/src/debug/rooms/events_a.o"(.text)
        "*/games/tla/src/debug/rooms/scene_events.o"(.text)
        "*/games/tla/src/debug/rooms/events_b.o"(.text)
        "*/games/tla/src/debug/rooms/prepare.o"(.text)
        "*/games/tla/src/debug/rooms/import.o"(.text)
        "*/games/tla/src/debug/rooms/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
