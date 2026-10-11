SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65b/entry.o"(.text)
        "*/games/tla/src/field/overlay_65b/scene_tables.o"(.text)
        "*/resource_65b_overlay.o"(.text.x02008054)
        "*/games/tla/src/field/overlay_65b/scene_events.o"(.text)
        "*/resource_65b_overlay.o"(.text.x020080c8)
        "*/resource_65b_overlay.o"(.text.x02008224)
        "*/games/tla/src/field/overlay_65b/s0280.o"(.text)
        "*/games/tla/src/field/overlay_65b/prepare.o"(.text)
        "*/games/tla/src/field/overlay_65b/import.o"(.text)
        "*/resource_65b_overlay.o"(.rodata.x02008348)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
