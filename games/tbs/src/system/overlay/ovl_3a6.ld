EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_dou/entry.o"(.text)
        "*/games/tbs/src/field/haidia_dou/objects.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/haidia_dou/dou.o"(.text)
        "*/games/tbs/src/field/haidia_dou/probed_column_scene.o"(.text)
        "*/games/tbs/src/field/haidia_dou/actor_nine.o"(.text)
        "*/games/tbs/src/field/haidia_dou/lowered_actor_scene.o"(.text)
        "*/games/tbs/src/field/haidia_dou/scene.o"(.text)
        "*/games/tbs/src/field/haidia_dou/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/haidia_dou/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
