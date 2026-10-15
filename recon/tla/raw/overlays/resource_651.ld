SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/daila/entry.o"(.text)
        "*/games/tla/src/field/daila/scene_tables.o"(.text)
        "*/games/tla/src/field/daila/places.o"(.text)
        "*/games/tla/src/field/daila/talk.o"(.text)
        "*/games/tla/src/field/daila/hip.o"(.text)
        "*/games/tla/src/field/daila/s03a0.o"(.text)
        "*/games/tla/src/field/daila/events.o"(.text)
        "*/games/tla/src/field/daila/scene.o"(.text)
        "*/games/tla/src/field/daila/import.o"(.text)
        "*/games/tla/src/field/daila/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
