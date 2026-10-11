SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b9/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b9/scene_tables.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008054)
        "*/resource_6b9_overlay.o"(.text.x02008064)
        "*/resource_6b9_overlay.o"(.text.x0200807c)
        "*/resource_6b9_overlay.o"(.text.x02008088)
        "*/resource_6b9_overlay.o"(.text.x020080b8)
        "*/resource_6b9_overlay.o"(.text.x020080d8)
        "*/games/tla/src/field/overlay_6b9/scene_events.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008104)
        "*/games/tla/src/field/overlay_6b9/prepare.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x0200815c)
        "*/resource_6b9_overlay.o"(.text.x020081e0)
        "*/resource_6b9_overlay.o"(.text.x0200829c)
        "*/resource_6b9_overlay.o"(.text.x02008358)
        "*/games/tla/src/field/overlay_6b9/scale.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008468)
        "*/games/tla/src/field/overlay_6b9/import.o"(.text)
        "*/resource_6b9_overlay.o"(.rodata.x02008608)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
