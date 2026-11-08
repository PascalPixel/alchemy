SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mt_aleph_summit/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s00b8.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/scene_tables.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s02a4.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s032c.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s0394.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s0478.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s0564.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/prepare.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/import.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
