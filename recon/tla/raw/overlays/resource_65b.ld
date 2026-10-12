SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65b/entry.o"(.text)
        "*/games/tla/src/field/overlay_65b/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_65b/s0054.o"(.text)
        "*/games/tla/src/field/overlay_65b/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_65b/s00c8.o"(.text)
        "*/games/tla/src/field/overlay_65b/s0224.o"(.text)
        "*/games/tla/src/field/overlay_65b/s0280.o"(.text)
        "*/games/tla/src/field/overlay_65b/prepare.o"(.text)
        "*/games/tla/src/field/overlay_65b/import.o"(.text)
        "*/games/tla/src/field/overlay_65b/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
