SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/goma_suiro/entry.o"(.text)
        "*/games/tbs/src/field/goma_suiro/objects.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/goma_suiro/suiro.o"(.text)
        "*/games/tbs/src/field/goma_suiro/burst.o"(.text)
        "*/games/tbs/src/field/goma_suiro/import.o"(.text)
        "*/games/tbs/src/field/goma_suiro/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
