SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_67a/entry.o"(.text)
        "*/games/tla/src/field/overlay_67a/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_67a/s0054.o"(.text)
        "*/games/tla/src/field/overlay_67a/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_67a/s0070.o"(.text)
        "*/games/tla/src/field/overlay_67a/prepare.o"(.text)
        "*/games/tla/src/field/overlay_67a/import.o"(.text)
        "*/games/tla/src/field/overlay_67a/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
