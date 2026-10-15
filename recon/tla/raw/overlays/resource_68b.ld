SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68b/entry.o"(.text)
        "*/games/tla/src/field/overlay_68b/distance.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008070)
        "*/resource_68b_overlay.o"(.text.x020080e8)
        "*/games/tla/src/field/overlay_68b/scene_tables.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008308)
        "*/resource_68b_overlay.o"(.text.x02008398)
        "*/games/tla/src/field/overlay_68b/events_a.o"(.text)
        "*/games/tla/src/field/overlay_68b/s0538.o"(.text)
        "*/games/tla/src/field/overlay_68b/events_b.o"(.text)
        "*/resource_68b_overlay.o"(.text.x0200862c)
        "*/games/tla/src/field/overlay_68b/s08e8.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008934)
        "*/games/tla/src/field/overlay_68b/scene_events.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008a6c)
        "*/games/tla/src/field/overlay_68b/s0ad8.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008b28)
        "*/games/tla/src/field/overlay_68b/s0bbc.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008bdc)
        "*/games/tla/src/field/overlay_68b/prepare.o"(.text)
        "*/games/tla/src/field/overlay_68b/s0e7c.o"(.text)
        "*/games/tla/src/field/overlay_68b/s1160.o"(.text)
        "*/games/tla/src/field/overlay_68b/s1b3c.o"(.text)
        "*/games/tla/src/field/overlay_68b/import.o"(.text)
        "*/games/tla/src/field/overlay_68b/data.o"(.data)
    }
    .bss : { "*/resource_68b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
