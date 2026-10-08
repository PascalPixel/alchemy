SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/shian_jiin/entry.o"(.text)
        "*/games/tbs/src/field/common/shian_jiin/new_effect.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/common/shian_jiin/jiin.o"(.text)
        "*/games/tbs/src/field/common/shian_jiin/import.o"(.text)
        "*/games/tbs/src/field/common/shian_jiin/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
