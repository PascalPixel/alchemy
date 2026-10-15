SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_688/entry.o"(.text)
        "*/games/tla/src/field/overlay_688/events_a.o"(.text)
        "*/games/tla/src/field/overlay_688/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_688/events_b.o"(.text)
        "*/games/tla/src/field/overlay_688/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_688/s0120.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008190)
        "*/games/tla/src/field/overlay_688/prepare.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008310)
        "*/games/tla/src/field/overlay_688/s03cc.o"(.text)
        "*/games/tla/src/field/overlay_688/import.o"(.text)
        "*/games/tla/src/field/overlay_688/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
