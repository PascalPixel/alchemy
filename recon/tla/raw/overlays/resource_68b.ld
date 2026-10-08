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
        "*/resource_68b_overlay.o"(.text.x020084a8)
        "*/resource_68b_overlay.o"(.text.x02008538)
        "*/resource_68b_overlay.o"(.text.x02008560)
        "*/games/tla/src/field/overlay_68b/events_b.o"(.text)
        "*/resource_68b_overlay.o"(.text.x0200862c)
        "*/resource_68b_overlay.o"(.text.x020088e8)
        "*/resource_68b_overlay.o"(.text.x02008934)
        "*/games/tla/src/field/overlay_68b/scene_events.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008a6c)
        "*/resource_68b_overlay.o"(.text.x02008ad8)
        "*/resource_68b_overlay.o"(.text.x02008b28)
        "*/resource_68b_overlay.o"(.text.x02008bbc)
        "*/resource_68b_overlay.o"(.text.x02008bdc)
        "*/games/tla/src/field/overlay_68b/prepare.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008e7c)
        "*/resource_68b_overlay.o"(.text.x0200909c)
        "*/resource_68b_overlay.o"(.text.x02009160)
        "*/resource_68b_overlay.o"(.text.x02009b3c)
        "*/games/tla/src/field/overlay_68b/import.o"(.text)
        "*/resource_68b_overlay.o"(.rodata.x02009d84)
    }
    .bss : { "*/resource_68b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
