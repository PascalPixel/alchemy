SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/toreto_heya/entry.o"(.text)
        "*/games/tbs/src/field/toreto_heya/heya.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/toreto_heya/scene.o"(.text)
        "*/games/tbs/src/field/toreto_heya/import.o"(.text)
        "*/games/tbs/src/field/toreto_heya/data.o"(.data)
    }
    .bss : { "*/resource_396_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
