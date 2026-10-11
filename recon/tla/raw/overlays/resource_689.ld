SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_689/entry.o"(.text)
        "*/games/tla/src/field/overlay_689/scene_tables.o"(.text)
        "*/resource_689_overlay.o"(.text.x02008044)
        "*/games/tla/src/field/overlay_689/places.o"(.text)
        "*/resource_689_overlay.o"(.text.x0200806c)
        "*/resource_689_overlay.o"(.text.x02008158)
        "*/resource_689_overlay.o"(.text.x02008254)
        "*/resource_689_overlay.o"(.text.x020082dc)
        "*/games/tla/src/field/overlay_689/scene_events.o"(.text)
        "*/resource_689_overlay.o"(.text.x020082f8)
        "*/resource_689_overlay.o"(.text.x02008360)
        "*/resource_689_overlay.o"(.text.x020083cc)
        "*/games/tla/src/field/overlay_689/import.o"(.text)
        "*/games/tla/src/field/overlay_689/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
