SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/shian_heya/entry.o"(.text)
        "*/games/tbs/src/field/shian_heya/object_spawn.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/shian_heya/heya.o"(.text)
        "*/games/tbs/src/field/shian_heya/import.o"(.text)
        "*/games/tbs/src/field/shian_heya/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
