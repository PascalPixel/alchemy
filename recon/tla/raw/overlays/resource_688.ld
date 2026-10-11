SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_688/entry.o"(.text)
        "*/games/tla/src/field/overlay_688/events_a.o"(.text)
        "*/games/tla/src/field/overlay_688/scene_tables.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008060)
        "*/resource_688_overlay.o"(.text.x020080b0)
        "*/games/tla/src/field/overlay_688/events_b.o"(.text)
        "*/games/tla/src/field/overlay_688/scene_events.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008120)
        "*/resource_688_overlay.o"(.text.x02008190)
        "*/games/tla/src/field/overlay_688/prepare.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008310)
        "*/resource_688_overlay.o"(.text.x020083cc)
        "*/games/tla/src/field/overlay_688/import.o"(.text)
        "*/games/tla/src/field/overlay_688/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
