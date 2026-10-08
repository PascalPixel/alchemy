SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_star/entry.o"(.text)
        "*/games/tbs/src/field/soru_star/objects.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/soru_star/star.o"(.text)
        "*/games/tbs/src/field/soru_star/ring_setup.o"(.text)
        "*/games/tbs/src/field/soru_star/import.o"(.text)
        "*/games/tbs/src/field/soru_star/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
