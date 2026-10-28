SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mt_aleph_summit/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x0200807e)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020080b6)
        "*/games/tla/src/scene/mt_aleph_summit/s00b8.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/scene_tables.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020082a4)
        "*/resource_6b4_overlay.o"(.text.x0200832c)
        "*/resource_6b4_overlay.o"(.text.x02008394)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x02008476)
        "*/resource_6b4_overlay.o"(.text.x02008478)
        "*/resource_6b4_overlay.o"(.text.x02008564)
        "*/games/tla/src/scene/mt_aleph_summit/prepare.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/import.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
