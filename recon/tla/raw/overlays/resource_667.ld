SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_667/entry.o"(.text)
        "*/games/tla/src/field/overlay_667/scene_tables.o"(.text)
        "*/resource_667_overlay.o"(.text.x0200804c)
        "*/resource_667_overlay.o"(.text.x0200806c)
        "*/resource_667_overlay.o"(.text.x020080b8)
        "*/resource_667_overlay.o"(.text.x02008150)
        "*/resource_667_overlay.o"(.text.x0200819c)
        "*/resource_667_overlay.o"(.text.x02008238)
        "*/resource_667_overlay.o"(.text.x02008370)
        "*/games/tla/src/field/overlay_667/scene_events.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008514)
        "*/games/tla/src/field/overlay_667/prepare.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008658)
        "*/resource_667_overlay.o"(.text.x02009488)
        "*/games/tla/src/field/overlay_667/import.o"(.text)
        "*/resource_667_overlay.o"(.rodata.x02009600)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
