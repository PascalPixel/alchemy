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
        "*/resource_6b6_overlay.o"(.rodata.x020080c4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
