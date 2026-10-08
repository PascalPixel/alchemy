EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/EFFECT_RECORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PAIR_BURST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/PALETTE_CYCLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/LIGHTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RISE_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/PILLAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RELEASE_TARGET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/MAKYURI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RELEASE_ATTACH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/DATA.o"(.data)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
