SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_67e/entry.o"(.text)
        "*/games/tla/src/field/overlay_67e/s0038.o"(.text)
        "*/resource_67e_overlay.o"(.text.x02008048)
        "*/games/tla/src/field/overlay_67e/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_67e/events.o"(.text)
        "*/resource_67e_overlay.o"(.text.x020084bc)
        "*/resource_67e_overlay.o"(.text.x02008538)
        "*/games/tla/src/field/overlay_67e/s05b4.o"(.text)
        "*/resource_67e_overlay.o"(.text.x0200860c)
        "*/resource_67e_overlay.o"(.text.x020086a4)
        "*/resource_67e_overlay.o"(.text.x02008814)
        "*/resource_67e_overlay.o"(.text.x02008954)
        "*/resource_67e_overlay.o"(.text.x020089d0)
        "*/games/tla/src/field/overlay_67e/prepare.o"(.text)
        "*/games/tla/src/field/overlay_67e/blocks.o"(.text)
        "*/games/tla/src/field/overlay_67e/import.o"(.text)
        "*/games/tla/src/field/overlay_67e/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
