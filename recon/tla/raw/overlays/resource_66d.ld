SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_66d/entry.o"(.text)
        "*/games/tla/src/field/overlay_66d/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_66d/events_a.o"(.text)
        "*/resource_66d_overlay.o"(.text.x02008368)
        "*/games/tla/src/field/overlay_66d/prepare.o"(.text)
        "*/games/tla/src/field/overlay_66d/import.o"(.text)
        "*/games/tla/src/field/overlay_66d/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
