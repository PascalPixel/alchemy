SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_689/entry.o"(.text)
        "*/games/tla/src/field/overlay_689/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_689/s0044.o"(.text)
        "*/games/tla/src/field/overlay_689/places.o"(.text)
        "*/games/tla/src/field/overlay_689/s006c.o"(.text)
        "*/games/tla/src/field/overlay_689/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_689/s02f8.o"(.text)
        "*/games/tla/src/field/overlay_689/import.o"(.text)
        "*/games/tla/src/field/overlay_689/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
