SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b1/entry.o"(.text)
        "*/games/tla/src/field/common/object/set_sprite_priority.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200807e)
        "*/games/tla/src/field/common/object/prepare_object.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x020080ce)
        "*/resource_6b1_overlay.o"(.text.x020080d0)
        "*/games/tla/src/field/common/effect/move.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200815a)
        "*/resource_6b1_overlay.o"(.text.x0200815c)
        "*/games/tla/src/field/overlay_6b1/hide.o"(.text)
        "*/games/tla/src/field/overlay_6b1/scene_tables.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200835c)
        "*/resource_6b1_overlay.o"(.text.x02008388)
        "*/resource_6b1_overlay.o"(.text.x02008408)
        "*/games/tla/src/field/overlay_6b1/reset.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x020084e4)
        "*/resource_6b1_overlay.o"(.text.x02008534)
        "*/resource_6b1_overlay.o"(.text.x02008588)
        "*/resource_6b1_overlay.o"(.text.x02008714)
        "*/resource_6b1_overlay.o"(.text.x02008740)
        "*/games/tla/src/field/overlay_6b1/prepare.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x02008858)
        "*/resource_6b1_overlay.o"(.text.x02008a98)
        "*/resource_6b1_overlay.o"(.text.x02008b18)
        "*/games/tla/src/field/overlay_6b1/list.o"(.text)
        "*/games/tla/src/field/overlay_6b1/import.o"(.text)
        "*/resource_6b1_overlay.o"(.rodata.x02008d6c)
    }
    .bss : { "*/resource_6b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
