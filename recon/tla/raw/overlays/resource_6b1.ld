SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/entry.o"(.text)
        "*/games/tla/src/field/shared/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/shared/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/spawn.o"(.text)
        "*/games/tla/src/field/shared/effect/move.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200815a)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s015c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/hide.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/scene_tables.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_a.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/reset.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_b.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x02008588)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/prepare.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x02008858)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/light.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/list.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/import.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/data.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.o"(.rodata)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/data2.o"(.data)
    }
    .bss : { "*/resource_6b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
