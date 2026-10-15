SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b1/entry.o"(.text)
        "*/games/tla/src/field/common/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/common/object/prepare_object.o"(.text)
        "*/games/tla/src/field/overlay_6b1/spawn.o"(.text)
        "*/games/tla/src/field/common/effect/move.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200815a)
        "*/resource_6b1_overlay.o"(.text.x0200815c)
        "*/games/tla/src/field/overlay_6b1/hide.o"(.text)
        "*/games/tla/src/field/overlay_6b1/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b1/events_a.o"(.text)
        "*/games/tla/src/field/overlay_6b1/reset.o"(.text)
        "*/games/tla/src/field/overlay_6b1/events_b.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x02008588)
        "*/games/tla/src/field/overlay_6b1/events_c.o"(.text)
        "*/games/tla/src/field/overlay_6b1/prepare.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x02008858)
        "*/games/tla/src/field/overlay_6b1/light.o"(.text)
        "*/games/tla/src/field/overlay_6b1/list.o"(.text)
        "*/games/tla/src/field/overlay_6b1/import.o"(.text)
        "*/games/tla/src/field/overlay_6b1/data.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.o"(.rodata)
        "*/games/tla/src/field/overlay_6b1/data2.o"(.data)
    }
    .bss : { "*/resource_6b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
