SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_695/entry.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_tables.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008098)
        "*/resource_695_overlay.o"(.text.x020080d4)
        "*/resource_695_overlay.o"(.text.x020080f0)
        "*/resource_695_overlay.o"(.text.x02008148)
        "*/resource_695_overlay.o"(.text.x02008184)
        "*/resource_695_overlay.o"(.text.x0200830c)
        "*/resource_695_overlay.o"(.text.x0200832c)
        "*/games/tla/src/field/overlay_695/places.o"(.text)
        "*/games/tla/src/field/overlay_695/scene_events.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008430)
        "*/resource_695_overlay.o"(.text.x0200883c)
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
