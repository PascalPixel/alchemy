SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65d/entry.o"(.text)
        "*/games/tla/src/field/overlay_65d/scene_tables.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008040)
        "*/games/tla/src/field/overlay_65d/exits.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008070)
        "*/resource_65d_overlay.o"(.text.x0200809c)
        "*/games/tla/src/field/overlay_65d/events_a.o"(.text)
        "*/resource_65d_overlay.o"(.text.x0200815c)
        "*/games/tla/src/field/overlay_65d/events_b.o"(.text)
        "*/resource_65d_overlay.o"(.text.x0200830c)
        "*/resource_65d_overlay.o"(.text.x02008438)
        "*/games/tla/src/field/overlay_65d/events_c.o"(.text)
        "*/games/tla/src/field/overlay_65d/import.o"(.text)
        "*/resource_65d_overlay.o"(.rodata.x020086f8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
