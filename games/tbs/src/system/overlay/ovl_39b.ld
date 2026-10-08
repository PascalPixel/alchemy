EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/makyuri_iriguchi/entry.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/effect_record.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/leader_sparks.o"(.text)
        "*/games/tbs/src/field/common/makyuri/palette_cycle.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/iriguchi.o"(.text)
        "*/games/tbs/src/field/common/makyuri/lights.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/rise_effect.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/pillar.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/clear_record.o"(.text)
        "*/games/tbs/src/field/common/makyuri/makyuri.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/release_attach.o"(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/makyuri_iriguchi/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
