SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_66c/entry.o"(.text)
        "*/games/tla/src/field/overlay_66c/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_66c/placements.o"(.text)
        "*/games/tla/src/field/overlay_66c/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_66c/warriors.o"(.text)
        "*/games/tla/src/field/overlay_66c/import.o"(.text)
        "*/games/tla/src/field/overlay_66c/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
