SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/runpa_mura/entry.o"(.text)
        "*/games/tbs/src/field/runpa_mura/effect.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/runpa_mura/mura.o"(.text)
        "*/games/tbs/src/field/runpa_mura/import.o"(.text)
        "*/games/tbs/src/field/runpa_mura/effect.o"(.rodata)
        "*/games/tbs/src/field/runpa_mura/village.o"(.rodata)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
