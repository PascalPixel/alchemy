SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65c/entry.o"(.text)
        "*/games/tla/src/field/overlay_65c/scene_tables.o"(.text)
        "*/resource_65c_overlay.o"(.text.x02008054)
        "*/games/tla/src/field/overlay_65c/scene_events.o"(.text)
        "*/resource_65c_overlay.o"(.text.x02008074)
        "*/resource_65c_overlay.o"(.text.x020080fc)
        "*/games/tla/src/field/overlay_65c/prepare.o"(.text)
        "*/games/tla/src/field/overlay_65c/import.o"(.text)
        "*/resource_65c_overlay.o"(.rodata.x020081b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
