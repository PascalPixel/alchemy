SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b9/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b9/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b9/scene_events.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008104)
        "*/games/tla/src/field/overlay_6b9/prepare.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x0200815c)
        "*/resource_6b9_overlay.o"(.text.x020081e0)
        "*/resource_6b9_overlay.o"(.text.x0200829c)
        "*/games/tla/src/field/overlay_6b9/sparks.o"(.text)
        "*/games/tla/src/field/overlay_6b9/scale.o"(.text)
        "*/games/tla/src/field/overlay_6b9/drop.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008468)
        "*/games/tla/src/field/overlay_6b9/import.o"(.text)
        "*/games/tla/src/field/overlay_6b9/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
