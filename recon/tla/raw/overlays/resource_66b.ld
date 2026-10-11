SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_66b/entry.o"(.text)
        "*/games/tla/src/field/overlay_66b/s0038.o"(.text)
        "*/games/tla/src/field/overlay_66b/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_66b/s009c.o"(.text)
        "*/games/tla/src/field/common/object/set_sprite_priority.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200810e)
        "*/games/tla/src/field/common/object/prepare_object.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200815e)
        "*/games/tla/src/field/overlay_66b/s0160.o"(.text)
        "*/games/tla/src/field/common/effect/move.o"(.text)
        "*/resource_66b_overlay.o"(.text.x020081ea)
        "*/resource_66b_overlay.o"(.text.x020081ec)
        "*/games/tla/src/field/overlay_66b/s03cc.o"(.text)
        "*/resource_66b_overlay.o"(.text.x020083f8)
        "*/games/tla/src/field/overlay_66b/s0584.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02008660)
        "*/games/tla/src/field/overlay_66b/s06f4.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02008738)
        "*/resource_66b_overlay.o"(.text.x02008768)
        "*/resource_66b_overlay.o"(.text.x020087d8)
        "*/resource_66b_overlay.o"(.text.x02008988)
        "*/games/tla/src/field/overlay_66b/s0bb4.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02008be4)
        "*/resource_66b_overlay.o"(.text.x02008dac)
        "*/resource_66b_overlay.o"(.text.x02008fcc)
        "*/resource_66b_overlay.o"(.text.x02009068)
        "*/games/tla/src/field/overlay_66b/import.o"(.text)
        "*/games/tla/src/field/overlay_66b/data.o"(.data)
        "*/resource_66b_overlay.o"(.rodata.x020093d8)
        "*/games/tla/src/field/overlay_66b/data2.o"(.data)
    }
    .bss : { "*/resource_66b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
