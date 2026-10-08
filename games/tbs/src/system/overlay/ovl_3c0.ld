SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/suhara_sabaku/entry.o"(.text)
        "*/games/tbs/src/field/suhara_sabaku/selected_presentation.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/suhara_sabaku/sabaku.o"(.text)
        "*/games/tbs/src/field/suhara_sabaku/import.o"(.text)
        "*/games/tbs/src/field/suhara_sabaku/data.o"(.data)
    }
    .bss (NOLOAD) : { "*/resource_3c0_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
