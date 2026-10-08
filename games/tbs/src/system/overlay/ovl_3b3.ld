EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/takara_hashira/entry.o"(.text)
        "*/games/tbs/src/field/takara_hashira/hashira.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/common/map/panel.o"(.text)
        "*/games/tbs/src/field/takara_hashira/pillar.o"(.text)
        "*/games/tbs/src/field/takara_hashira/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/takara_hashira/data.o"(.data)
    }
    .bss : { "*/resource_3b3_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
