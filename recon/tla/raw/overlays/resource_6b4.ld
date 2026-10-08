SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b4/entry.o"(.text)
        "*/games/tla/src/field/common/object/set_sprite_priority.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x0200807e)
        "*/games/tla/src/field/common/effect/move.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020080b6)
        "*/resource_6b4_overlay.o"(.text.x020080b8)
        "*/games/tla/src/field/overlay_6b4/scene_tables.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020082a4)
        "*/resource_6b4_overlay.o"(.text.x0200832c)
        "*/resource_6b4_overlay.o"(.text.x02008394)
        "*/games/tla/src/field/common/effect/advance_motion.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x02008476)
        "*/resource_6b4_overlay.o"(.text.x02008478)
        "*/resource_6b4_overlay.o"(.text.x02008564)
        "*/games/tla/src/field/overlay_6b4/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b4/import.o"(.text)
        "*/resource_6b4_overlay.o"(.rodata.x0200959c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
