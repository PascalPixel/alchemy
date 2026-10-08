EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/babi_iriguchi/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/babi_iriguchi/objects.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/babi_iriguchi/iriguchi.o"(.text)
        "*/games/tbs/src/field/babi_iriguchi/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/babi_iriguchi/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
