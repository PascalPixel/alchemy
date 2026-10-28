SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gondowan_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0038.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/scene_tables.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s009c.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200810e)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/resource_66b_overlay.o"(.text.x0200815e)
        "*/games/tla/src/scene/gondowan_cliffs/s0160.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/resource_66b_overlay.o"(.text.x020081ea)
        "*/games/tla/src/scene/gondowan_cliffs/s01ec.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s03cc.o"(.text)
        "*/resource_66b_overlay.o"(.text.x020083f8)
        "*/games/tla/src/scene/gondowan_cliffs/s0584.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0660.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s06f4.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0738.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0768.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s07d8.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0988.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0bb4.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02008be4)
        "*/resource_66b_overlay.o"(.text.x02008dac)
        "*/games/tla/src/scene/gondowan_cliffs/s0fcc.o"(.text)
        "*/resource_66b_overlay.o"(.text.x02009068)
        "*/games/tla/src/scene/gondowan_cliffs/import.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/data.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/gondowan_cliffs/data2.o"(.data)
    }
    .bss : { "*/resource_66b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
