SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_683/entry.o"(.text)
        "*/games/tla/src/field/overlay_683/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_683/prepare.o"(.text)
        "*/games/tla/src/field/overlay_683/import.o"(.text)
        "*/games/tla/src/field/overlay_683/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
