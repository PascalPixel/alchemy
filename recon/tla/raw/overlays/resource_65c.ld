SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65c/entry.o"(.text)
        "*/games/tla/src/field/overlay_65c/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_65c/s0054.o"(.text)
        "*/games/tla/src/field/overlay_65c/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_65c/s0074.o"(.text)
        "*/resource_65c_overlay.o"(.text.x020080fc)
        "*/games/tla/src/field/overlay_65c/prepare.o"(.text)
        "*/games/tla/src/field/overlay_65c/import.o"(.text)
        "*/games/tla/src/field/overlay_65c/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
