SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b6/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b6/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b6/events_a.o"(.text)
        "*/games/tla/src/field/overlay_6b6/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_6b6/events_b.o"(.text)
        "*/games/tla/src/field/overlay_6b6/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b6/import.o"(.text)
        "*/games/tla/src/field/overlay_6b6/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
