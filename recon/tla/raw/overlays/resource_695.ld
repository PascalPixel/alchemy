SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_695/entry.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_695/s0098.o"(.text)
        "*/resource_695_overlay.o"(.text.x020080f0)
        "*/games/tla/src/field/overlay_695/s0148.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008184)
        "*/games/tla/src/field/overlay_695/s030c.o"(.text)
        "*/games/tla/src/field/overlay_695/places.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_events.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008430)
        "*/games/tla/src/field/overlay_695/s083c.o"(.text)
        "*/resource_695_overlay.o"(.text.x0200887c)
        "*/resource_695_overlay.o"(.text.x02008a24)
        "*/resource_695_overlay.o"(.text.x02008d84)
        "*/resource_695_overlay.o"(.text.x02008e04)
        "*/games/tla/src/field/overlay_695/events.o"(.text)
        "*/games/tla/src/field/overlay_695/import.o"(.text)
        "*/games/tla/src/field/overlay_695/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
