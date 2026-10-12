SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_695/entry.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_695/s0098.o"(.text)
        "*/games/tla/src/field/overlay_695/s00f0.o"(.text)
        "*/games/tla/src/field/overlay_695/s0148.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008184)
        "*/games/tla/src/field/overlay_695/s030c.o"(.text)
        "*/games/tla/src/field/overlay_695/places.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_695/s0430.o"(.text)
        "*/games/tla/src/field/overlay_695/s083c.o"(.text)
        "*/games/tla/src/field/overlay_695/s087c.o"(.text)
        "*/games/tla/src/field/overlay_695/s0a24.o"(.text)
        "*/games/tla/src/field/overlay_695/s0d84.o"(.text)
        "*/games/tla/src/field/overlay_695/s0e04.o"(.text)
        "*/games/tla/src/field/overlay_695/events.o"(.text)
        "*/games/tla/src/field/overlay_695/import.o"(.text)
        "*/games/tla/src/field/overlay_695/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
