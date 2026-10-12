SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6a7/entry.o"(.text)
        "*/games/tla/src/field/overlay_6a7/scene_tables.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x020081d8)
        "*/games/tla/src/field/overlay_6a7/scene_events.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x02008618)
        "*/resource_6a7_overlay.o"(.text.x02008680)
        "*/resource_6a7_overlay.o"(.text.x020086b0)
        "*/games/tla/src/field/overlay_6a7/import.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/field/overlay_6a7/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
